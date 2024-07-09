#include "Browser.hpp"


Browser::Browser(/* args */)
{
    
}

Browser::~Browser()
{
    FreeAll();
}

UART_Keyboard_Input* Browser::UART_Keyboard()
{
    static UART_Keyboard_Input input;
    string userInput;
    bool flag = true;
    while(flag)
    {
        SourceInput_t source = myUART.check_source();
        /********************************************************************************** */
        if(source == SOURCE_UART)
        {
            uint8_t buffer[4];
            input.source = SOURCE_UART;
            myUART.userInputBuffer(buffer);
            switch (buffer[1])
            {
            case BUTTON1_BYTE:
                input.uartData.uartType = BUTTON1_BYTE;
                break;
            case BUTTON2_BYTE:
                input.uartData.uartType = BUTTON2_BYTE;
                break;
            case ADC_BYTE:
                input.uartData.uartType = ADC_BYTE;
                break;
            default:
                input.uartData.uartType = ERROR_BYTE;
                break;
            }
            input.uartData.valueNumber = buffer[2];
            return &input;
        }
        /********************************************************************************** */
        else if (source == SOURCE_KEYBROAD)
        {
            input.source = SOURCE_KEYBROAD;
            getline(cin, userInput);
            if (!userInput.empty())
            {
                stringstream ss(userInput);
                size_t MusicChoice;
                if (ss >> MusicChoice)
                {
                    input.keyboardData.keyboardType = NUMBER_TYPE;
                    input.keyboardData.valueNumber = MusicChoice;
                }
                else
                {
                    if(userInput.length()<=1)
                    {
                        input.keyboardData.keyboardType = STRING_TYPE;
                        input.keyboardData.valueString[0] = userInput[0];
                    }
                    else
                    {
                        cout << "Invalid choice. Please enter a valid option." << endl;
                    }
                }
                return &input;
            }
            else
            {
                return NULL;
            }
        }
    }
    return NULL;
}

void Browser::setPath()
{
    do
    {
        Path = mediaPathView.input_path();
        if(Path == "0")
        {
            flowID.pop();
            return;
        }
    }
    while(!(fs::exists(Path) && fs::is_directory(Path)));
    loadFile();
    flowID.pop();
    flowID.push(MENU_ID);
}
void Browser::setPathView()
{
    size_t current_screen = flowID.top();
    do
    {
        usbDeviceScanner.scanDevices();
        devices = usbDeviceScanner.getUSBMountPoints();
        mediaPathView.display_MediaPath(devices);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        current_screen = flowID.top();
    }while(current_screen  == PATH_USB_ID);
}
void Browser::PathUsbSelection()
{
    int option;
    std::thread PathThread(&Browser::setPathView, this);
    PathThread.detach();
    do
    {
        option = userInput();
        // cout<<option<<endl;
    }
    while(option > (int)devices.size() + 1 || option < 0);

    if(option == 0)
    {   /*CLOSE*/
        flowID.pop();
        flowID.push(CLOSE_PROGRAM);
    }
    else if(option == 1)
    {
        flowID.push(SET_PATH_ID);
    }
    else
    {
        Path = devices[option - 2];
        loadFile();
        flowID.push(MENU_ID);
    }
}
void Browser::FreeAll()
{
    if(vPlayList.size() != 0)
    {
        if(vPlayList[0]->getPlaylist().size() != 0)
        {
            for(MediaFile* mediafile : vPlayList[0]->getPlaylist())
            {
                delete mediafile;
            }
        }
        for(Playlist* playlist : vPlayList)
        {
            delete playlist;
        }
    }
    vPlayList.clear();
}

void Browser::loadFile()
{
    if(Path  != "0")
    {
        vPlayList.push_back(new Playlist("All"));
        for (const auto& entry : fs::directory_iterator(Path))
        {
            if (entry.is_regular_file() && entry.path().extension() == MP3_EXTENSION){
                vPlayList[0]->addFile(new MediaFile(entry.path().filename().string(), entry.path().string(), MP3_TYPE));
            }
            else if(entry.is_regular_file() && entry.path().extension() == MP4_EXTENSION){
                vPlayList[0]->addFile(new MediaFile(entry.path().filename().string(), entry.path().string(), MP4_TYPE));
            }
        }
    }
    else
    {
        return;
    }
}

