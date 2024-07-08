#ifndef UART_HPP
#define UART_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/select.h>
using namespace std;


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
    int valueNumber;
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

class UARTInputData
{
    private:
        const char* portname = "/dev/ttyACM0"; // Thay đổi thiết bị UART nếu cần thiết
        int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
        uint8_t crc8_table[256];

        std::vector<std::string> portnames;
        std::vector<int> fds;

        // Hàm cài đặt các thuộc tính của giao diện UART
        int setInterfaceAttribs(int fd, int speed);
        // Hàm cài đặt chế độ blocking hoặc non-blocking
        void setBlocking(int fd, bool should_block);
    
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

    public:
        UARTInputData();
        virtual ~UARTInputData();
        // get data from  UART 
        const char* getPortname() const { return portname; }
        int getFileDescriptor() const { return fd; }
        string userInputString();
        virtual int userInput(); // Thêm từ khóa virtual ở đây
        SourceInput_t check_source();
        void userInputBuffer(uint8_t* buffer);
};

#endif