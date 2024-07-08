#include "UART.hpp"

UARTInputData::UARTInputData()
{
    setInterfaceAttribs(fd, B9600);  // cài đặt tốc độ baud 9600, 8n1 (không parity)
    setBlocking(fd, false);           // cài đặt chế độ non-blocking
    
}
UARTInputData::~UARTInputData()
{
    close(fd);
}

int UARTInputData::setInterfaceAttribs(int fd, int speed)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0) {
        cerr << "Error " << errno << " from tcgetattr1: " << strerror(errno) << endl;
        return -1;
    }
    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 5;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag |= 0;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        cerr << "Error " << errno << " from tcsetattr2: " << strerror(errno) << endl;
        return -1;
    }
    return 0;
}

void UARTInputData::setBlocking(int fd, bool should_block)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(fd, &tty) != 0) {
        cerr << "Error " << errno << " from tggetattr3: " << strerror(errno) << endl;
        return;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        cerr << "Error " << errno << " setting term attributes: " << strerror(errno) << endl;
    }
}

int UARTInputData::userInput()
{
    return 0;
}
SourceInput_t UARTInputData::check_source()
{
    fd_set readfds;
    bool flag =true;
    while(flag)
    {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        FD_SET(STDIN_FILENO, &readfds); // Thêm stdin vào tập tệp mô tả cần kiểm tra
        // Đặt thời gian chờ
        struct timeval tv;
        tv.tv_sec = 5; // Thời gian chờ tối đa là 5 giây
        tv.tv_usec = 0;
        int max_fd = max(fd, STDIN_FILENO) + 1;
        int ret = select(max_fd, &readfds, NULL, NULL, &tv);

        if (ret == -1) {
            cerr << "Error in select: 4" << strerror(errno) << endl;
            // break;
        }
        else if (ret == 0)
        {
            // cout << "No data within five seconds." << endl;
        }
        else
        {
            if (FD_ISSET(fd, &readfds))
            {
                flag = false;
                return SOURCE_UART;
            }
            if (FD_ISSET(STDIN_FILENO, &readfds))
            {
                flag = false;
                return SOURCE_KEYBROAD;
            }
        }
    }
    return SOURCE_KEYBROAD;
}

string UARTInputData::userInputString()
{
    string shared_variable;
    fd_set readfds;
    char buf[100];
    while (true) {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval tv;
        tv.tv_sec = 5; // Thời gian chờ tối đa là 5 giây
        tv.tv_usec = 0;

        int max_fd = max(fd, STDIN_FILENO) + 1;
        int ret = select(max_fd, &readfds, NULL, NULL, &tv);

        if (ret == -1) {
            cerr << "Error in select: 5" << strerror(errno) << endl;
            break;
        }
        else if(ret == 0)
        {
            // cout << "No data within five seconds." << endl;

        }
        else
        {
            if (FD_ISSET(fd, &readfds))
            {
                int n = read(fd, buf, sizeof buf - 1);
                if (n > 0)
                {
                    buf[n] = '\0';
                    shared_variable = string(buf);
                    return shared_variable; 
                }
            }
        }
        return "\0";
    }
}
void UARTInputData::userInputBuffer(uint8_t* buffer)
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    // Đặt thời gian chờ
    struct timeval tv;
    tv.tv_sec = 5; // Thời gian chờ tối đa là 5 giây
    tv.tv_usec = 0;

    int max_fd = fd + 1; // Chỉ sử dụng fd của UART
    int ret = select(max_fd, &readfds, NULL, NULL, &tv);
    if (ret == -1)
    {
        std::cerr << "Error in select: 6" << strerror(errno) << std::endl;
        return;
    }
    else if(ret == 0)
    {
        // std::cout << "No data within five seconds." << std::endl;
        return;
    } 
    else
    {
        if(FD_ISSET(fd, &readfds))
        {
            int count = 0;
            while (count < 4) 
            {
                ssize_t bytes_read = read(fd, buffer + count, 1); // Đọc từng byte vào buffer
                if (bytes_read < 0)
                {
                    std::cerr << "Error reading from UART: " << strerror(errno) << std::endl;
                    return;
                } else if (bytes_read == 0)
                {
                    std::cerr << "UART has been closed." << std::endl;
                    return;
                }
                else
                {
                    if(*buffer != START_BYTE)
                    {
                        break;
                    }
                    else
                    {
                        count++;
                        if (count == 4)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
}


