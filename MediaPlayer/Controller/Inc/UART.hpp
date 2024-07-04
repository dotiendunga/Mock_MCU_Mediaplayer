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

enum{
    
    SOURCE_UART = 0,
    SOURCE_KEYBROAD = 1
}; 

using namespace std;

class UARTInputData{

    private:
        const char* portname = "/dev/ttyAMA0"; // Thay đổi thiết bị UART nếu cần thiết
        int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
        // Hàm cài đặt các thuộc tính của giao diện UART
        int setInterfaceAttribs(int fd, int speed);
        // Hàm cài đặt chế độ blocking hoặc non-blocking
        void setBlocking(int fd, bool should_block);
    public:
        UARTInputData();
        virtual ~UARTInputData();
        // get data from  UART 
        const char* getPortname() const { return portname; }
        int getFileDescriptor() const { return fd; }
        string userInputString();
        virtual int userInput(); // Thêm từ khóa virtual ở đây
        int check_source();
};

#endif