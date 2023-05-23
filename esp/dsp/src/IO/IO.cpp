#include "IO.hpp"

namespace IO
{
    // IO
    void IO::init()
    {
        state.io = this;

        pinMode(ENC_PIN_SW, INPUT_PULLUP);
        lcd.init();
        lcd.backlight();

        enc.attachHalfQuad(ENC_PIN_DT, ENC_PIN_CLK);
        enc.setCount(0);

        // state.io->display();
        // serPort.begin(UART_BAUD, SERIAL_8N1, 16, 17);
        // Serial.println("3");
        // while(!serPort.available()) 
        // {
        //     serPort.write(1);
        //     Serial.println("30");
        //     delay(100);
        //     continue;
        // }
        // serPort.read();

        // delay(5000);

        state.init();
    }
    void IO::transmitUpdate()
    {
        // Serial.println("23");
        // serPort.write(state.actMenu);
        // serPort.write(state.menCount);
        // Serial.println("24");
        // for(int i=0; i<state.menCount; ++i)
        // {
        //     serPort.write(state.menus[i].title, sizeof(state.menus[i].title));
        //     Serial.println("25");
        //     serPort.write(state.menus[i].symInd);
        //     serPort.write(state.menus[i].actSym);
        //     Serial.println("26");

        //     for(int j=0; j<state.menus[i].symInd; ++j)
        //     {
        //         serPort.write(state.menus[i].symbols[j].type);
        //         serPort.write(state.menus[i].symbols[j].key, sizeof(state.menus[i].symbols[j].key));
        //         Serial.println("27");

        //         state.menus[i].symbols[j].currVal.toCharArray(state.menus[i].symbols[j].currValBuf, sizeof(state.menus[i].symbols[j].currValBuf));
        //         Serial.println("28");
        //         serPort.write(state.menus[i].symbols[j].currValBuf, sizeof(state.menus[i].symbols[j].currValBuf));
        //         Serial.println("29");
        //     }
        // }
    }
    void IO::receiveUpdate()
    {
        state.actMenu = serPort.read();
        state.menCount = serPort.read();

        for(int i=0; i<state.menCount; ++i)
        {
            serPort.readString().toCharArray(state.menus[i].title, IO_MAX_STR_LEN);
            state.menus[i].symInd = serPort.read();
            state.menus[i].actSym = serPort.read();
            
            for(int j=0; j<state.menus[i].symInd; ++j)
            {
                state.menus[i].symbols[j].type = (Type)serPort.read();
                serPort.readString().toCharArray(state.menus[i].symbols[j].key, IO_MAX_STR_LEN);
                state.menus[i].symbols[j].currVal = serPort.readString();
            }
        }
        
        state.changeActiveMenu(state.actMenu);
    }
    void IO::handleInput()
    {
        // Serial.println("131");
        // int encChange = enc.getCount() - prevEnc;
        // bool buttPress = (bool)digitalRead(ENC_PIN_SW);
        // if(encChange == 0 && !buttPress) return;
        // prevEnc = enc.getCount();

        // Serial.println("411");

        // state.menus[state.actMenu].inputAction(buttPress, encChange, &state.menus[state.actMenu].symbols[state.menus[state.actMenu].actSym]);

        display();
    }
    void IO::display()
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(state.menus[state.actMenu].title);

