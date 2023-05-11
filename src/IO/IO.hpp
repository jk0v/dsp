#pragma once
#ifndef IO_HPP
#define IO_HPP
#include "conf.h"
#include <string.h>
#include <Arduino.h>

namespace IO
{
    // typedef struct State State; typedef struct Symbol Symbol; typedef struct Menu Menu; class IO;
    // enum Type
    // {
    //     INT32 = 0,
    //     FLOAT = 1
    // };
    // typedef struct State
    // {
    //     Menu menus[IO_MAX_MENUS];
    //     uint8_t actMenu;
    // } State;

    // typedef struct Symbol
    // {
    //     void* valPtr;
    //     Type type;
    //     char key[IO_MAX_STR_LEN];
    // } Symbol;

    // typedef struct Menu
    // {
    //     char title[IO_MAX_STR_LEN];
    //     Symbol symbols[IO_MAX_SYMBOLS];

    //     void setTitle(const char* tit)
    //     {
    //         strncpy(title, tit, IO_MAX_STR_LEN);
    //     }
    //     void addSymbol(Symbol sym)
    //     {
    //         if(symInd >= IO_MAX_SYMBOLS) return;
    //         symbols[symInd] = sym;
    //         symInd++;
    //     }
    //     uint8_t symInd = 0;
    // } Menu;

    // class IO
    // {
    //     public:
    //     IO(){}
    //     ~IO(){}

    //     void init();

    //     void updateState(State newState);
    //     void updateDisplay();

    //     void transmit();

    //     void receive();

    //     private:
    //     State state;
    //     char display[2][IO_LINE_SIZE];
    // };
}

#endif