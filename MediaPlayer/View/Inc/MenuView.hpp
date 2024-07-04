#ifndef MENUVIEW_HPP
#define MENUVIEW_HPP

#include "main.hpp"

enum MenuChoices
{
    EXIT        =   0,
    MEDIA_LIST  =   1,
    PLAY_LIST   =   2,
    PLAY_MUSIC  =   3
};

using namespace std;

class MenuView
{
private:
public:
    MenuView()=default;
    ~MenuView()= default;
    void display_menu();
    void InvalidChoiceInterface();
    void exitProgram();
};
#endif/*MENUVIEW_HPP*/