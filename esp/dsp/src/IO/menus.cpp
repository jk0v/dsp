// #include "menus.hpp"
// #include "Wire.h"

// namespace IO
// {
//     // MenuNode
//     void MenuNode::init(const char string[MAX_STRING_LENGTH], Menu* subMenu)
//     {
//         strstr((char*)this->string, string);
//         this->subMenu = subMenu;
//     }
//     // ModValue
//     void ModValue::init(char string[MAX_STRING_LENGTH], int initVal)
//     {
//         strstr((char*)this->string, string);
//         this->val = initVal;
//     }
//     // ModuleNode
//     void ModuleNode::init(const char* title, Menu* subMenu)
//     {
//         strstr((char*)this->title, title);
//         this->subMenu = subMenu;
//     }

//     // ModuleViewMenu
//     ModuleViewMenu::ModuleViewMenu(ModuleType* modules, int amountModules)
//     {
//         menuType = MODULE_VIEW;
        
//     }
    
//     void ModuleViewMenu::requestModulesI2C(int addr)
//     {
//         // Wire.requestFrom()
//     } 
// }