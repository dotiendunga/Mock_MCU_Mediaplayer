#ifndef UART_HPP
#define UART_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/select.h>
#include <vector>
enum SourceInput{ 
    SOURCE_UART = 0,
    SOURCE_KEYBROAD = 1
}; 
enum TypeData
{
    START_BYTE = 0xAA,
    BUTTON1_BYTE = 0x01,
    BUTTON2_BYTE = 0x02,
    ADC_BYTE     = 0x03, 
};
using namespace std;

class UARTInputData{

    private:
    public:
        const char* portname = "/dev/ttyACM0"; // Thay đổi thiết bị UART nếu cần thiết
        int fd = open(portname, O_RDWR /*| O_NOCTTY | O_SYNC*/);
        // // Hàm cài đặt các thuộc tính của giao diện UART
        // int setInterfaceAttribs(int fd, int speed);
        // // Hàm cài đặt chế độ blocking hoặc non-blocking
        // void setBlocking(int fd, bool should_block);
        std::vector<std::string> portnames;
        std::vector<int> fds;

        int setInterfaceAttribs(int fd, int speed);
        void setBlocking(int fd, bool should_block);
        // void initUART(const std::string& portname);
    


        UARTInputData();
        virtual ~UARTInputData();
        // get data from  UART 
        const char* getPortname() const { return portname; }
        int getFileDescriptor() const { return fd; }
        string userInputString();
        virtual int userInput(); // Thêm từ khóa virtual ở đây
        int check_source();
        void userInputBuffer(uint8_t* buffer);
        // void monitorUART();
        // void readUART(int fd);
        // void handleEvent(struct inotify_event* event);
        // void addPortname(const std::string& portname);
        // void removePortname(const std::string& portname);
};

#endif