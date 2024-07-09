#ifndef UART_HPP
#define UART_HPP

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <cstring>
#include <thread>
#include <chrono>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <mutex>
#include <condition_variable>
using namespace std;

#define CRC8_TABLE_SIZE  256

typedef enum
{ 
    SOURCE_UART     = 0,
    SOURCE_KEYBROAD = 1
} SourceInput_t; 
typedef enum
{
    START_BYTE      = 0xAA,
    BUTTON1_BYTE    = 0x01,
    BUTTON2_BYTE    = 0x02,
    ADC_BYTE        = 0x03, 
    ERROR_BYTE      = 0x00
} UART_TypeData_t;

typedef enum
{
    NUMBER_TYPE     = 0,
    STRING_TYPE
} Keyboard_TypeData_t;

typedef struct
{
    Keyboard_TypeData_t keyboardType;
    size_t valueNumber;
    string valueString;
} Keyboard_Data_t;

typedef struct
{
    UART_TypeData_t uartType;
    size_t valueNumber;
} UART_Data_t;

typedef struct
{
    SourceInput_t source;
    UART_Data_t uartData;
    Keyboard_Data_t keyboardData;
} UART_Keyboard_Input;

class UARTInputData {
public:
    UARTInputData();
    virtual ~UARTInputData();
    void check_port();
    int ReinitUart();
    int setInterfaceAttribs(int fd, int speed);
    void setBlocking(int fd, bool should_block);
    int userInput();
    SourceInput_t check_source();
    std::string userInputString();
    void userInputBuffer(uint8_t* buffer);
    int userInputInt();
private:
    // std::string portname = "/dev/ttyACM0"; // Thay đổi thiết bị UART nếu cần thiết
    // int fd = open(portname.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    // static constexpr size_t CRC8_TABLE_SIZE = 256;
    uint8_t crc8_table[CRC8_TABLE_SIZE]; // Khai báo mảng crc8_table
    bool uartRunning;
    int fd;
    std::string portname;
    std::mutex uartMutex;
    std::condition_variable cv;
    /*=====================================Checksum BEGIN ===================================*/
        // Hàm khởi tạo bảng tra cứu CRC-8
        void init_crc8_table()
        {
            uint8_t crc;
            for (uint16_t i = 0; i < 256; i++) {
                crc = i;
                for (uint8_t j = 8; j > 0; j--) {
                    if (crc & 0x80) {
                        crc = (crc << 1) ^ 0x07;
                    } else {
                        crc <<= 1;
                    }
                }
                crc8_table[i] = crc;
            }
        }

        // Hàm tính toán CRC-8
        uint8_t crc8(const uint8_t *data, size_t length)
        {
            uint8_t crc = 0x00;
            for (size_t i = 0; i < length; i++)
            {
                uint8_t byte = data[i];
                uint8_t lookup_index = (crc ^ byte) & 0xFF;
                crc = crc8_table[lookup_index];
            }
            return crc;
        }
        /*=====================================Checksum END ===================================*/
};

#endif