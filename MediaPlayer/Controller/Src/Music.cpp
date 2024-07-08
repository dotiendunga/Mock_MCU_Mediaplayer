#include "Browser.hpp"

void Browser::playmusic_player(int& chosenList, int& chosenMusic)
{
    UART_Keyboard_Input* pInput;
    mediaPlayerView.VPlayerMusic_DisplayMusic(vPlayList[chosenList - 1]->getPlaylist(), list, timelapse.count(), duration, myPlayer);

    pInput = UART_Keyboard();
    if(pInput == NULL) return;
    if(pInput->source == SOURCE_KEYBROAD)
    {
        if(pInput->keyboardData.keyboardType == STRING_TYPE)
        {
            processInput(pInput->keyboardData.valueString[0]);
        }
        else
        {
            if(pInput->keyboardData.valueNumber > 0 && pInput->keyboardData.valueNumber <= vPlayList[chosenList - 1]->getPlaylist().size())
            {
                chosenMusic = pInput->keyboardData.valueNumber;
                resetTimer();
                myPlayer.setIndexInList(chosenMusic);
                myPlayer.playMusic();
            }
        }
    }
    else
    {
        char option;
        switch(pInput->uartData.uartType)
        {
        case BUTTON1_BYTE:
            if(pInput->uartData.valueNumber > 0 && pInput->uartData.valueNumber <= vPlayList[chosenList - 1]->getPlaylist().size())
            {
                chosenMusic = pInput->uartData.valueNumber;
                resetTimer();
                myPlayer.setIndexInList(chosenMusic);
                myPlayer.playMusic();
            }
            break;
        case BUTTON2_BYTE:
            switch (pInput->uartData.valueNumber)
            {
            case 1:
                option = 'r';
                break;
            case 2:
                option = '-';
                break;
            case 3:
                option = '+';
                break;
            case 4:
                option = 'e';
                break;
            case 5:
                option = 'n';
                break;
            case 6:
                option = 'p';
                break;
            case 7:
                option = 'a';
                break;
            default:
                break;
            }
            processInput(option);
            break;
        case ADC_BYTE:
            myPlayer.setVolume(pInput->uartData.valueNumber * 2);
            break;
        default:
            break;
        }
    }
}

void Browser::processInput(char option)
{
    switch (option)
    {
    case 'E':
    case 'e':
        list = 1;
        flowID.pop();
        myThread.join();
        break;
    case 'U':
    case 'u':
        myPlayer.VolumeUp();
        break;
    case 'D':
    case 'd':
        myPlayer.VolumeDown();
        break;
    case 'R':
    case 'r':
        myPlayer.ResumePause();
        break;
    case '+':
        {
        std::lock_guard<std::mutex> lock1(mtx1);
        myPlayer.nextMusic();
        }
        resetTimer();
        break;
    case '-':
        {
        std::lock_guard<std::mutex> lock1(mtx1);
        myPlayer.preMusic();
        }
        resetTimer();
        break;
    case 'a':
    case 'A':
        // Case Auto or Repeat'
        if(myPlayer.getFlagAuto() ==true)
        {
            myPlayer.setFlagAuto(false);
        }
        else
        {
            myPlayer.setFlagAuto(true);
        }
        break;
    case 'P':
    case 'p':
        if (list > 1)
        {
            list--;
        } 
        break;
    case 'N':
    case 'n':
        if (list < 4)
        {
            list++;
        }
    default:
        
        break;
    }
}