#pragma once
#ifndef IO_HPP
#define IO_HPP
#include "conf.h"
#include "Menus.hpp"

namespace IO
{
    typedef struct State
    {
        MenuType menuType = MODULE_VIEW;
        Menu menus[MAX_MENUS];
        Menu* activeMenu = &menus[0];

        int menuIndex = 0;
        int interactIndex = 0;
    } State;
    class IO
    {
        public:
        IO();
        ~IO(){}

        void display();
        void updateAndTransmitData();

        private:
        State state;
    };
}

#endif