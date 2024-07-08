#include "UART.hpp"


#include "thread"
UARTInputData::UARTInputData()
{
    // startMonitoring();
    findUARTDevices();
}
UARTInputData::~UARTInputData()
{
    uartRunning = false; // Signal thread to stop

}

void UARTInputData::findUARTDevices() {
    while (uartRunning) {
        DIR *dir = opendir("/dev");
        if (dir == nullptr) {
            std::cerr << "Error opening /dev directory" << std::endl;
            continue;
        }
        bool found = false;
        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string deviceName = "/dev/";
            if (strncmp(entry->d_name, "ttyACM", 6) == 0) {
                deviceName += entry->d_name;
                if (deviceName != portname) {
                    
                    if (fd != -1) {
                        close(fd); // Close previous fd if open
                    }
                    this->fd = open(deviceName.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
                    if (fd == -1) {
                        std::cerr << "Error opening " << deviceName << ": " << strerror(errno) << std::endl;
                        continue;
                    }
                    this->portname = deviceName;
                    std::cout << "Opened device: " << portname << std::endl;
                    setInterfaceAttribs(fd, B9600);  // Set baud rate to 9600, 8n1 (no parity)
                    setBlocking(fd, false);           // Set non-blocking mode
                    found = true;
                    break;
                    if (!found) {
                        std::cerr << "No new UART device found." << std::endl;
                    }  
                }
                
            }
        }
        closedir(dir);
        // Wait before checking again
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
// void UARTInputData::startMonitoring() {
//     uartRunning = true;
//     std::thread monitorThread(&UARTInputData::findUARTDevices, this);
//     monitorThread.detach();
// }

// Hàm cài đặt các thuộc tính của giao diện UART
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

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit charsS
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

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        cerr << "Error " << errno << " from tcsetattr2: " << strerror(errno) << endl;
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

int UARTInputData::check_source() {
    fd_set readfds;
    bool flag = true;
    
    // Set fd to non-blocking mode
    while (flag) {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        FD_SET(STDIN_FILENO, &readfds);
        struct timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        int max_fd = std::max(fd, STDIN_FILENO) + 1;
        int ret = select(max_fd, &readfds, nullptr, nullptr, &tv);

        if (ret == -1) {
            std::cerr << "Error in select: " << strerror(errno) << std::endl;
        } else if (ret == 0) {
            continue; // Timeout, check again
        } else {
            if (fd != -1 && FD_ISSET(fd, &readfds)) {
                flag = false;
                std::cout << "Data received from UART." << std::endl;
                return SOURCE_UART;
            } else if (FD_ISSET(STDIN_FILENO, &readfds)) {
                flag = false;
                std::cout << "Data received from keyboard." << std::endl;
                return SOURCE_KEYBROAD;
            } else {
                std::cerr << "Unknown data source." << std::endl;
            }
        }
    }
    return SOURCE_KEYBROAD; // Default to keyboard source
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
            cerr << "Error in select: 5" << strerror(errno) << endl;
            // break;
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
                    // cout << "UART event: " << shared_variable << endl;
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
                    // std::cerr << "Error reading from UART: " << strerror(errno) << std::endl;
                    return;
                } else if (bytes_read == 0)
                {
                    // std::cerr << "UART has been closed." << std::endl;
                    // return;
                }
                else
                {
                    if(*buffer != START_BYTE)
                    {
                        break;
                    }
                    else
                    {
                        // std::cout << "Read " << bytes_read << " byte(s) from UART." << std::endl;
                        count++;
                        // Thoát khỏi vòng lặp nếu đã nhận đủ 4 giá trị
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