int Browser::userInput()
{
    int source = myUART.check_source();
    if (source == SOURCE_UART) {
        uint8_t buffer[4];
        myUART.userInputBuffer(buffer);
        if (buffer[1] == BUTTON2_BYTE)
        {
            return buffer[2];
        }
        return -1;
    }else{
        int choice;
        std::cin >> choice;
        if (std::cin.fail())
        {
            std::cin.clear(); // clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
        return -1;
    }
}

string Browser::userInputString()
{
    string userInput;
    while(userInput.empty())
    {
        getline(cin, userInput);
    }
    return userInput;
}

/*=============================================== Option 2 in Menu =========================================================*/
void Browser::playlist(int& chosenList, int& chosenMusic)
{
    int UserOption = 0;
    playListView.display_Playlist(vPlayList, list);
    UserOption = playListView.check_choice_PlaylistView(vPlayList, list);
    switch (UserOption)
    {
    /* Exit */
    case 0:
        list = 1;
        flowID.pop();
        break;
    /* Create playlist */
    case -1:
        createList();
        break;

    /* Delete playlist */
    case -2:
        deleteList();
        break;

    /* Rename playlist */
    case -3:
        renameList();
        break;

    /* Choose list */
    default:
        chosenList = (size_t)UserOption;
        flowID.push(PLAY_LIST_MUSIC_ID);
        break;
    }
}
/*Create Playlist*/
void Browser::createList()
{
    cout << "Name: " ;
    string newPlaylistName = userInputString();
    vPlayList.push_back(new Playlist(newPlaylistName));
    /*IF vPLAYLIST EMPTY ADD SONG */
    cout<<"Playlist empty. Enter to add songs";
    cin.ignore();
    if(vPlayList[vPlayList.size()-1]->getPlaylist().size() == 0)
    {
        // flowID.push(7);
        // size_t currentPage=1;
        mediaFileView.display_MediaFile(vPlayList[0]->getPlaylist(), list);
        cout<<"Choose media to Add: ";
        int choose_add = mediaFileView.check_choice(vPlayList[0]->getPlaylist(), list);
        if(choose_add != -1){
            vPlayList[vPlayList.size()-1]->getPlaylist().push_back(vPlayList[0]->getPlaylist()[choose_add-1]);
        }
        return;
    }
}

/* Delete playlist */
void Browser::deleteList()
{
    cout << "Choose list number to delete: " ;
    int newPlaylistIndex = userInput() - 1;
    if (newPlaylistIndex < 0 || newPlaylistIndex >= (int)vPlayList.size()) {
        cout << "Invalid list number." << endl;
        return;
    }
    else{
    
        vPlayList[newPlaylistIndex]->getPlaylist().clear();
        vPlayList.erase(vPlayList.begin() + newPlaylistIndex);
    }
}

/* Rename playlist */
void Browser::renameList()
{
    cout << "Choose list number to rename: " ;
    int playlistIndex = userInput() - 1;
    cout << vPlayList.size();
    if (playlistIndex < 0 || playlistIndex >= (int)vPlayList.size()) {
        cout << "Invalid list number." << endl;
        cin.ignore();
        return;
    }
    else
    {
        cout << "New name: " ;
        string newPlaylistName = userInputString();
        vPlayList[playlistIndex]->setName(newPlaylistName);
    }
}
/* ================================= Interact with Playlist ====================================== */

void Browser::playlist_music(int& chosenList)
{
    int choose_remove;
    int choose_add;
    bool check_add;
    int UserOption = 0;
    // size_t currentPage =1;
    playListView.display_PlaylistName(vPlayList[chosenList - 1]->getPlaylist(), list);
    UserOption = playListView.check_choice_PlaylistName(vPlayList[chosenList - 1]->getPlaylist(), list);
    switch (UserOption)
    {
    /* Exit */
    case EXIT_MUSIC:
        list = 1;
        flowID.pop();
        break;
    /* Add Music */
    case ADD_MUSIC:
        mediaFileView.display_MediaFile(vPlayList[0]->getPlaylist(), list);
        cout<<"Choose media to Add: ";
        choose_add = mediaFileView.check_choice(vPlayList[0]->getPlaylist(), list);
        if(choose_add != -1)
        {
            check_add=playListView.check_choice_PlaylistName_ADD(vPlayList[chosenList - 1]->getPlaylist(),vPlayList[0]->getPlaylist(),choose_add);
            if(check_add){
                MediaFile*new_songs =vPlayList[0]->getPlaylist()[choose_add-1];
                vPlayList[chosenList - 1]->getPlaylist().push_back(new_songs);
                list =1;
                return;
            }
            list =1;
        }
        break;
    /* Remove Music */
    case REMOVE_MUSIC:
        playListView.display_PlayNameRemove(vPlayList[chosenList - 1]->getPlaylist(), list);
        choose_remove = playListView.check_choice_PlaylistName_REMOVE(vPlayList[chosenList - 1]->getPlaylist(), list);
        vPlayList[chosenList - 1]->getPlaylist().erase( vPlayList[chosenList - 1]->getPlaylist().begin()+choose_remove-1);
        break;
    /* SHOW METADATA SONGS IN PLAYLISTNAME  */
    default:
        file_path = vPlayList[chosenList-1]->getPlaylist()[UserOption-1]->getPath();
        file_name = vPlayList[chosenList-1]->getPlaylist()[UserOption-1]->getName();
        file_type = vPlayList[chosenList-1]->getPlaylist()[UserOption-1]->getType();
        flowID.push(METADATA_LIST_ID);
    }
}


/*========================================== Option 3 in Menu =========================================================*/
void Browser::playmusic(int& chosenList)
{
    mediaPlayerView.VPlayerMusic_DisplayList(vPlayList, list);
    chosenList = mediaPlayerView.VPlayerMusic_InputList(vPlayList, list);
    if(chosenList > 0)
    {
        myPlayer.setList(vPlayList[chosenList - 1]->getPlaylistPointer());
        flowID.push(PLAY_MUSIC_PLAYER_ID);
        startThread();
    }
    else
    {
        /* Exit */
        list = 1;
        flowID.pop();
    }
}

/*============================== Thread ===============================*/

void Browser::updatePlayerView()
{
    size_t current_screen;
    do
    {
        std::string musicPath;
        musicPath = myPlayer.getPlayingMusicPath();
        try
        {   
            std::lock_guard<std::mutex> lock1(mtx1);
            if(musicPath == "")
            {
                throw std::runtime_error("The music path is empty.");
            }
            fileRef = TagLib::FileRef(myPlayer.getPlayingMusicPath().c_str());
            duration = fileRef.audioProperties()->lengthInSeconds();
            if(myPlayer.isPlaying())
            {
                timelapse = std::chrono::steady_clock::now() - startTime;
                if(timelapse.count() >= duration)
                {
                    resetTimer();
                    myPlayer.autoMusic();
                }
            }
            else
            {
                startTime = std::chrono::steady_clock::now() - std::chrono::duration_cast<std::chrono::steady_clock::duration>(timelapse);
            }
            // mediaPlayerView.Time_Volume( list, timelapse.count(), duration, myPlayer);
            mediaPlayerView.Update_Time_Volume(timelapse.count(), duration, myPlayer.getVolume(),myPlayer);
        }
        catch(const std::exception &e)
        {
            // mediaPlayerView.Time_Volume( list, 0, 0, myPlayer);
            mediaPlayerView.Update_Time_Volume(0,0,myPlayer.getVolume(),myPlayer);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        current_screen = flowID.top();
    }
    while(current_screen == PLAY_MUSIC_PLAYER_ID);
}

/*========================================== Program Flow =====================================================*/
void Browser::programFlow()
{
    flowID.push(PATH_USB_ID);
    // cin.ignore();
    bool flag =true;
    while (flag){
        
        current_screen = flowID.top();
        switch(current_screen)
        {
            case MENU_ID:
                menu();
                break;
            case MEDIA_LIST_ID:
                MediaList();
                break;
            case METADATA_LIST_ID:
                metadatalist();
                break;
            case PLAY_LIST_ID:
                playlist(chosenList, chosenMusic);
                break;
            case PLAY_MUSIC_ID:
                playmusic(chosenList);
                break;
            case PLAY_LIST_MUSIC_ID:
                playlist_music(chosenList);
                break;
            case PLAY_MUSIC_PLAYER_ID:
                playmusic_player(chosenList, chosenMusic);
                break;
                // }
            case PATH_USB_ID:
                PathUsbSelection();
                break;
            case SET_PATH_ID:
                setPath();
                break;
            default:
                list = 1;
                flag=false;
                break;
        }
    }
}
