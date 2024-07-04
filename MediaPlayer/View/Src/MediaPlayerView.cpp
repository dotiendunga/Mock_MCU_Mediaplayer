#include "MediaPlayerView.hpp"

//========================================================================================================================================================//
//                                                                  SHOW PLAYLIST IN  PLAY MUSIC
//========================================================================================================================================================//

void PlayMusicView::display_PlayMucsic(const vector<Playlist*>& plists, size_t& currentpage)
{
    system("clear");
    string header = "Play Music";
    cout<< string(tableWidth , '=')<<std::flush<<endl;;
    cout<< string(tableWidth / 2-header.length()/2, ' ') << header <<std::flush<<endl;;
    cout<< string(tableWidth, '=')<<std::flush<<endl;;
    cout<<"|"<< left << setw(10) << " No."
        <<"|"<< left << setw(tableWidth/3) << "List Name"
        <<"|"<< left << setw(tableWidth/3) << "Number"<<std::flush<<endl;;
    cout<< string(tableWidth ,'-')<<"\n"<<std::flush<<endl;;

    display_PlaylistPerPage(plists,currentpage);

    cout<<"\n"<<string(tableWidth, '-')<<std::flush<<endl;;
    cout << "Total Media list: " << plists.size() << std::flush<<endl;;
    cout << "Page: " << currentpage<<std::flush<<endl;;
    cout <<string(tableWidth/4, ' ')
         << left << setw(25) << "P. Previous"
         << left << setw(25) << "N. Next"
         << left << setw(25) << "E. Exit"<< std::flush<<endl;;
    cout<<std::flush<<endl;;
    cout<< string(tableWidth, '=')<<std::flush<<endl;;
}

// ==================================================== CHECK USING FOR SHOW PLAYLIST IN PLAY MUSIC ======================================================//

int PlayMusicView::check_choice_PlayMusicView(const vector<Playlist*>& lists, size_t& currentPage) {
    string userInput;
    bool flag = true;
    while(flag)
    {    
        display_PlayMucsic(lists,currentPage);
        cout << "Choose option to playlist: "; 
        getline(cin,userInput);
        if (!userInput.empty()) {
            stringstream ss(userInput);
            size_t ListChoice;
            if (ss >> ListChoice)
            {
                if (ListChoice > 0 && ListChoice <= lists.size())
                {
                    return ListChoice;
                }else{
                    cout << "Invalid choice. Please enter a valid option." << endl;
                    cin.ignore();
                }
            }
            else
            {
                if(userInput.length()<=1){
                    char command = userInput[0];
                    switch (command)
                    {   
                        /*NEXT PAGE*/
                        case 'N':
                        case 'n':
                            if (currentPage < (lists.size() + PAGE_LIST_SIZE - 1) / PAGE_LIST_SIZE)
                            {
                                currentPage++;
                            }

                            break;
                        /*PREVIOUS PAGE*/
                        case 'P':
                        case 'p':
                            if (currentPage > 1)
                            {
                                currentPage--;
                            }
        
                            break;
                        /*EXIT PAGE*/
                        case 'E':
                        case 'e':
                            flag = false;
                            break;
                        default:
                            cout << "Invalid choice. Please enter a valid option." << endl;
                            cin.ignore();
                    }
            }else{
                        cout << "Invalid choice. Please enter a valid option." << endl;
                    }
        }
        }else{
            cout << "Invalid choice. Please enter a valid option." << endl;
            cin.ignore();
        }
    }
    /* RETURN MENU */
    return 0;
}

/*========================================================================================================================================================*/


/*========================================================================================================================================================*/
//                                                          SHOW SONG TO PLAY IN PLAYLIST  <PLAY MUSIC>
/*========================================================================================================================================================*/

