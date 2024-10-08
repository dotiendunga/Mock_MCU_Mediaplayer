#include "PlaylistView.hpp"


/*========================================================================================================================================================*/
//                                                                       SHOW NUMBERS PLAYLIST                                                            //
/*========================================================================================================================================================*/

void PlaylistView::display_Playlist(const vector<Playlist*>& plists, size_t &currentPage){
    system("clear");
    string header = "Play List";
    cout<< string(tableWidth , '=')<<endl;
    cout<< string(tableWidth / 2-header.length()/2, ' ') << header <<endl;
    cout<< string(tableWidth, '=')<<endl;
    cout<<"|"<< left << setw(10) << " No."
        <<"|"<< left << setw(tableWidth/3) << "List Name"
        <<"|"<< left << setw(tableWidth/3) << "Number"<<endl;
    cout<< string(tableWidth ,'-')<<"\n"<<endl;

    display_PlaylistPerPage(plists,currentPage);

    cout<<"\n"<<string(tableWidth, '-')<<endl;
    cout << "Total Media list: " << plists.size() << endl;
    cout << "Page: " << currentPage<<endl;
    cout <<string(tableWidth/4, ' ')
         <<left << setw(25) << "C. Create"
         << left << setw(25) << "D. Delete"
         << left << setw(25) << "R. Rename"
         << left << setw(25) << "E. Exit"<< endl;
    cout<< string(tableWidth, '=')<<endl;
}

void PlaylistView::display_PlaylistPerPage(const vector<Playlist*>& plists, size_t &currentPage) {
    int startIndex = (int(currentPage) - 1) * PAGE_LIST_SIZE;
    int endIndex = min(startIndex + PAGE_LIST_SIZE, static_cast<int>(plists.size()));
    // static_cast<int> chuyen doi sang so int
    for (int i = startIndex; i < endIndex; ++i) {
        cout<<"|"<< left << setw(10) << i + 1
            <<"|"<< left << left_align(truncate_utf8(plists[i]->getName(), 35),40)
            <<"|"<< left <<plists[i]->getSize()<<" Songs"<< endl;
    }
}

// ================================================CHECK CHOOSE PLAYLIST TO SHOW ============================================================

int PlaylistView::check_choice_PlaylistView(const vector<Playlist*>& lists, size_t& currentPage) {
    string userInput;
    bool flag = true;
    while(flag)
    {   
        display_Playlist(lists,currentPage);
        cout << "Choose option to playlist: ";
        getline(cin, userInput);
        // userInput=userInputString();
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
                if(userInput.length()<=1)
                {
                    char command = userInput[0];
                    switch (command)
                    {
                        case 'N':
                        case 'n':
                            if (currentPage < (lists.size() + PAGE_LIST_SIZE - 1) / PAGE_LIST_SIZE)
                            {
                                currentPage++;
                            }
                            break;
                        case 'P':
                        case 'p':
                            if (currentPage > 1)
                            {
                                currentPage--;
                            }
                            break;

                        /* Create list */
                        case 'C':
                        case 'c':
                            return -1;
                            break;

                        /* Delete list */
                        case 'D':
                        case 'd':
                            return -2;
                            break;

                        /* Rename list*/
                        case 'R':
                        case 'r':
                            return -3;
                            break;
                        /* Exit page */
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
        }}else{

            cout << "Invalid choice. Please enter a valid option." << endl;
            cin.ignore();
        }   
    }
    /* Return Menu */
    return 0;
}

/*======================================================================================================================================*/


/*======================================================================================================================================*/
//                                                          SHOW SONGS IN PLAYLISTNAME                                                  //
/*======================================================================================================================================*/


