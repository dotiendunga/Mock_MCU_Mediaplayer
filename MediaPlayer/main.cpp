#include "main.hpp"
#include "Browser.hpp"
// #include "UART.hpp"
int main()
{
    
    Browser myBrowser;
    myBrowser.programFlow();

    return 0;
}

// #include <iostream>
// #include <cstring>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/select.h>
// #include <sys/time.h>
// #include <dirent.h>

// #define SOURCE_UART 1
// #define SOURCE_KEYBOARD 2

// class UARTInputData {
// private:
//     std::string portname = "/dev/ttyACM0";
//     std::string preport = "";
//     int fd = -1;

// public:
//     void findUARTDevices() {
//         if (preport != portname) {
//             struct dirent *entry;
//             DIR *dir = opendir("/dev");
//             if (dir == nullptr) {
//                 std::cerr << "Error opening /dev directory" << std::endl;
//                 return;
//             }
//             while ((entry = readdir(dir)) != nullptr) {
//                 std::string deviceName = "/dev/";
//                 if (strncmp(entry->d_name, "ttyACM", 6) == 0) {
//                     deviceName += entry->d_name;
//                     portname = deviceName;
//                     preport = portname;
//                     if (fd != -1) {
//                         close(fd); // Đóng file descriptor cũ nếu đang mở
//                     }
//                     fd = open(portname.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
//                     if (fd != -1) {
//                         std::cout << "Opened device: " << portname << std::endl;
//                         closedir(dir);
//                         return;
//                     }
//                 }
//             }
//             closedir(dir);
//             std::cerr << "No UART device found." << std::endl;
//         } else {
//             return;
//         }
//     }

//     int check_source() {
//         findUARTDevices();

//         fd_set readfds;
//         bool flag = true;

//         while (flag) {
//             FD_ZERO(&readfds);
//             FD_SET(fd, &readfds);
//             FD_SET(STDIN_FILENO, &readfds);

//             struct timeval tv;
//             tv.tv_sec = 5;
//             tv.tv_usec = 0;

//             int max_fd = std::max(fd, STDIN_FILENO) + 1;
//             int ret = select(max_fd, &readfds, nullptr, nullptr, &tv);

//             if (ret == -1) {
//                 std::cerr << "Error in select: " << strerror(errno) << std::endl;
//                 return SOURCE_KEYBOARD; // Mặc định cho bàn phím nếu có lỗi
//             } else if (ret == 0) {
//                 std::cout << "Timeout, no data received." << std::endl;
//                 continue; // Timeout, kiểm tra lại
//             } else {
//                 if (FD_ISSET(fd, &readfds)) {
//                     flag = false;
//                     std::cout << "Data received from UART." << std::endl;
//                     return SOURCE_UART;
//                 }
//                 if (FD_ISSET(STDIN_FILENO, &readfds)) {
//                     flag = false;
//                     std::cout << "Data received from keyboard." << std::endl;
//                     return SOURCE_KEYBOARD;
//                 }
//             }
//         }

//         return SOURCE_KEYBOARD; // Mặc định cho bàn phím
//     }
// };

// int main() {
//     UARTInputData uart;
//     while (true) {
//         int source = uart.check_source();
//         if (source == SOURCE_UART) {
//             std::cout << "Data received from UART." << std::endl;
//             std::cin.ignore();
//         } else if (source == SOURCE_KEYBOARD) {
//             std::cout << "Data received from keyboard." << std::endl;
//             std::cin.ignore();
//         } else {
//             std::cerr << "Unknown data source." << std::endl;
//             std::cin.ignore();
//         }
//     }
//     return 0;
// }