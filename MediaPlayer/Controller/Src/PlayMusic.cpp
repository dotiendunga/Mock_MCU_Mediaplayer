#include "Browser.hpp"



/*============================================================= PLAYMUSI - LIST ===============================================================*/
void Browser::playmusic_plist(int& chosenList)
{   
    UART_Keyboard_Input* pInput;
    mediaPlayerView.VPlayerMusic_DisplayList(vPlayList, list);
    pInput = UART_Keyboard();
    // chosenList = mediaPlayerView.VPlayerMusic_InputList(vPlayList, list);
    if(pInput == NULL) return;
    if(pInput->source == SOURCE_KEYBROAD)
    {
        if(pInput->keyboardData.keyboardType == STRING_TYPE)
        {
            Plist_ProcessInput(pInput->keyboardData.valueString[0]);
        }
        else
        {
            if(pInput->keyboardData.valueNumber > 0 && pInput->keyboardData.valueNumber <= vPlayList.size())
            {
                chosenList = pInput->keyboardData.valueNumber;
                myPlayer.setList(vPlayList[chosenList - 1]->getPlaylistPointer());
                flowID.push(PLAY_MUSIC_PLAYER_ID);
                // UPDATE SHOW DISPLAY PLAYER IN PLAYMUSIC
                startThread();
            }
        }
    }
    else
    {
        char option;
        switch(pInput->uartData.uartType)
        {
        case BUTTON1_BYTE:
            if(pInput->uartData.valueNumber > 0 && pInput->uartData.valueNumber <= vPlayList.size())
            {
                chosenList = pInput->keyboardData.valueNumber;
                myPlayer.setList(vPlayList[chosenList - 1]->getPlaylistPointer());
                flowID.push(PLAY_MUSIC_PLAYER_ID);
                // UPDATE SHOW DISPLAY PLAYER IN PLAYMUSIC
                startThread();
            }
            break;
        case BUTTON2_BYTE:
            switch (pInput->uartData.valueNumber)
            {
            case 1:
                option = 'n';
                break;
            case 2:
                option = 'p';
                break;
            case 3:
                option = 'e';
            default:
                break;
            }
            Plist_ProcessInput(option);
            break;
        case ADC_BYTE:
            myPlayer.setVolume(pInput->uartData.valueNumber * 2);
            break;
        default:
            break;
        }
    }
}


void Browser::Plist_ProcessInput(char& UserOption)
{   
        switch (UserOption)
        {   
            /*NEXT PAGE*/
            case 'N':
            case 'n':
                if (list < (vPlayList.size() + PAGE_LIST_SIZE - 1) / PAGE_LIST_SIZE)
                {
                    list++;
                }

                    break;
            /*PREVIOUS PAGE*/
            case 'P':
            case 'p':
                if (list > 1)
                {
                    list--;
                }
                break;
            /*EXIT PAGE*/
            case 'E':
            case 'e':
                list = 1;
                flowID.pop();
                break;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
                    // cin.ignore();
        }
}


/*============================================================= PLAYMUSI - PLAYER ===============================================================*/

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
            Player_ProcessInput(pInput->keyboardData.valueString[0]);
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
            Player_ProcessInput(option);
            break;
        case ADC_BYTE:
            myPlayer.setVolume(pInput->uartData.valueNumber * 2);
            break;
        default:
            break;
        }
    }
}

void Browser::Player_ProcessInput(char option)
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