void PlaylistView:: playlistName(const vector<MediaFile*>&plist_name, size_t &currentPage)
{
    system("clear");
    string header = "Play List Name";
    cout<< string(tableWidth , '=')<<endl;
    cout<< string(tableWidth / 2-header.length()/2, ' ') << header <<endl;
    cout<< string(tableWidth, '=')<<endl;
    cout <<"|"<< left << setw(10) << " No."
         <<"|"<< left << setw(tableWidth/3) << "Name"
         <<"|"<< left << setw(tableWidth/4) << "Artist"
         <<"|"<< left << setw(tableWidth/8) << "Duration (s)"
         <<"|"<< left << setw(tableWidth/8) << "Publisher"<<endl;
    cout<< string(tableWidth ,'-')<<"\n"<<endl;
    display_PlaylistNamePerPage(plist_name,currentPage);

    cout<< string(tableWidth , '=')<<endl;
    cout << "Total Media list: " << plist_name.size() << "\n" << endl;
    cout << "Page: " << currentPage<<endl;
}

void PlaylistView::display_PlaylistNamePerPage(const vector<MediaFile*>&plist_name, size_t &currentPage) {
    size_t startIndex = (currentPage - 1) * PAGE_SONG_SIZE;
    size_t endIndex = min(startIndex + PAGE_SONG_SIZE, plist_name.size());
    for (size_t i = startIndex; i < endIndex; ++i) {
        string file_name = plist_name[i]->getName();
        string file_path = plist_name[i]->getPath();
        // size_t file_type = plist_name[i]->getType();
        TagLib::FileRef fileRef(file_path.c_str());
        if (!fileRef.isNull() && fileRef.tag()){
            TagLib::Tag *tag = fileRef.tag();
            cout <<"|"<< left << setw(10) << i + 1
                <<"|"<< left << left_align(truncate_utf8(tag->title().toCString(true), 35),tableWidth/3)
                <<"|"<< left << left_align(truncate_utf8(tag->artist().toCString(true), 30),tableWidth/4)
                <<"|"<< left << setw(tableWidth/8) << secondsToTimeFormat(fileRef.audioProperties()->lengthInSeconds())
                <<"|"<< left << setw(tableWidth/8) << tag->year()<<endl;
        }
    }
    for (int i = endIndex - startIndex; i < PAGE_SONG_SIZE; ++i) {
                cout << "|" << setw(10) << " "
                     << "|" << setw(tableWidth / 3) << " "
                     << "|" << setw(tableWidth / 4) << " "
                     << "|" << setw(tableWidth / 8) << " "
                     << "|" << setw(tableWidth / 6) << " "<< endl;
}}


/*==================================================================== SHOW  PLAYLIST NAME  ================================================================*/

void PlaylistView::display_PlaylistName(const vector<MediaFile*>&plist_name, size_t &currentPage) {
    playlistName(plist_name,currentPage);
    cout <<string(tableWidth/4, ' ')
         <<left << setw(25) << "P. Previous"
         <<left << setw(25) << "N. Next"
         <<left << setw(25) << "E. Exit"<< endl;
    cout<<endl;
    cout <<string(tableWidth/4, ' ')
         <<left << setw(25) << "R. Remove"
         <<left << setw(25) << "A. Add" << endl;
    cout<<endl;
    cout<< string(tableWidth , '=')<<endl;
}

/*=========================================================== CHECK CHOOSE FOR PLAYLIST  ===================================================================*/

