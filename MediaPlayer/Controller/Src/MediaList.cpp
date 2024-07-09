#include "Browser.hpp"

/*========================================== Option 1 in Menu =========================================================*/

/************************************* Media List ***************************************/
void Browser::MediaList()
{
    // choose_song = mediaFileView.check_choice(vPlayList[0]->getPlaylist(), list);
    UART_Keyboard_Input* pInput = nullptr;
    size_t chosentSong = 0;
    mediaFileView.display_MediaFile(vPlayList[0]->getPlaylist(), list);
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
            chosentSong = pInput->keyboardData.valueNumber;
            if (chosentSong > 0 && chosentSong <= vPlayList[0]->getPlaylist().size())
            {
                file_path = vPlayList[0]->getPlaylist()[chosentSong-1]->getPath();
                file_name = vPlayList[0]->getPlaylist()[chosentSong-1]->getName();
                file_type = vPlayList[0]->getPlaylist()[chosentSong-1]->getType();
                flowID.push(METADATA_LIST_ID);
            }
            else if((int)chosentSong == -1)
            {
                list = 1;
                flowID.pop();
                return;
            }
            else
            {
                /* Do nothing */
            }
        }
    }
    else
    {
        switch(pInput->uartData.uartType)
        {
        case BUTTON1_BYTE:
            chosentSong = pInput->uartData.valueNumber;
            if(chosentSong > 0 && chosentSong <= vPlayList[0]->getPlaylist().size())
            {
                file_path = vPlayList[0]->getPlaylist()[chosentSong-1]->getPath();
                file_name = vPlayList[0]->getPlaylist()[chosentSong-1]->getName();
                file_type = vPlayList[0]->getPlaylist()[chosentSong-1]->getType();
                flowID.push(METADATA_LIST_ID);
            }
            break;
        case BUTTON2_BYTE:
            switch (pInput->uartData.valueNumber)
            {
            case 1:
                break;
            case 2:
                break;
            case 3:
                flowID.pop();
                break;
            default:
                break;
            }
            break;
        case ADC_BYTE:
            myPlayer.setVolume(pInput->uartData.valueNumber * 2);
            break;
        default:
            break;
        }
    }
}

/************************************* Metadata opion view in Media List ***************************************/
void Browser::metadatalist()
{
    metadataView.menuMetaView();

    // int user_input = userInput();

    UART_Keyboard_Input* pInput = nullptr;
    mediaFileView.display_MediaFile(vPlayList[0]->getPlaylist(), list);
    pInput = UART_Keyboard();

    if(pInput->source == SOURCE_KEYBROAD)
    {
        if(pInput->keyboardData.keyboardType == NUMBER_TYPE)
        {
            switch (pInput->keyboardData.valueNumber)
            {
                case SHOW_METADATA:
                    viewMetadata(file_path,file_name,file_type);
                    cout << "Enter to back: " << endl;
                    cin.ignore();
                    break;
                case UPDATE_METADATA:
                    updateMetadata(file_path,file_name,file_type);
                    break;
                case BACK_MENU:
                    flowID.pop();
                    break; 
                default:       
                    cout << "Invalid choice. Please enter a valid option." << endl;
                    break;
            }
        }
        else
        {
            /* Do something */
        }
    }
    else
    {
        switch(pInput->uartData.uartType)
        {
        case BUTTON1_BYTE:
            switch (pInput->uartData.valueNumber)
            {
            case SHOW_METADATA:
                viewMetadata(file_path,file_name,file_type);
                cout << "Enter to back: " << endl;
                // cin.ignore();
                UART_Keyboard();
                break;
            case UPDATE_METADATA:
                updateMetadata(file_path,file_name,file_type);
                break;
            case BACK_MENU:
                flowID.pop();
                break; 
            default:
                break;
            }
            break;
        case ADC_BYTE:
            myPlayer.setVolume(pInput->uartData.valueNumber * 2);
            break;
        default:
            break;
        }
    }
}

