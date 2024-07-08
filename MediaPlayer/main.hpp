#ifndef MAIN_HPP
#define MAIN_HPP

#include "SDL.h"
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/taglib.h>
#include <string.h> 
#include <filesystem>

enum SourceInput{ 
    SOURCE_UART = 0,
    SOURCE_KEYBROAD = 1
}; 
enum TypeData
{
    START_BYTE = 0x7F,
    BUTTON1_BYTE = 0x01,
    BUTTON2_BYTE = 0x02,
    ADC_BYTE     = 0x03, 
};

using namespace std;

#define PAGE_SONG_SIZE 4
#define PAGE_LIST_SIZE 4
#define tableWidth 120


#endif /*MAIN_HPP*/