void PlayMusicView::display_ShowPlay(const vector<MediaFile*>& lists_name, size_t &currentpage, size_t timelapse, size_t duration, MediaPlayer& myPlayer)
{
    system("clear");
    string header = "Play Music";
    cout<< string(tableWidth , '=')<<endl;
    cout<< string(tableWidth / 2-header.length()/2, ' ') << header <<endl;
    cout<< string(tableWidth, '=')<<endl;
    cout <<"|"<< left << setw(10) << " No."
         <<"|"<< left << setw(tableWidth/3) << "Name"
         <<"|"<< left << setw(tableWidth/4) << "Artist"
         <<"|"<< left << setw(tableWidth/8) << "Duration (s)"
         <<"|"<< left << setw(tableWidth/8) << "Publisher"<<endl;
    cout << string(tableWidth ,'-')<<"\n"<<endl;
    
    display_PlaylistNamePerPage(lists_name,currentpage);

    // ======================================== UPDATE TIMESLIDE MUSIC ========================================= // 
    string Play_header =".......................................................................";
    // std::cout << "\r" << message << " (" << elapsed << "s)" << std::flush;
    cout<< "\r" << string(tableWidth / 2-Play_header.length()/2, ' ') << Play_header <<std::flush<<endl;;
    cout<<std::flush<<endl;;
    if(duration > 0)
    {
        string Playing_name = "Playing: "+ myPlayer.getPlayingMusicName();
        cout<< "\r" << string(tableWidth / 2-Playing_name.length()/2, ' ') << Playing_name << std::flush<<endl;;
        cout<<std::flush<<endl;;
    }
    else
    {
        string Playing_name = "Playing: UNKNOWN";
        cout<< string(tableWidth / 2-Playing_name.length()/2, ' ') << Playing_name <<std::flush<<endl;;
        cout<<std::flush<<endl;;
    }
    Time_Volume(timelapse, duration, myPlayer.getVolume());

    // ============================================================================================================
    if(myPlayer.getFlagAuto()==true )
    {
        cout <<left <<setw(20)<<" "
         << left << setw(25) << "<->. Previous song"
         << left << setw(25) << "<R>. Pause/ Resume"
         << left << setw(18) << "<A>. Repeat"
         << left << setw(25) << "<+>. Next song" <<std::flush<<endl;;
    }else{
        cout <<left << setw(20)<<" "
            << left << setw(25) << "<->. Previous song"
            << left << setw(25) << "<R>. Pause/ Resume"
            << left << setw(18) << "<A>. Auto"
            << left << setw(25) << "<+>. Next song" <<std::flush<<endl;;
    }
    cout<< string(tableWidth , '=')<<std::flush<<endl;;
    cout << "Total Media list: " << lists_name.size() << "\n" << std::flush<<endl;;
    cout << "Page: " << currentpage<<std::flush<<endl;;
    cout <<string(tableWidth/4, ' ')
         << left << setw(25) << "P. Previous"
         << left << setw(25) << "N. Next"
         << left << setw(25) << "E. Exit"<< std::flush<<endl;;
    cout<<std::flush<<endl;;
    cout <<string(tableWidth/4, ' ')
         << left << setw(25) << "U. Up Volume"
         << left << setw(25) << "D. Down Volume"<< std::flush<<endl;;
    cout<<std::flush<<endl;;
    cout<< string(tableWidth , '=')<<std::flush<<endl;;
    cout<< "Choose option to play: " << std::flush<<endl;;
}

// ================================================== SHOW CHANGE TIME  - VOLUME - NEXT SONG  - PREVIOUS SONG =========================================== //

void PlayMusicView::Time_Volume(size_t timelapse, size_t duration, const size_t volume) const
{
    if(duration > 0)
    {
        size_t progressLong = timelapse * 50 / duration;
        // Show time sẽ thay giá trị vào
        cout <<string(tableWidth/6,' ')<< "Time: "
            << left <<setw(8) <<" "<<"<" << string(progressLong, '#')  << string((50-progressLong), '=')  << ">"
            << format_time(timelapse) << "/" << format_time(duration) <<"\n"<<std::flush<<endl;;
    }
    else
    {
        cout <<string(tableWidth/6,' ')<< "Time: "
            << left <<setw(10) <<" "<<"<" << string(50, '=')  << ">"
            << format_time(0) << "/" << format_time(0) <<"\n"<<std::flush<<endl;;
    }
    cout <<string(tableWidth/6,' ')<<"Volume: "
         << left <<setw(6) <<" "<<"<  "<< (volume*100)/128 << "%  >"<<"\n"<<std::flush<<endl;;
}

