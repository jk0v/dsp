#pragma once
#ifndef IO_HPP
#define IO_HPP
#include "conf.h"
#include <string.h>
#include <Arduino.h>

namespace IO
{
    // typedef struct State State; typedef struct Symbol Symbol; typedef struct Menu Menu; class IO;
    typedef struct State State;
    typedef struct Symbol Symbol;
    class IO;

    // actionFunc(bool buttonPressed, int encoderChange, Symbol* symbol)
    typedef void (*actionFunc)(bool, int, Symbol*);

    enum Type
    {
        LINK,
        INT32,
        FLOAT,
        BOOL,
        STRING,
        FILENAME
    };

    typedef struct Symbol
    {
        Type type;
        char key[IO_MAX_STR_LEN];
        String currVal;
        char currValBuf[IO_MAX_STR_LEN];
        void* valPtr;

        State* state;
        
        void action();
    } Symbol;

    typedef struct Menu
    {
        char title[IO_MAX_STR_LEN];
        Symbol symbols[IO_MAX_SYMBOLS];
        uint8_t symInd = 0;
        uint8_t actSym = 0;

        // actionFunc inputAction; // on esp

        void setTitle(const char* title);
        void addSymbol(Type type, const char* key, String currVal, void* valPtr);
        void addSymbol(Symbol* sym);
    } Menu;
    typedef struct State
    {
        Menu menus[IO_MAX_MENUS];
        uint8_t actMenu = 0;
        uint8_t menCount = 0;

        IO* io;

        // void init(); // on esp
        void addMenu(const char* title, uint8_t index);
        void changeActiveMenu(uint8_t index);
        void editSymbol(Symbol* symbol);
    } State;

    class IO
    {
        public:
        IO(){};
        ~IO(){}

        // void init(); // on esp

        // void display(); // on esp
        // void handleInput();
        
        void transmitUpdate();
        void receiveUpdate();

        State state;
        private:
        // State state;
        // char display[2][IO_LINE_SIZE];
    };
}

#endif