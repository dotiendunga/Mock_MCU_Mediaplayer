#include "UART.hpp"



UARTInputData::UARTInputData()
{
    // configInput();
    setInterfaceAttribs(fd, B9600);  // cài đặt tốc độ baud 9600, 8n1 (không parity)
    setBlocking(fd, false);           // cài đặt chế độ non-blocking
}
UARTInputData::~UARTInputData()
{
    close(fd);
}


// Hàm cài đặt các thuộc tính của giao diện UART
int UARTInputData::setInterfaceAttribs(int fd, int speed)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(fd, &tty) != 0) {
        cerr << "Error " << errno << " from tcgetattr: " << strerror(errno) << endl;
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 1;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD); // ignore modem controls,
                                     // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= 0;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        cerr << "Error " << errno << " from tcsetattr: " << strerror(errno) << endl;
        return -1;
    }
    return 0;
}

// Hàm cài đặt chế độ blocking hoặc non-blocking
void UARTInputData::setBlocking(int fd, bool should_block)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(fd, &tty) != 0) {
        cerr << "Error " << errno << " from tggetattr: " << strerror(errno) << endl;
        return;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        cerr << "Error " << errno << " setting term attributes: " << strerror(errno) << endl;
    }
}

// int UARTInputData::configInput()
// {
//     if (fd < 0) {
//         cerr << "Error " << errno << " opening " << portname << ": " << strerror(errno) << endl;
//         return -1;
//     }
    
//     return 0;
// }
int UARTInputData::userInput()
{
    return 0;
}
int UARTInputData::check_source()
{
    // string shared_variable;
    fd_set readfds;
    // char buf[100];
    bool flag =true;
    while(flag){
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
            cerr << "Error in select: " << strerror(errno) << endl;
            // break;
        } else if (ret == 0) {
            // cout << "No data within five seconds." << endl;

        } else {
            if (FD_ISSET(fd, &readfds)) {
                flag = false;
                return SOURCE_UART;
            }
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                flag = false;
                return SOURCE_KEYBROAD;
            }
        }
    }
}

string UARTInputData::userInputString()
{
    string shared_variable;
    fd_set readfds;
    char buf[100];
    while (true) {
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
            cerr << "Error in select: " << strerror(errno) << endl;
            // break;
        } else if (ret == 0) {
            // cout << "No data within five seconds." << endl;

        } else {
            if (FD_ISSET(fd, &readfds)) {
                int n = read(fd, buf, sizeof buf - 1);
                if (n > 0) {
                    buf[n] = '\0';
                    shared_variable = string(buf);
                    cout << "UART event: " << shared_variable << endl;
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
    if (ret == -1) {
        std::cerr << "Error in select: " << strerror(errno) << std::endl;
        return;
    } else if (ret == 0) {
        std::cout << "No data within five seconds." << std::endl;
        return;
    } else {
        if (FD_ISSET(fd, &readfds)) {
            int count = 0;
            while (count < 3) {
                ssize_t bytes_read = read(fd, buffer + count, 1); // Đọc từng byte vào buffer

                if (bytes_read < 0) {
                    std::cerr << "Error reading from UART: " << strerror(errno) << std::endl;
                    return;
                } else if (bytes_read == 0) {
                    std::cerr << "UART has been closed." << std::endl;
                    return;
                } else {
                    std::cout << "Read " << bytes_read << " byte(s) from UART." << std::endl;
                    count++;
                    // Thoát khỏi vòng lặp nếu đã nhận đủ 3 giá trị
                    if (count == 3) {
                        return;
                    }
                }
            }
        }
    }
}


