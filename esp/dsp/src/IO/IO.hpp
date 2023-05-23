#pragma once
#ifndef IO_HPP
#define IO_HPP
#include "conf.h"
#include <Arduino.h>
#include <ESP32Encoder.h>
#include <LiquidCrystal_I2C.h>
// #include "Menus.hpp"

namespace IO
{
    // typedef struct State
    // {
    //     MenuType menuType = MODULE_VIEW;
    //     Menu menus[MAX_MENUS];
    //     Menu* activeMenu = &menus[0];

    //     int menuIndex = 0;
    //     int interactIndex = 0;
    // } State;
    typedef struct State State;
    typedef struct Symbol Symbol;
    class Menu;
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

        Menu* menu;
        
        void action();
    } Symbol;

    typedef struct Menu
    {
        char title[IO_MAX_STR_LEN];
        Symbol symbols[IO_MAX_SYMBOLS];
        uint8_t symInd = 0;
        uint8_t actSym = 0;

        actionFunc inputAction; // on esp

        State* state;

        void setTitle(const char* title);
        void addSymbol(Type type, const char* key, String currVal, void* valPtr);
        void addSymbol(Symbol* sym);
    } Menu;

    void standInputAction(bool buttPress, int encChange, Symbol* sym);
    // Edit symbol functions
    void editSymInt(bool buttPress, int encChange, Symbol* sym);
    void editSymFloat(bool buttPress, int encChange, Symbol* sym);
    void editSymBool(bool buttPress, int encChange, Symbol* sym);
    void editSymString(bool buttPress, int encChange, Symbol* sym);
    void editSymFilename(bool buttPress, int encChange, Symbol* sym);

    typedef struct State
    {
        Menu menus[IO_MAX_MENUS];
        uint8_t actMenu = 0;
        uint8_t menCount = 0;

        IO* io;

        void init();
        void addMenu(const char* title, uint8_t index);
        void changeActiveMenu(uint8_t index);
        void editSymbol(Symbol* symbol);
    } State;

    class IO
    {
        public:
        IO() : lcd(0x27, LCD_COLS, LCD_ROWS), serPort(2), prevEnc(0){};
        ~IO(){}

        void init();
        
        void display();
        void handleInput();

        void transmitUpdate();
        void receiveUpdate();

        State state;

        LiquidCrystal_I2C lcd;
        ESP32Encoder enc;
        HardwareSerial serPort;

        int prevEnc;
        // private:
        // State state;
        // char display[2][IO_LINE_SIZE];
    };
}

#endif