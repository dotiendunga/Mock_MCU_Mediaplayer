#ifndef GETINPUTBUTTON_HPP
#define GETINPUTBUTTON_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/select.h>

using namespace std;

class UARTInputData{

    private:
        const char* portname = "/dev/ttyUSB0"; // Thay đổi thiết bị UART nếu cần thiết
        int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
        // Hàm cài đặt các thuộc tính của giao diện UART
        int setInterfaceAttribs(int fd, int speed);

        // Hàm cài đặt chế độ blocking hoặc non-blocking
        void setBlocking(int fd, bool should_block);
    public:
        UARTInputData();
        ~UARTInputData();
        //  config Get data from UART 
        int configInput();
        // get data from  UART 
        string getDataUART();
};

#endif