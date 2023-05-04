/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <utility/imxrt_hw.h>
#include "input_tdm.h"
#include "output_i2s.h"
#include "../../include/conf.h"

DMAMEM __attribute__((aligned(32))) static uint64_t i2s_rx_buffer[AUDIO_BLOCK_SAMPLES];
BufferQueue AudioInputTDM::buffers;
DMAChannel AudioInputTDM::dma(false);
int32_t *outBuffers[2]; // temporary holder for the values returned by getData

// extern bool inUpdates;
// extern void setupModInterrupt();

void AudioInputTDM::begin()
{
    switch(ADDA_I2S_INTERFACE)
    {
        case 1:
        {
            break;
        }
        case 2:
        {
            dma.begin(true); // Allocate the DMA channel first

            config_tdm();

            AD_DATA_PIN_CONF = 2; // 1:RX_DATA0
            // CORE_PIN8_CONFIG  = 3;  //1:RX_DATA0
            IOMUXC_SAI2_RX_DATA0_SELECT_INPUT = 0;

            dma.TCD->SADDR = (void *)((uint32_t)&I2S2_RDR0 + 0);		   // source address, read from 0 byte offset as we want the full 32 bits
            dma.TCD->SOFF = 0;											   // how many bytes to jump from current address on the next move. We're always reading the same register so no jump.
            dma.TCD->ATTR = DMA_TCD_ATTR_SSIZE(2) | DMA_TCD_ATTR_DSIZE(2); // 1=16bits, 2=32 bits. size of source, size of dest
            dma.TCD->NBYTES_MLNO = 4;									   // number of bytes to move, minor loop.
            dma.TCD->SLAST = 0;											   // how many bytes to jump when hitting the end of the major loop. In this case, no change to the source address.
            dma.TCD->DADDR = i2s_rx_buffer;								   // Destination address.
            dma.TCD->DOFF = 4;											   // how many bytes to move the destination at each minor loop. jump 4 bytes.
            dma.TCD->CITER_ELINKNO = sizeof(i2s_rx_buffer) / 4;			   // how many iterations are in the major loop
            dma.TCD->DLASTSGA = -sizeof(i2s_rx_buffer);					   // how many bytes to jump the destination address at the end of the major loop
            dma.TCD->BITER_ELINKNO = sizeof(i2s_rx_buffer) / 4;			   // beginning iteration count
            dma.TCD->CSR = DMA_TCD_CSR_INTHALF | DMA_TCD_CSR_INTMAJOR;	   // Tells the DMA mechanism to trigger interrupt at half and full population of the buffer
            dma.triggerAtHardwareEvent(DMAMUX_SOURCE_SAI2_RX);			   // run DMA at hardware event when new I2S data transmitted.

            // Enabled transmitting and receiving
            I2S2_RCSR = I2S_RCSR_RE | I2S_RCSR_BCE | I2S_RCSR_FRDE | I2S_RCSR_FR;

            dma.enable();
            dma.attachInterrupt(isr);
        }
    }
	
    // inUpdates = true;
    // setupModInterrupt();
}

int32_t **AudioInputTDM::getData()
{
	outBuffers[0] = buffers.readPtr[0];
	outBuffers[1] = buffers.readPtr[1];
	buffers.consume();
	return outBuffers;
}

