#ifndef UART_HPP
#define UART_HPP

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <iostream>
#include <cstring>
#include <dirent.h>
#include "main.hpp"

using namespace std;

class UARTInputData{
    std::string portname;
    bool uartRunning = false;
    int fd ;
    // Hàm cài đặt các thuộc tính của giao diện UART
    int setInterfaceAttribs(int fd, int speed);
    // Hàm cài đặt chế độ blocking hoặc non-blocking
    void setBlocking(int fd, bool should_block);

public:

    UARTInputData();
    virtual ~UARTInputData();
    std::string userInputString();
    virtual int userInput();
    int check_source();
    void userInputBuffer(uint8_t* buffer);
    void findUARTDevices();
    void startMonitoring();
};

#endif