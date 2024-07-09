#include "Browser.hpp"

/*===========================================  Menu =========================================================*/
void Browser::menu()
{
    UART_Keyboard_Input* pInput;
    menuView.display_menu();
    pInput = UART_Keyboard();
    int MenuInput = userInput();
    switch (MenuInput)
    {
    /*PUSH MEDIALIST TO STACK*/
    case MEDIA_LIST:
        flowID.push(MEDIA_LIST_ID);
        break;
    /*PUSH PLAYLIST TO STACK*/
    case PLAY_LIST:
        flowID.push(PLAY_LIST_ID);
        break;
    /*PUSH PLAYMUSC TO STACK*/
    case PLAY_MUSIC:
        flowID.push(PLAY_MUSIC_ID);
        break;
    /*EXIT TO PATH*/
    case EXIT:
        myPlayer.ExitAudio();
        FreeAll();
        flowID.pop();
        list = 1;
        break;
    default:
        break;
    }
}