/************************************* Metadata view in Media List ******************************************* */
void Browser::viewMetadata(const string& file_path,const string& file_name,const int& file_type)
{
    system("clear");
    metaData.set_FilePath(file_path);
    TagLib::FileRef fileRef=metaData.getfileRef();
    string header = "Displaying Metadata...";
    cout << string(tableWidth , '=')<<endl;
    cout << string(tableWidth / 2-header.length()/2-file_name.length()/2, ' ') << header <<file_name<<endl;
    cout << string(tableWidth, '=')<<endl;
    cout << endl;
    if (!fileRef.isNull() && fileRef.tag())
    {
        TagLib::Tag *tag = fileRef.tag();
        switch (file_type)
        {
        case AUDIO_FILE_TYPE:
            metadataView.displayAudioFileMetadata(tag, fileRef);
            break;
        case VIDEO_FILE_TYPE:
            metadataView.displayVideoFileMetadata(tag, fileRef, file_path);
            break;
        default:
            metadataView.getMediaFileTypeError();
            break;
        }
    }
    else
    {
        metadataView.getMetadataError();
    }
}

void Browser::updateMetadata(string& file_path,string& file_name,int& file_type)
{
    metaData.set_FilePath(file_path);
    TagLib::FileRef fileRef = metaData.getfileRef();
    TagLib::Tag *tag = fileRef.tag();
    string new_value;
    bool flag = true;
    string header = "Updating Metadata...";
    int update_opt;
    while(flag)
    {
        // Show data file mp3
        system("clear");
        cout << string(tableWidth , '=')<<endl;
        cout << string(tableWidth / 2-header.length()/2 -file_name.length()/2, ' ') << header << file_name<<endl;
        cout << string(tableWidth, '=')<<endl;
        cout << endl;
        metadataView.displayAudioFileMetadata(tag, fileRef);
        cout<<endl;
        cout << left << setw(30) << "[ 0 ]. Back" << endl;
        cout<<endl;
        cout << string(tableWidth, '=')<<endl;
        metadataView.chooseMetadataField();
        update_opt = userInput();
        // ======================================================
        if(update_opt == 0)
        {
            return;
        }
        else if(update_opt<0 || update_opt >6)
        {
            menuView.InvalidChoiceInterface();
        }
        else
        {
            update_opt = userInput();
            metadataView.enterMetadataValue();
            if (file_type == AUDIO_FILE_TYPE)
            {
                switch (update_opt)
                {
                case MODIFY_NAME:
                    tag->setTitle(new_value.c_str());
                    flag =false;
                    break;
                case MODIFY_ALBUM:
                    tag->setAlbum(new_value.c_str());
                    flag =false;
                    break;
                case MODIFY_ARTIST:
                    tag->setArtist(new_value.c_str());
                    flag =false;
                    break;
                case MODIFY_YEAR:
                    tag->setYear(stoi(new_value));
                    flag =false;
                    break;
                case MODIFY_GENRE:
                    tag->setGenre(new_value.c_str());
                    flag =false;
                    break;
                case MODIFY_AUDIO_DURATION:
                    return;
                default:
                    menuView.InvalidChoiceInterface();
                    flag =true;
                    return;
                }
            }
            else
            {
                switch (update_opt)
                {
                case MODIFY_VIDEO_NAME:
                    tag->setTitle(new_value.c_str());
                    flag =false;
                    break;
                case MODIFY_SIZE:
                    metadataView.modifyMetadataError();
                    flag =false;
                    return;
                case MODIFY_BIT_RATE:
                    metadataView.modifyMetadataError();
                    flag =false;
                    return;
                case MODIFY_VIDEO_DURATION:
                    metadataView.modifyMetadataError();
                    flag =false;
                    return;
                default:
                    menuView.InvalidChoiceInterface();
                    flag =true;
                    return;
                }
            }
            if (fileRef.save() == true)
            {
                metadataView.modifyMetadataSuccess();
                return;
            }
        }
    }
}
