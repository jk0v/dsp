#include "IO.hpp"

namespace IO
{
    // IO
    // void IO::init()
    // {
    //     state.init();
    // }
    void IO::transmitUpdate()
    {
        Serial1.write(state.actMenu);
        Serial1.write(state.menCount);
        for(int i=0; i<state.menCount; ++i)
        {
            Serial1.write(state.menus[i].title, sizeof(state.menus[i].title));
            Serial1.write(state.menus[i].symInd);
            Serial1.write(state.menus[i].actSym);

            for(int j=0; j<state.menus[i].symInd; ++j)
            {
                Serial1.write(state.menus[i].symbols[j].type);
                Serial1.write(state.menus[i].symbols[j].key, sizeof(state.menus[i].symbols[j].key));

                state.menus[i].symbols[j].currVal.toCharArray(state.menus[i].symbols[j].currValBuf, sizeof(state.menus[i].symbols[j].currValBuf));
                Serial1.write(state.menus[i].symbols[j].currValBuf, sizeof(state.menus[i].symbols[j].currValBuf));
            }
        }
    }
    void IO::receiveUpdate()
    {
        state.actMenu = Serial1.read();
        state.menCount = Serial1.read();

        for(int i=0; i<state.menCount; ++i)
        {
            Serial1.readString(IO_MAX_STR_LEN).toCharArray(state.menus[i].title, IO_MAX_STR_LEN);
            state.menus[i].symInd = Serial1.read();
            state.menus[i].actSym = Serial1.read();
            
            for(int j=0; j<state.menus[i].symInd; ++j)
            {
                state.menus[i].symbols[j].type = (Type)Serial1.read();
                Serial1.readString(IO_MAX_STR_LEN).toCharArray(state.menus[i].symbols[j].key, IO_MAX_STR_LEN);
                state.menus[i].symbols[j].currVal = Serial1.readString(IO_MAX_STR_LEN);
            }
        }
    }

    // Symbol
    void Symbol::action()
    {
        if(type == LINK)
        {
            state->changeActiveMenu(currVal.toInt());
        } else {
            state->editSymbol(this);
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
    // void State::init()
    // {
    //     addMenu("Edit symbol:", IO_INT32_MENU);
    //     addMenu("Edit symbol:", IO_FLOAT_MENU);
    //     addMenu("Edit symbol:", IO_BOOL_MENU);
    //     addMenu("Edit symbol:", IO_STRING_MENU);
    //     addMenu("Edit symbol:", IO_FILENAME_MENU);

    //     menus[0].setTitle("Effects");
    //     addMenu("Play!", 1);
    //     menus[0].addSymbol(LINK, "Play!", String(1), nullptr);
    //     actMenu = 0;
    // }
    void State::addMenu(const char* title, uint8_t index)
    {
        if(menCount >= IO_MAX_MENUS) return;
        
        menus[index].setTitle(title);
        menCount++;
        io->transmitUpdate();
    }
    void State::changeActiveMenu(uint8_t index)
    {
        if(index > menCount) return;

        actMenu = index;
        io->transmitUpdate();
    }
    void State::editSymbol(Symbol* symbol)
    {
        switch(symbol->type)
        {
            case INT32:
                changeActiveMenu(IO_INT32_MENU);
                break;
            case FLOAT:
                changeActiveMenu(IO_FLOAT_MENU);
                break;
            case BOOL:
                changeActiveMenu(IO_BOOL_MENU);
                break;
            case STRING:
                changeActiveMenu(IO_STRING_MENU);
                break;
            case FILENAME:
                changeActiveMenu(IO_FILENAME_MENU);
                break;
        }
    }
}