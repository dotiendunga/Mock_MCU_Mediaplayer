#ifndef MEDIAPLAYERVIEW_HPP
#define MEDIAPLAYERVIEW_HPP


#include "main.hpp"
#include "Playlist.hpp"
#include "PlaylistView.hpp"
#include "MediaPlayer.hpp"
#include "UART.hpp"

class PlayMusicView:public PlaylistView
{
    private:
        // const char* portname = getPortname();
        // int fd = getFileDescriptor();
    public:
        PlayMusicView()=default;
        ~PlayMusicView()=default;

        void VPlayerMusic_DisplayList(const vector<Playlist*>& plists, size_t& currentpage);
        // void display_ListsPerPage(const vector<MediaFile*>& songs, size_t currentpage);
        int VPlayerMusic_InputList(const vector<Playlist*>& lists, size_t& currentPage);
        
        void VPlayerMusic_DisplayMusic(const vector<MediaFile*>& lists_name, size_t &currentpage, size_t timelapse, size_t duration, MediaPlayer& myPlayer);
        // int VPlayerMusic_InputMusic(const vector< MediaFile*>& lists_name, size_t& currentPage, int& volume);
        
        void Time_Volume(size_t timelapse, size_t duration, const size_t volume) const;
        void Update_Time_Volume(size_t timelapse, size_t duration, const size_t volume,MediaPlayer& myPlayer) const;
        inline string format_time(size_t total_seconds) const;
};

#endif/*MEDIAPLAYERVIEW_HPP*/