void AudioInputTDM::isr(void)
{
	uint32_t daddr, offset;
	const int32_t *src;
	int32_t *dest_left, *dest_right;
	bool incrementQueue;

	daddr = (uint32_t)(dma.TCD->DADDR);
	dma.clearInterrupt();

	if (daddr < (uint32_t)i2s_rx_buffer + sizeof(i2s_rx_buffer) / 2)
	{
		// DMA is receiving to the first half of the buffer
		// need to remove data from the second half
		src = (int32_t *)&i2s_rx_buffer[AUDIO_BLOCK_SAMPLES*16];
		offset = AUDIO_BLOCK_SAMPLES / 2;
		incrementQueue = true;

        // if(inUpdates) NVIC_SET_PENDING(IRQ_SOFTWARE);
	}
	else
	{
		// DMA is receiving to the second half of the buffer
		// need to remove data from the first half
		src = (int32_t *)&i2s_rx_buffer[0];
		offset = 0;
		incrementQueue = false;
	}

	int32_t *left = buffers.writePtr[0];
	int32_t *right = buffers.writePtr[1];
	dest_left = &(left[offset]);
	dest_right = &(right[offset]);

	for (size_t i = 0; i < AUDIO_BLOCK_SAMPLES / 2; ++i)
	{
		dest_left[i] = src[2 * i];
		dest_right[i] = src[2 * i + 1];
	}

	if (incrementQueue)
	{
		buffers.publish();
	}

	arm_dcache_delete((void *)src, sizeof(i2s_rx_buffer) / 2);
}