void PlayMusicView::Update_Time_Volume(size_t timelapse, size_t duration, const size_t volume,MediaPlayer& myPlayer) const
{

// ======================================== UPDATE TIMESLIDE MUSIC ========================================= // 
    string Play_header =".......................................................................";
    // std::cout << "\r" << message << " (" << elapsed << "s)" << std::flush;
    // Xóa dòng hiện tại
    cout << "\033[" << PAGE_SONG_SIZE+7<< ";0H"; // Di chuyển đến dòng cụ thể, cột 0
    cout << "\033[K"<< "\r" << string(tableWidth / 2-Play_header.length()/2, ' ') << Play_header <<std::flush<<endl;;
    // Xóa dòng hiện tại
    cout << "\033[K"<<std::flush<<endl;;
    if(duration > 0)
    {
        string Playing_name = "Playing: "+ myPlayer.getPlayingMusicName();
        // Xóa dòng hiện tại
        cout << "\033[K"<< "\r" << string(tableWidth / 2-Playing_name.length()/2, ' ') << Playing_name << std::flush<<endl;;
        // Xóa dòng hiện tại
        cout << "\033[K"<<std::flush<<endl;;
        size_t progressLong = timelapse * 50 / duration;
        // Show time sẽ thay giá trị vào
        // Xóa dòng hiện tại
        cout << "\033[K"<<string(tableWidth/6,' ')<< "Time: "
            << left <<setw(8) <<" "<<"<" << string(progressLong, '#')  << string((50-progressLong), '=')  << ">"
            << format_time(timelapse) << "/" << format_time(duration) <<"\n"<<std::flush<<endl;;
    }
    else
    {
        string Playing_name = "Playing: UNKNOWN";
        // Xóa dòng hiện tại
    cout << "\033[K"<< string(tableWidth / 2-Playing_name.length()/2, ' ') << Playing_name <<std::flush<<endl;;
        // Xóa dòng hiện tại
    cout << "\033[K"<<std::flush<<endl;;
        // Xóa dòng hiện tại
    cout << "\033[K"<<string(tableWidth/6,' ')<< "Time: "
            << left <<setw(10) <<" "<<"<" << string(50, '=')  << ">"
            << format_time(0) << "/" << format_time(0) <<"\n"<<std::flush<<endl;;
    }
    // Xóa dòng hiện tại
    cout << "\033[K"<<string(tableWidth/6,' ')<<"Volume: "
         << left <<setw(6) <<" "<<"<  "<< (volume*100)/128 << "%  >"<<"\n"<<std::flush<<endl;;
    cout << "\033[" << PAGE_SONG_SIZE+25<< ";0H"<<std::flush<<endl;; // Di chuyển đến dòng cụ thể, cột 0
}

string PlayMusicView::format_time(size_t total_seconds) const
{
    size_t hours = total_seconds / 3600;
    size_t minutes = (total_seconds % 3600) / 60;
    size_t seconds = total_seconds % 60;
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours << ":"
       << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << seconds;
    return ss.str();
}


// ================================================== CHECK USING FOR SHOW SONG TO PLAY IN PLAYLIST < PLAY MUSIC >=========================================//

// int PlayMusicView::check_choice_PlayMusicView_ShowPlay(const vector<MediaFile*>& lists_name, size_t& currentPage) {
//     string userInput;
//     bool flag = true;
//     while(flag)
//     {
//         // getline(cin, userInput);
//         userInput=userInputString();
//         if (!userInput.empty()) {
//             stringstream ss(userInput);
//             size_t MusicChoice;
//             if (ss >> MusicChoice)
//             {
//                 if (MusicChoice > 0 && MusicChoice <= lists_name.size())
//                 {
//                     return MusicChoice;
//                 }else{
//                     cout << "Invalid choice. Please enter a valid option."<<endl ;                
//                     return -7;
//                 }
//             }
//             else
//             {
//                 if(userInput.length()<=1)
//                 {
//                     char command = userInput[0];
//                     cout<< command<<endl;
//                     switch (command)
//                     {
//                         case 'N':
//                         case 'n':
//                             if (currentPage < (lists_name.size() + PAGE_SONG_SIZE - 1) / PAGE_SONG_SIZE)
//                             {
//                                 currentPage++;
//                             }
//                             return -7;
//                         case 'P':
//                         case 'p':
//                             if (currentPage > 1)
//                             {
//                                 currentPage--;
//                             }
//                             return -7;
//                         case 'U':
//                         case 'u':
//                             return -1;
//                         case 'D':
//                         case 'd':
//                             return -2;
//                         case 'R':
//                         case 'r':
//                             return -3;
//                         case '+':
//                             return -4;
//                         case '-':
//                             return -5;
//                         case 'a':
//                         case 'A':
//                             return -6;
//                         case 'E':
//                         case 'e':
//                             flag = false;
//                             return 0;
//                             // break;
//                         default:
//                             cout << "Invalid choice. Please enter a valid option." << endl;
//                             return -7;
//                     }
//             }else{
//                         cout << "Invalid choice. Please enter a valid option." << endl;
//                         return -7;
//                     }
//             } 
//         }else{
//             // system("clear");
//             // display_ShowPlay(lists_name,currentPage);
//             cout << "Invalid choice. Please enter a valid option." << endl;
//             return -7;
//         }
//     }
//     return 0;
// }