int PlaylistView::check_choice_PlaylistName(const vector<MediaFile*>& lists_name, size_t& currentPage) {
    string userInput;
    bool flag = true;
    
    while(flag)
    {   
        display_PlaylistName(lists_name,currentPage);
        cout << "Choose option to Modifle: ";
        getline(cin, userInput);
        // userInput=userInputString();
        if (!userInput.empty()) {
            stringstream ss(userInput);
            size_t MusicChoice;
            if (ss >> MusicChoice)
            {
                if (MusicChoice > 0 && MusicChoice <= lists_name.size())
                {
                    return MusicChoice;
                }
                else{
                    cout << "Invalid choice. Please enter a valid option." << endl;
                    cin.ignore();
                }
            }
            else
            {
                if(userInput.length()<=1)
                {
                    char command = userInput[0];
                    switch (command)
                    {
                        /* Next page */
                        case 'N':
                        case 'n':
                            if (currentPage < (lists_name.size() + PAGE_SONG_SIZE - 1) / PAGE_SONG_SIZE)
                            {
                                currentPage++;
                            }
                            break;

                        /* Previous page */
                        case 'P':
                        case 'p':
                            if (currentPage > 1)
                            {
                                currentPage--;
                            }
                            break;

                        /* Add Music */
                        case 'A':
                        case 'a':
                            return ADD_MUSIC;
                        
                        /* Remove Music */
                        case 'R':
                        case 'r':
                            return REMOVE_MUSIC;
                        
                        /* Exit */
                        case 'E':
                        case 'e':
                            flag = false;
                            return EXIT_MUSIC;
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
    /* Exit */
    return EXIT_MUSIC;
}

/*========================================================================================================================================================*/
//                                                       SHOW DISPLAY REMOVE SONG IN PLAYLIST 
/*========================================================================================================================================================*/

void PlaylistView::display_PlayNameRemove(const vector<MediaFile*>& plist_name, size_t& currentPage)
{
    playlistName(plist_name,currentPage);
    cout <<string(tableWidth/4, ' ')
         <<left << setw(25) << "P. Previous"
         <<left << setw(25) << "N. Next"
         <<left << setw(25) << "E. Exit"<< endl;
    cout<< string(tableWidth , '=')<<endl;
}

/*============================================================ CHECK CHOOSE FOR PLAYLIST TO REMOVE =======================================================*/
int PlaylistView::check_choice_PlaylistName_REMOVE(const vector<MediaFile*>& lists_name, size_t& currentPage){
    string userInput;
    bool flag = true;
    while(flag)
    {   
        display_PlayNameRemove(lists_name, currentPage);
        cout<<"Choose media to Remove: ";
        getline(cin, userInput);
        // userInput=userInputString();
        if (!userInput.empty()) {
            stringstream ss(userInput);
            size_t MusicChoice;
            if (ss >> MusicChoice)
            {
                if (MusicChoice > 0 && MusicChoice <= lists_name.size())
                {
                    return MusicChoice;
                }
            }
            else
            {
                if(userInput.length()<=1)
                {
                    char command = userInput[0];
                    switch (command)
                    {
                        /* Next page */
                        case 'N':
                        case 'n':
                            if (currentPage < (lists_name.size() + PAGE_SONG_SIZE - 1) / PAGE_SONG_SIZE)
                            {
                                currentPage++;
                            }

                            break;

                        /* Previous page */
                        case 'P':
                        case 'p':
                            if (currentPage > 1)
                            {
                                currentPage--;
                            }
                            break;
                        /* Exit */
                        case 'E':
                        case 'e':
                            flag = false;
                            return EXIT_MUSIC;
                        default:
                            cout << "Invalid choice. Please enter a valid option." << endl;
                            cin.ignore();
                    }
                }else{
                        cout << "Invalid choice. Please enter a valid option." << endl;
                    }
        }}else{
            /**/
        }
    }
    return 0;
}
/*========================================================================================================================================================*/



/*========================================================================================================================================================*/
//                                                                SHOW DISPLAY ADĐ SONG TO PLAYLIST 
/*========================================================================================================================================================*/


/*================================================================ CHECK CHOOSE FOR PLAYLIST TO ADD  =====================================================*/
bool PlaylistView::check_choice_PlaylistName_ADD(const vector<MediaFile*>& lists_name_add,const vector<MediaFile*>& lists_name_compare,int Choice_index)
{
    std::string path_compare = lists_name_compare[Choice_index-1]->getPath();
    for(int i=0;i<(int)lists_name_add.size();i++)
    {
        if(path_compare == lists_name_add[i]->getPath())
        {
            return false;
        }
    }
    return true;
    // return true;
}
/*========================================================================================================================================================*/