void AudioInputTDM::config_tdm()
{
    #if defined(KINETISK)
        SIM_SCGC6 |= SIM_SCGC6_I2S;
        SIM_SCGC7 |= SIM_SCGC7_DMA;
        SIM_SCGC6 |= SIM_SCGC6_DMAMUX;

        // if either transmitter or receiver is enabled, do nothing
        if (I2S0_TCSR & I2S_TCSR_TE) return;
        if (I2S0_RCSR & I2S_RCSR_RE) return;

        // enable MCLK output
        I2S0_MCR = I2S_MCR_MICS(MCLK_SRC) | I2S_MCR_MOE;
        while (I2S0_MCR & I2S_MCR_DUF) ;
        I2S0_MDR = I2S_MDR_FRACT((MCLK_MULT-1)) | I2S_MDR_DIVIDE((MCLK_DIV-1));

        // configure transmitter
        I2S0_TMR = 0;
        I2S0_TCR1 = I2S_TCR1_TFW(4);
        I2S0_TCR2 = I2S_TCR2_SYNC(0) | I2S_TCR2_BCP | I2S_TCR2_MSEL(1)
            | I2S_TCR2_BCD | I2S_TCR2_DIV(0);
        I2S0_TCR3 = I2S_TCR3_TCE;
        I2S0_TCR4 = I2S_TCR4_FRSZ(7) | I2S_TCR4_SYWD(0) | I2S_TCR4_MF
            | I2S_TCR4_FSE | I2S_TCR4_FSD;
        I2S0_TCR5 = I2S_TCR5_WNW(31) | I2S_TCR5_W0W(31) | I2S_TCR5_FBT(31);

        // configure receiver (sync'd to transmitter clocks)
        I2S0_RMR = 0;
        I2S0_RCR1 = I2S_RCR1_RFW(4);
        I2S0_RCR2 = I2S_RCR2_SYNC(1) | I2S_TCR2_BCP | I2S_RCR2_MSEL(1)
            | I2S_RCR2_BCD | I2S_RCR2_DIV(0);
        I2S0_RCR3 = I2S_RCR3_RCE;
        I2S0_RCR4 = I2S_RCR4_FRSZ(7) | I2S_RCR4_SYWD(0) | I2S_RCR4_MF
            | I2S_RCR4_FSE | I2S_RCR4_FSD;
        I2S0_RCR5 = I2S_RCR5_WNW(31) | I2S_RCR5_W0W(31) | I2S_RCR5_FBT(31);

        // configure pin mux for 3 clock signals
        CORE_PIN23_CONFIG = PORT_PCR_MUX(6); // pin 23, PTC2, I2S0_TX_FS (LRCLK) - 44.1kHz
        CORE_PIN9_CONFIG  = PORT_PCR_MUX(6); // pin  9, PTC3, I2S0_TX_BCLK  - 11.2 MHz
        CORE_PIN11_CONFIG = PORT_PCR_MUX(6); // pin 11, PTC6, I2S0_MCLK - 22.5 MHz

    #elif defined(__IMXRT1062__)
        CCM_CCGR5 |= CCM_CCGR5_SAI2(CCM_CCGR_ON);

        // if either transmitter or receiver is enabled, do nothing
        if (I2S2_TCSR & I2S_TCSR_TE) return;
        if (I2S2_RCSR & I2S_RCSR_RE) return;
        //PLL:
        int fs = SAMPLERATE;
        // PLL between 27*24 = 648MHz und 54*24=1296MHz
        int n1 = 4; //SAI prescaler 4 => (n1*n2) = multiple of 4
        int n2 = 1 + (24000000 * 27) / (fs * 256 * n1);

        double C = ((double)fs * 256 * n1 * n2) / 24000000;
        int c0 = C;
        int c2 = 10000;
        int c1 = C * c2 - (c0 * c2);
        set_audioClock(c0, c1, c2);
        // clear SAI2_CLK register locations
        CCM_CSCMR1 = (CCM_CSCMR1 & ~(CCM_CSCMR1_SAI2_CLK_SEL_MASK)) | CCM_CSCMR1_SAI2_CLK_SEL(2);										// &0x03 // (0,1,2): PLL3PFD0, PLL5, PLL4

        n1 = n1 / 2; //Double Speed for TDM

        CCM_CS2CDR = (CCM_CS2CDR & ~(CCM_CS2CDR_SAI2_CLK_PRED_MASK | CCM_CS2CDR_SAI2_CLK_PODF_MASK))
            | CCM_CS2CDR_SAI2_CLK_PRED(n1-1) // &0x07
            | CCM_CS2CDR_SAI2_CLK_PODF(n2-1); // &0x3f

        IOMUXC_GPR_GPR1 = (IOMUXC_GPR_GPR1 & ~(IOMUXC_GPR_GPR1_SAI2_MCLK3_SEL_MASK)) | (IOMUXC_GPR_GPR1_SAI2_MCLK_DIR | IOMUXC_GPR_GPR1_SAI2_MCLK3_SEL(0));	//Select MCLK

        // configure transmitter
        int rsync = 0;
        int tsync = 1;

        I2S2_TMR = 0;
        I2S2_TCR1 = I2S_TCR1_RFW(4);
        I2S2_TCR2 = I2S_TCR2_SYNC(tsync) | I2S_TCR2_BCP | I2S_TCR2_MSEL(1)
            | I2S_TCR2_BCD | I2S_TCR2_DIV(0);
        I2S2_TCR3 = I2S_TCR3_TCE;
        I2S2_TCR4 = I2S_TCR4_FRSZ(7) | I2S_TCR4_SYWD(0) | I2S_TCR4_MF
            | I2S_TCR4_FSE | I2S_TCR4_FSD;
        I2S2_TCR5 = I2S_TCR5_WNW(31) | I2S_TCR5_W0W(31) | I2S_TCR5_FBT(31);

        I2S2_RMR = 0;
        I2S2_RCR1 = I2S_RCR1_RFW(4);
        I2S2_RCR2 = I2S_RCR2_SYNC(rsync) | I2S_TCR2_BCP | I2S_RCR2_MSEL(1)
            | I2S_RCR2_BCD | I2S_RCR2_DIV(0);
        I2S2_RCR3 = I2S_RCR3_RCE;
        I2S2_RCR4 = I2S_RCR4_FRSZ(7) | I2S_RCR4_SYWD(0) | I2S_RCR4_MF
            | I2S_RCR4_FSE | I2S_RCR4_FSD;
        I2S2_RCR5 = I2S_RCR5_WNW(31) | I2S_RCR5_W0W(31) | I2S_RCR5_FBT(31);

        ADDA_MCLK_PIN_CONF = 2;  // 1:MCLK
        ADDA_LRCLK_PIN_CONF = 2; // RX_SYNC  (LRCLK)
        ADDA_BCLK_PIN_CONF = 2; // 1:RX_BCLK
    #endif
}