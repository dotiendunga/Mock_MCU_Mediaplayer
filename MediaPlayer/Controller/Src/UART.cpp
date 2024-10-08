#include "UART.hpp"



UARTInputData::UARTInputData()
{
    init_crc8_table();
    setInterfaceAttribs(fd, B115200);  // cài đặt tốc độ baud 9600, 8n1 (không parity)
    setBlocking(fd, false);           // cài đặt chế độ non-blocking
    std::thread uartThread(&UARTInputData::check_port, this);
    uartThread.detach(); // Tách thread ra để nó tự quản lý
    
}
UARTInputData::~UARTInputData() {
    {
        std::lock_guard<std::mutex> lock(uartMutex);
        uartRunning = false;
    }
    cv.notify_all(); // Đánh thức thread để nó có thể kết thúc
}

void UARTInputData::check_port() {
    while (uartRunning) {
        std::unique_lock<std::mutex> lock(uartMutex);
        cv.wait(lock, [this] { return !uartRunning || fd == -1; });

        if (!uartRunning) break;

        if (fd == -1) {
            close(fd);  // Đóng fd cũ   
            ReinitUart(); // Thử mở lại UART
        } else {
            if (fcntl(fd, F_GETFL) == -1 && errno == EBADF) {
                close(fd);  // Đóng fd cũ
                ReinitUart();
            }
        }
    }
}

int UARTInputData::ReinitUart() {
    std::lock_guard<std::mutex> lock(uartMutex);
    DIR *dir = opendir("/dev");
    if (dir == nullptr) {
        std::cerr << "Error opening /dev directory" << std::endl;
        return -1;
    }
    bool found = false;
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string deviceName = "/dev/";
        if (strncmp(entry->d_name, "ttyACM", 6) == 0) {
            deviceName += entry->d_name;
            // Mở fd mới
            fd = open(deviceName.c_str(), O_RDWR);
            cout<<fd;
            if (fd == -1) {
                std::cerr << "Failed to open new_fd: " << strerror(errno) << std::endl;
            } else {
                portname = deviceName;
                setInterfaceAttribs(fd, B1152000);  // Set baud rate to 9600, 8n1 (no parity)
                setBlocking(fd, false);           // Set non-blocking mode
                found = true;
                break;
            }
        }
    }
    closedir(dir);
    if (!found) {
        // std::cerr << "No UART device found." << std::endl;
        return -1;
    }
    cv.notify_all(); // Đánh thức thread để kiểm tra lại port
    return 0;
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
        FD_SET(STDIN_FILENO, &readfds);
        struct timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        int max_fd = std::max(fd, STDIN_FILENO) + 1;
        int ret = select(max_fd, &readfds, nullptr, nullptr, &tv);

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
                // std::cout << "Data received from UART." << std::endl;
                return SOURCE_UART;
            }
            if (FD_ISSET(STDIN_FILENO, &readfds))
            {
                flag = false;
                // std::cout << "Data received from keyboard." << std::endl;
                return SOURCE_KEYBROAD;
            } else {
                // std::cerr << "Unknown data source." << std::endl;
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
    while (true)
    {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval tv;
        tv.tv_sec = 5; // Thời gian chờ tối đa là 5 giây
        tv.tv_usec = 0;

        int max_fd = max(fd, STDIN_FILENO) + 1;
        int ret = select(max_fd, &readfds, NULL, NULL, &tv);

        if (ret == -1)
        {
            std::cerr << "Error in select: 5" << strerror(errno) << std::endl;
            break;
        }
        else if(ret == 0)
        {
            // cout << "No data within five seconds." << endl;
        }
        else
        {
            if (FD_ISSET(fd, &readfds)) {
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
    return "\0";
}
void UARTInputData::sendRequest(uint8_t*request,size_t length)
{
        write(fd,request,length);

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
                    std::cout << "Error reading from UART: " << strerror(errno) << std::endl;
                    break;
                } else if (bytes_read == 0)
                {
                    // std::cout << "UART has been closed." << std::endl;
                    break;
                }
                else
                {
                    if(*buffer != START_BYTE)
                    {
                        /* Incorrect start byte */
                        break;
                    }
                    else
                    {
                        count++;
                        if (count == 4)
                        {
                            if(buffer[3] != crc8(buffer, 3))
                            {
                                /* Correct data */
                                break;
                            }
                            else
                            {
                                /* Incorrect data */
                                count = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}