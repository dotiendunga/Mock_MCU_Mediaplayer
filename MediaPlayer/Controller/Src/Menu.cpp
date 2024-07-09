#include "Browser.hpp"

/*===========================================  Menu =========================================================*/
void Browser::menu()
{
    UART_Keyboard_Input* pInput;
    int chosenMenu;
    menuView.display_menu();
    pInput = UART_Keyboard();
    if(pInput == NULL) return;

    if(pInput->source == SOURCE_KEYBROAD)
    {
        if(pInput->keyboardData.keyboardType == STRING_TYPE)
        {
            // INPUT STRING -> RESET VIEW MENU
            Menu_ProcessInput(-1);
            
        }
        else
        {
            chosenMenu = pInput->keyboardData.valueNumber;
            Menu_ProcessInput(chosenMenu);
        }
    }
    else
    {
        switch(pInput->uartData.uartType)
        {
        case BUTTON1_BYTE:
            chosenMenu = pInput->uartData.valueNumber;
            Menu_ProcessInput(chosenMenu);
            break;
        case BUTTON2_BYTE:
            chosenMenu = pInput->uartData.valueNumber;
            Menu_ProcessInput(chosenMenu);
            break;
        case ADC_BYTE:
            myPlayer.setVolume(pInput->uartData.valueNumber * 2);
            break;
        default: 



        
            break;
        }
    }
}
void Browser::Menu_ProcessInput(int MenuInput){
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