        lcd.setCursor(0,1);
        for(int i=0; i<state.menus[state.actMenu].symInd; ++i)
        {
            lcd.printf("%s> ", state.menus[state.actMenu].symbols[i].key);
        }
    }

    // Symbol
    void Symbol::action()
    {
        Serial.println("121");
        if(type == LINK)
        {
            menu->state->changeActiveMenu(currVal.toInt());
        } else {
            menu->state->editSymbol(this);
        }
    }

    // Menu
    void Menu::setTitle(const char* title)
    {
        strncpy(this->title, title, IO_MAX_STR_LEN);
    }
    void Menu::addSymbol(Type type, const char* key, String currVal, void* valPtr)
    {
        if(symInd >= IO_MAX_SYMBOLS) return;

        symbols[symInd].type = type;
        strncpy(symbols[symInd].key, key, IO_MAX_STR_LEN);
        symbols[symInd].currVal = currVal;
        symbols[symInd].valPtr = valPtr;
        symbols[symInd].menu = this;

        actSym = symInd;
        symInd++;
    }
    void Menu::addSymbol(Symbol* sym)
    {
        if(symInd >= IO_MAX_SYMBOLS) return;

        symbols[symInd].type = sym->type;
        strncpy(symbols[symInd].key, sym->key, IO_MAX_STR_LEN);
        symbols[symInd].currVal = sym->currVal;
        symbols[symInd].valPtr = sym->valPtr;

        actSym = symInd;
        symInd++;
    }

    // State
    void State::init()
    {
        addMenu("Edit symbol:", IO_INT32_MENU);
        menus[IO_INT32_MENU].inputAction = (actionFunc)editSymInt;
        addMenu("Edit symbol:", IO_FLOAT_MENU);
        menus[IO_FLOAT_MENU].inputAction = (actionFunc)editSymFloat;
        addMenu("Edit symbol:", IO_BOOL_MENU);
        menus[IO_BOOL_MENU].inputAction = (actionFunc)editSymBool;
        addMenu("Edit symbol:", IO_STRING_MENU);
        menus[IO_STRING_MENU].inputAction = (actionFunc)editSymString;
        addMenu("Edit symbol:", IO_FILENAME_MENU);
        menus[IO_FILENAME_MENU].inputAction = (actionFunc)editSymFilename;

        menus[0].setTitle("Effects");
        addMenu("Play!", 1);
        menus[0].addSymbol(LINK, "Play!", String(1), nullptr);
        
        changeActiveMenu(0);
    }
    void State::addMenu(const char* title, uint8_t index)
    {
        if(menCount >= IO_MAX_MENUS) return;
        menus[index].setTitle(title);
        menus[index].state = this;
        menus[index].inputAction = (actionFunc)standInputAction;

        menCount++;
        io->transmitUpdate();
    }
    void State::changeActiveMenu(uint8_t index)
    {
        if(index > menCount) return;
        actMenu = index;
        
        io->display();
        io->transmitUpdate();
    }
    void State::editSymbol(Symbol* symbol)
    {
        switch(symbol->type)
        {
            case INT32:
                menus[IO_INT32_MENU].addSymbol(symbol);
                Serial.println("11");
                changeActiveMenu(IO_INT32_MENU);
                break;
            case FLOAT:
                menus[IO_FLOAT_MENU].addSymbol(symbol);
                changeActiveMenu(IO_FLOAT_MENU);
                break;
            case BOOL:
                menus[IO_BOOL_MENU].addSymbol(symbol);
                changeActiveMenu(IO_BOOL_MENU);
                break;
            case STRING:
                menus[IO_BOOL_MENU].addSymbol(symbol);
                changeActiveMenu(IO_STRING_MENU);
                break;
            case FILENAME:
                menus[IO_FILENAME_MENU].addSymbol(symbol);
                changeActiveMenu(IO_FILENAME_MENU);
                break;
        }
    }

    void standInputAction(bool buttPress, int encChange, Symbol* sym)
    {
        Serial.println("1561");
        if(!buttPress && encChange == 0) return;
        Serial.println("1331");
        if(encChange != 0)
        {
            Serial.println("1661");
            sym->menu->actSym = (sym->menu->actSym + encChange)%sym->menu->symInd;
        }
        if(buttPress)
        {
            Serial.println("1551");
            sym->menu->symbols[sym->menu->actSym].action();
        }
        Serial.println("1441");
    }

    // Edit symbol functions
    void editSymInt(bool buttPress, int encChange, Symbol* sym)
    {
        int currVal = sym->currVal.toInt() + encChange;
        sym->currVal = String(currVal);
    }
    void editSymFloat(bool buttPress, int encChange, Symbol* sym)
    {
        float currVal = sym->currVal.toFloat() + encChange*IO_FLOAT_STEP;
        sym->currVal = String(currVal);
    }
    void editSymBool(bool buttPress, int encChange, Symbol* sym)
    {
        int currVal = (sym->currVal.toInt() + encChange)%2;
        sym->currVal = String(currVal);
    }
    void editSymString(bool buttPress, int encChange, Symbol* sym)
    {
        // int currVal = (sym->currVal.toInt() + encChange)%2;
        // sym->currVal = String(currVal);
    }
    void editSymFilename(bool buttPress, int encChange, Symbol* sym)
    {
        // int currVal = (sym->currVal.toInt() + encChange)%2;
        // sym->currVal = String(currVal);
    }
}