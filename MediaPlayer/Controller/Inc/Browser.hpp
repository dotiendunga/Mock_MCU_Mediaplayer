#pragma once
#include <string>
#include <filesystem>
#include <vector>
#include <stack>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/taglib.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

#include "MediaPlayerView.hpp"
#include "MediaFileView.hpp"
#include "MenuView.hpp"
#include "MediaPathView.hpp"
#include "AudioFile.hpp"
#include "Playlist.hpp"
#include "MediaPlayer.hpp"
#include "MetadataView.hpp"
#include "Metadata.hpp"
#include "PlaylistView.hpp"
#include "MediaPlayerView.hpp"
#include "USB.hpp"
#include "UART.hpp"

#define START_PAGE                  1
#define MP3_EXTENSION               ".mp3"
#define MP4_EXTENSION               ".mp4"
#define MP3_TYPE                    1
#define MP4_TYPE                    2

enum FlowID
{   
    MENU_ID = 0,
    MEDIA_LIST_ID = 1,
    METADATA_LIST_ID =2,
    PLAY_LIST_ID = 3,
    PLAY_MUSIC_ID = 4,
    PLAY_LIST_MUSIC_ID = 5,
    PLAY_MUSIC_PLAYER_ID = 6,
    PATH_USB_ID =7,
    SET_PATH_ID =8,
    CLOSE_PROGRAM =9
};

// C++17 up
namespace fs = std:: filesystem;


class Browser
{
private:
    std::string Path;
    int current_screen;
    int chosenList = 1;
    int chosenMusic = 1;
    size_t list = 1;
    int duration;
    int volume;

    string file_path = "";
    string file_name = "";
    int file_type = 0;
    /* Stack */
    std::stack<int> flowID;

    /* Vector data */
    vector<Playlist*> vPlayList;

    /* View declaration */
    MediaPathView mediaPathView;
    PlayMusicView mediaPlayerView; 
    MenuView menuView;
    MediaFileView mediaFileView;
    Metadataview metadataView;
    PlaylistView playListView;

    /* Player */
    MediaPlayer& myPlayer = MediaPlayer::getMediaPlayer();
    Metadata metaData;
    
    
    /* Thread */
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::duration<double> timelapse;
    std::thread myThread;
    std::thread myThread2;
    TagLib::FileRef fileRef;

    std::mutex mtx1;
    std::mutex mtx2;

    /* USB */
    USBDeviceScanner usbDeviceScanner;
    std::vector<std::string> devices;
    /* UART */
    UARTInputData myUART;

    /* Inline */
    inline void startThread()
    {
        myThread = std::thread(&Browser::updatePlayerView, this);
    }

    inline void resetTimer()
    {
        startTime = std::chrono::steady_clock::now();
        timelapse = std::chrono::duration<double>::zero();
    }

public:
    Browser(/* args */);
    ~Browser();

    void setPath();
    void setPathView();
    void PathUsbSelection();

    void loadFile();   
    void FreeAll();

    /* Get input */
    int userInput() ;
    string userInputString();
    UART_Keyboard_Input* UART_Keyboard();

    /* Menu */ 
    void menu();

    /*MEDIA*/
    void MediaList();
    void MediaList_ProcessInput(const char option);

    /*META DATA*/
    void metadatalist();
    void viewMetadata(const string& file_path,const string& file_name,const int& file_type);
    void updateMetadata(string& file_path,string& file_name,int& file_type);
    
    /**/
    void playlist(int& chosenList, int& chosenMusic);
    void playlist_music(int& chosenList);

    /*Create Playlist*/
    void createList();
    
    /* Delete playlist */
    void deleteList();

    /* Rename playlist */
    void renameList();

    /* Music player*/
    void playmusic(int& chosenList);
    void playmusic_player(int& chosenList, int& chosenMusic);
    void Player_ProcessInput(char option);

    /* program Flow */
    void programFlow();

    /*thread*/
    void updatePlayerView();
};