enum 
{
    END_BYTE = 0x00,
    BTN1    = 0x01,
    BTN2   = 0x02,
    KEYBROAD = 0x03
};
// enum{
    
//     SOURCE_UART = 0,
//     SOURCE_KEYBROAD = 1
// }; 

int PlayMusicView::check_choice_PlayMusicView_ShowPlay(const vector<MediaFile*>& lists_name, size_t& currentPage) {
    string userInput;
    string data;
    bool flag = true;
    while(flag)
    {
        int source = check_source();
        if(source==SOURCE_UART)
        {
            userInput=userInputString();
            // BUTTON CHECK SELECT DATA
            if (userInput[0] == BTN1)
            {
                size_t MusicChoice = (size_t)userInput[1];
    
                    if (MusicChoice > 0 && MusicChoice <= lists_name.size())
                    {
                        return MusicChoice;
                    }else{
                        cout << "Invalid choice. Please enter a valid option." << endl;
                        // return -7;
                    }
            }else if (userInput[0] == BTN2)
            {
                stringstream ss(userInput);
                size_t handleChoice;
                if (ss >> handleChoice)
                {
                    switch (handleChoice)
                    {
                        case 1:
                            if (currentPage < (lists_name.size() + PAGE_SONG_SIZE - 1) / PAGE_SONG_SIZE)
                            {
                                currentPage++;
                            }
                            return -7;
                        case 2:
                            if (currentPage > 1)
                            {
                                currentPage--;
                            }
                            return -7;
                        case 3:
                            return -1;
                        case 4:
                            return -2;
                        case 5:
                            return -3;
                        case 6:
                            return -4;
                        case 7:
                            return -5;
                        case 8:
                            return -6;
                        case 9:
                            flag = false;
                            return 0;
                            // break;
                        default:
                            cout << "Invalid choice. Please enter a valid option." << endl;
                            // return -7;
                }
            }
        }
        }
        else if (source == SOURCE_KEYBROAD)
        {
            getline(cin, userInput);
            cout<<userInput<<endl;
            if (!userInput.empty()) {
                stringstream ss(userInput);
                size_t MusicChoice;
                if (ss >> MusicChoice)
                {
                    cout<<userInput<<endl;
                    if (MusicChoice > 0 && MusicChoice <= lists_name.size())
                    {
                        return MusicChoice;
                    }else{
                        cout << "Invalid choice. Please enter a valid option." << endl;
                        // return -7;
                    }
                }
                else
                {
                    if(userInput.length()<=1)
                    {
                        char command = userInput[0];
                        switch (command)
                        {
                            case 'N':
                            case 'n':
                                if (currentPage < (lists_name.size() + PAGE_SONG_SIZE - 1) / PAGE_SONG_SIZE)
                                {
                                    currentPage++;
                                }
                                return -7;
                            case 'P':
                            case 'p':
                                if (currentPage > 1)
                                {
                                    currentPage--;
                                }
                                return -7;
                            case 'U':
                            case 'u':
                                return -1;
                            case 'D':
                            case 'd':
                                return -2;
                            case 'R':
                            case 'r':
                                return -3;
                            case '+':
                                return -4;
                            case '-':
                                return -5;
                            case 'a':
                            case 'A':
                                return -6;
                            case 'E':
                            case 'e':
                                flag = false;
                                return 0;
                                // break;
                            default:
                                cout << "Invalid choice. Please enter a valid option." << endl;
                                // return -7;
                        }
                }else{
                            cout << "Invalid choice. Please enter a valid option." << endl;
                            // return -7;
                        }
                } 
            }else{
                // system("clear");
                // display_ShowPlay(lists_name,currentPage);
                cout << "Invalid choice. Please enter a valid option." << endl;
                // return -7;
            }
    }
    
}
return 0;
}


/*========================================================================================================================================================*/