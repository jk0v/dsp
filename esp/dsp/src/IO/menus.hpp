// #pragma once
// #ifndef MENUS_HPP
// #define MENUS_HPP
// #include "conf.h"

// namespace IO
// {
//     class Menu;
//     enum MenuType
//     {
//         MODULE_VIEW
//     };
//     enum ModuleType
//     {
//         INPUT_I2S,
//         OUTPUT_I2S,
//         MIXER,
//         NN
//     };
//     typedef struct MenuNode
//     {
//         char string[MAX_STRING_LENGTH];
//         Menu* subMenu = nullptr;

//         void init(const char* string, Menu* subMenu);
//     } MenuNode;
//     typedef struct ModValue
//     {
//         char string[MAX_STRING_LENGTH];
//         int val = 0;
        
//         void init(char string[MAX_STRING_LENGTH], int initVal);
//     } ModValue;

//     typedef struct ModuleNode
//     {
//         char title[MAX_STRING_LENGTH];

//         Menu* subMenu = nullptr;

//         void init(const char* title, Menu* subMenu);
//     } ModuleNode;
//     class Menu
//     {
//         public:
//         Menu(){}
//         ~Menu(){}

//         virtual void display();
//         // virtual void onClick();
//         // virtual void onEncoder();
//         // virtual void transmitI2C();

//         MenuType menuType;
        
//         char menuTitle[MAX_STRING_LENGTH]; 
//         MenuNode* subMenus[MAX_MENU_NODES]; 
//     };

//     // indiv menus
//     class ModuleViewMenu : public Menu
//     {
//         public:
//         ModuleViewMenu(ModuleType* modules, int amountModules);
//         ~ModuleViewMenu(){}

//         void display() override;
//         // void onClick() override;
//         // void onEncoder() override;
//         // void transmitI2C() override;

//         void requestModulesI2C(int addr);

//         private:
//         MenuNode modNodes[MAX_MENU_NODES];
//         char modData[MAX_MENU_NODES*MAX_STRING_LENGTH];
//     };
// }
// #endif