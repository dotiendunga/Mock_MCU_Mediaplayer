#include "MediaPlayerView.hpp"

//========================================================================================================================================================//
//                                                                  SHOW PLAYLIST IN  PLAY MUSIC
//========================================================================================================================================================//

void PlayMusicView::VPlayerMusic_DisplayList(const vector<Playlist*>& plists, size_t& currentpage)
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
    cout << "Choose option to playlist: "<<endl;
}

// ==================================================== CHECK USING FOR SHOW PLAYLIST IN PLAY MUSIC ======================================================//

// int PlayMusicView::VPlayerMusic_InputList(const vector<Playlist*>& lists, size_t& currentPage) {
    // string userInput;
    // bool flag = true;
    // while(flag)
    // {    
    //     VPlayerMusic_DisplayList(lists,currentPage);
    //     cout << "Choose option to playlist: "; 
    //     getline(cin,userInput);
    //     // getline(cin,userInput);
    //     if (!userInput.empty()) {
    //         stringstream ss(userInput);
    //         size_t ListChoice;
    //         if (ss >> ListChoice)
    //         {
    //             if (ListChoice > 0 && ListChoice <= lists.size())
    //             {
    //                 return ListChoice;
    //             }else{
    //                 cout << "Invalid choice. Please enter a valid option." << endl;
    //                 // cin.ignore();
    //             }
    //         }
    //         else
    //         {
    //             if(userInput.length()<=1){
    //                 char command = userInput[0];
    //                 switch (command)
    //                 {   
    //                     /*NEXT PAGE*/
    //                     case 'N':
    //                     case 'n':
    //                         if (currentPage < (lists.size() + PAGE_LIST_SIZE - 1) / PAGE_LIST_SIZE)
    //                         {
    //                             currentPage++;
    //                         }

    //                             break;
    //                         /*PREVIOUS PAGE*/
    //                         case 'P':
    //                         case 'p':
    //                             if (currentPage > 1)
    //                             {
    //                                 currentPage--;
    //                             }
            
    //                             break;
    //                         /*EXIT PAGE*/
    //                         case 'E':
    //                         case 'e':
    //                             flag = false;
    //                             break;
    //                         default:
    //                             cout << "Invalid choice. Please enter a valid option." << endl;
    //                             // cin.ignore();
    //                     }
    //             }else{
    //                         cout << "Invalid choice. Please enter a valid option." << endl;
    //                     }
    //         }
    //         }else{
    //             cout << "Invalid choice. Please enter a valid option." << endl;
    //             // cin.ignore();
    //         }
    //     }
    //     /* RETURN MENU */
    //     return 0;
    // }
        // display_PlayMucsic(lists,currentPage);
        
/*========================================================================================================================================================*/


/*========================================================================================================================================================*/
//                                                          SHOW SONG TO PLAY IN PLAYLIST  <PLAY MUSIC>
/*========================================================================================================================================================*/

void PlayMusicView::VPlayerMusic_DisplayMusic(const vector<MediaFile*>& lists_name, size_t &currentpage, size_t timelapse, size_t duration, MediaPlayer& myPlayer)
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
    cout<< string(tableWidth / 2-Play_header.length()/2, ' ') << Play_header <<std::flush<<endl;;
    cout<<std::flush<<endl;;
    if(duration > 0)
    {
        string Playing_name = "Playing: "+ myPlayer.getPlayingMusicName();
        cout<< string(tableWidth / 2-Playing_name.length()/2, ' ') << Playing_name << std::flush<<endl;;
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
    }
    else
    {
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

void PlayMusicView::Update_Time_Volume(size_t timelapse, size_t duration, MediaPlayer& myPlayer) const
{
    // ======================================== UPDATE TIMESLIDE MUSIC ========================================= // 
    string Play_header = ".......................................................................";
    cout << "\033[" << PAGE_SONG_SIZE + 7 << ";0H"; // Di chuyển đến dòng cụ thể, cột 0
    cout << "\033[K" << "\r" << string(tableWidth / 2 - Play_header.length() / 2, ' ') << Play_header << std::flush << endl;
    cout << "\033[K" << std::flush << endl;

    if (duration > 0)
    {
        string Playing_name = "Playing: " + myPlayer.getPlayingMusicName();
        cout << "\033[K" << "\r" << string(tableWidth / 2 - Playing_name.length() / 2, ' ') << Playing_name << std::flush << endl;
        cout << "\033[K" << std::flush << endl;

        size_t progressLong = timelapse * 50 / duration;

        // Di chuyển đến dòng hiển thị Time và xóa nó
        cout << "\033[" << PAGE_SONG_SIZE + 11 << ";0H"; // Di chuyển đến dòng cụ thể
        cout << "\033[K" << std::flush; // Xóa dòng

        // Hiển thị Time mới
        cout << string(tableWidth / 6, ' ') << "Time: "
            << left << setw(8) << " " << "<" << string(progressLong, '#') << string((50 - progressLong), '=') << ">"
            << format_time(timelapse) << "/" << format_time(duration) << "\n" << std::flush << endl;
    }
    else
    {
        string Playing_name = "Playing: UNKNOWN";
        cout << "\033[K" << string(tableWidth / 2 - Playing_name.length() / 2, ' ') << Playing_name << std::flush << endl;
        cout << "\033[K" << std::flush << endl;

        // Di chuyển đến dòng hiển thị Time và xóa nó
        cout << "\033[" << PAGE_SONG_SIZE + 11 << ";0H"; // Di chuyển đến dòng cụ thể
        cout << "\033[K" << std::flush; // Xóa dòng

        // Hiển thị Time mới
        cout << string(tableWidth / 6, ' ') << "Time: "
            << left << setw(10) << " " << "<" << string(50, '=') << ">"
            << format_time(0) << "/" << format_time(0) << "\n" << std::flush << endl;
    }

    // Di chuyển đến dòng hiển thị Volume và xóa nó
    cout << "\033[" << PAGE_SONG_SIZE + 13 << ";0H"; // Di chuyển đến dòng cụ thể
    cout << "\033[K" << std::flush; // Xóa dòng

    // Hiển thị Volume mới
    cout << string(tableWidth / 6, ' ') << "Volume: "
        << left << setw(6) << " " << "<  " << (myPlayer.getVolume() * 100) / 128 << "%  >" << "\n" << std::flush << endl;
    cout << "\033[" << PAGE_SONG_SIZE + 26 << ";0H"; // Di chuyển đến dòng cụ thể
    cout << "\033[K" <<std::flush; // Xóa dòng


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
/*========================================================================================================================================================*/