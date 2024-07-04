#include "main.hpp"
#include "Browser.hpp"
int main()
{
    Browser myBrowser;
    myBrowser.programFlow();
    return 0;
}
// #include <iostream>
// #include <iomanip>
// #include <vector>
// #include <string>
// #include <thread>
// #include <mutex>
// #include <chrono>

// using namespace std;

// struct Song {
//     int no;
//     string name;
//     string artist;
//     int duration;  // seconds
//     string publisher;
// };

// mutex mtx; // Mutex để đồng bộ hóa truy cập vào dữ liệu
// bool stopRequested = false; // Biến để kiểm tra yêu cầu dừng

// // Hàm để di chuyển con trỏ đến vị trí cụ thể trong terminal
// void moveTo(int x, int y) {
//     cout << "\033[" << y << ";" << x << "H";
// }

// // Hàm để làm sạch các dòng trong terminal
// void clearLines(int fromLine, int toLine) {
//     for (int i = fromLine; i <= toLine; ++i) {
//         moveTo(0, i);
//         cout << "\033[2K";  // Clear the line
//     }
// }

// void printHeader(const string& content, int width, int startY) {
//     moveTo(0, startY);
//     cout << string(width, '=') << endl;
//     cout << content << endl;
//     cout << string(width, '=') << endl;
// }

// void printFooter(const string& content, int width, int startY) {
//     moveTo(0, startY);
//     cout << content << endl;
// }


// // Hàm để hiển thị các bài hát trên một trang
// void displaySongsPerPage(const vector<Song>& songs, int page, int sectionHeight, int tableWidth, int startY, bool& stop) {
//     int startIndex = (page - 1) * 10;  // Giả sử mỗi trang có 10 bài hát
//     int endIndex = min(static_cast<int>(songs.size()), startIndex + 10);

//     while (!stop) {
//         // Lock mutex để đảm bảo an toàn khi truy cập dữ liệu
//         {
//             lock_guard<mutex> lock(mtx);

//             // Di chuyển con trỏ về đầu phần giữa màn hình và xóa phần trước đó
//             clearLines(startY, startY + sectionHeight +4);
//             moveTo(0, startY);
//             // Hiển thị danh sách bài hát
//             for (int i = startIndex; i < endIndex; ++i) {
//                 cout << "|" << setw(10) << songs[i].no
//                      << "|" << setw(tableWidth / 3) << songs[i].name
//                      << "|" << setw(tableWidth / 4) << songs[i].artist
//                      << "|" << setw(tableWidth / 8) << songs[i].duration
//                      << "|" << setw(tableWidth / 6) << songs[i].publisher
//                      << "|" << endl;
//             }

//             // Thêm các dòng trống nếu cần để đủ chiều cao của phần
//             for (int i = endIndex - startIndex; i < sectionHeight - 2; ++i) {
//                 cout << "|" << setw(10) << " "
//                      << "|" << setw(tableWidth / 3) << " "
//                      << "|" << setw(tableWidth / 4) << " "
//                      << "|" << setw(tableWidth / 8) << " "
//                      << "|" << setw(tableWidth / 6) << " "
//                      << "|" << endl;
//             }
//             // Phần cuối: Thông tin bổ sung và điều hướng
//             cout << string(tableWidth, '=') << "\n" << endl;
//             string footer = "Total Media list: " + to_string(songs.size()) + "\n"
//                             + "Page: " + to_string(1) + "\n"
//                             + string(tableWidth / 4, ' ')
//                             + "P. Previous"
//                             + string(15, ' ')
//                             + "N. Next"
//                             + string(15, ' ')
//                             + "E. Exit";
//             printFooter(footer, tableWidth, startY + sectionHeight);
//             cout << string(tableWidth, '=') << "\n" << endl;
//             // Di chuyển con trỏ về cuối màn hình và in dòng thông báo
//             cout<< "Nhap vao di con troi"<<endl;
//         }
//         // Sleep để giả lập thời gian
//         this_thread::sleep_for(chrono::milliseconds(1000));
//     }
// }

// int main() { 
//     int tableWidth = 120;  // Chiều rộng cố định của mỗi phần
//     int sectionHeight = 10; // Chiều cao cố định của phần giữa
//     int currentpage = 1;  // Trang hiện tại
//     int startY = 6; // Vị trí bắt đầu của phần giữa trên màn hình

//     vector<Song> songs = {
//         {1, "Song1", "Artist1", 210, "Publisher1"},
//         {2, "Song2", "Artist2", 180, "Publisher2"},
//         // Thêm các bài hát khác vào đây
//     };
//     system("clear");
//     string header = "Media List";
//     string headerSize = string(tableWidth / 2 - header.length() / 2, ' ') + header;
//     // Phần đầu: Tiêu đề và tiêu đề cột
//     printHeader(headerSize, tableWidth, 0);
//     cout << "|" << left << setw(10) << " No."
//          << "|" << left << setw(tableWidth / 3) << "Name"
//          << "|" << left << setw(tableWidth / 4) << "Artist"
//          << "|" << left << setw(tableWidth / 8) << "Duration (s)"
//          << "|" << left << setw(tableWidth / 6) << "Publisher" << "|" << endl;
//     cout << string(tableWidth, '-') << "\n" << endl;

//     // Phần giữa: Bắt đầu luồng để cập nhật và hiển thị phần danh sách bài hát
//     thread displayThread(displaySongsPerPage, ref(songs), currentpage, sectionHeight, tableWidth, startY, ref(stopRequested));

//     cout << "Nhấn Enter để dừng..." << endl;
//     while(true){
//         int a;
//         cin>>a;
//         if(a==0){
//             //  Yêu cầu dừng luồng và chờ nó kết thúc
//             stopRequested = true;
//             displayThread.join(); // Chờ đến khi luồng kết thúc
//             cout << "Đã dừng." << endl;
//             break;
//         }else{
//             cout<<"concop";
//         }
//     }
//     return 0;
// }


// #include <iostream>
// #include <iomanip>
// #include <vector>
// #include <string>

// using namespace std;
// #include <thread>

// // Hàm delay sử dụng std::this_thread::sleep_for
// void delay(int milliseconds) {
//     std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
// }

// struct Song {
//     int no;
//     string name;
//     string artist;
//     int duration;  // seconds
//     string publisher;
// };

// // Hàm để di chuyển con trỏ đến vị trí cụ thể trong terminal
// void moveTo(int x, int y) {
//     cout << "\033[" << y << ";" << x << "H";
// }

// // Hàm để làm sạch các dòng trong terminal
// void clearLines(int fromLine, int toLine) {
//     for (int i = fromLine; i <= toLine; ++i) {
//         moveTo(0, i);
//         cout << "\033[2K";  // Clear the line
//     }
// }

// // Hàm để hiển thị các bài hát trên một trang
// void displaySongsPerPage(const vector<Song>& songs, int page, int sectionHeight, int tableWidth) {
//     int startIndex = (page - 1) * 10;  // Giả sử mỗi trang có 10 bài hát
//     int endIndex = min(static_cast<int>(songs.size()), startIndex + 10);
//     for (int i = startIndex; i < endIndex; ++i) {
//         cout << "|" << setw(5) << songs[i].no
//              << "|" << setw(tableWidth/3) << songs[i].name
//              << "|" << setw(tableWidth/4) << songs[i].artist
//              << "|" << setw(tableWidth/8) << songs[i].duration
//              << "|" << setw(tableWidth/8) << songs[i].publisher
//              << "|" << endl;
//     }
//     // Thêm các dòng trống nếu cần để đủ chiều cao của phần
//     for (int i = endIndex - startIndex; i < sectionHeight - 2; ++i) {
//         cout << "|" << setw(5) << " "
//              << "|" << setw(tableWidth/3) << " "
//              << "|" << setw(tableWidth/4) << " "
//              << "|" << setw(tableWidth/8) << " "
//              << "|" << setw(tableWidth/8) << " "
//              << "|" << endl;
//     }
// }

// void printSection(const string& content, int width, int height) {
//     cout << string(width, '=') << endl;
//     cout << content << endl;
//     cout << string(width, '=') << endl;
//     for (int i = 0; i < height - 3; ++i) {  // Trừ đi số dòng đã in
//         cout << endl;
//     }
// }

// // Hàm để cập nhật phần giữa
// void updateMiddleSection(const vector<Song>& songs, int page, int sectionHeight, int tableWidth, int startY) {
//     clearLines(startY, startY + sectionHeight - 1);  // Clear the middle section

//     moveTo(0, startY);  // Move cursor to the start of the middle section

//     // Hiển thị các bài hát
//     displaySongsPerPage(songs, page, sectionHeight, tableWidth);
// }

// int main() {
//     int tableWidth = 120;  // Chiều rộng cố định của mỗi phần
//     int sectionHeight = 10; // Chiều cao cố định của mỗi phần
//     int currentpage = 1;  // Trang hiện tại

//     vector<Song> songs = {
//         {1, "Song1", "Artist1", 210, "Publisher1"},
//         {2, "Song2", "Artist2", 180, "Publisher2"},
//         // Thêm các bài hát khác vào đây
//     };

//     system("clear");
    
//     string header = "Media List";
//     string headerSize = string(tableWidth / 2 - header.length() / 2, ' ') + header;
//     printSection(headerSize, tableWidth, 3);

//     cout << "|" << left << setw(10) << " No."
//          << "|" << left << setw(tableWidth / 3) << "Name"
//          << "|" << left << setw(tableWidth / 4) << "Artist"
//          << "|" << left << setw(tableWidth / 8) << "Duration (s)"
//          << "|" << left << setw(tableWidth / 8) << "Publisher" << "|" << endl;
//     cout << string(tableWidth, '-') << "\n" << endl;

//     // Hiển thị danh sách bài hát ban đầu
//     updateMiddleSection(songs, currentpage, sectionHeight, tableWidth, 7);

//     cout << string(tableWidth, '-') << endl;

    
    


//     // Ví dụ về việc cập nhật phần giữa khi xóa một bài hát
//     cout << "Deleting song 1..." << endl;
//     songs.erase(songs.begin());  // Xóa bài hát đầu tiên
//     updateMiddleSection(songs, currentpage, sectionHeight, tableWidth,7);
//     delay(2000);
//     // Ví dụ về việc cập nhật phần giữa khi thêm một bài hát
//     cout << "Adding a new song..." << endl;
//     songs.push_back({3, "Song3", "Artist3", 200, "Publisher3"});  // Thêm bài hát mới
//     updateMiddleSection(songs, currentpage, sectionHeight, tableWidth,7);
//     // Phần chân
//     string footer = "Total Media list: " + to_string(songs.size()) + "\n"
//                     + "Page: " + to_string(currentpage) + "\n"
//                     + string(tableWidth / 4, ' ')
//                     + "P. Previous"
//                     + string(15, ' ')
//                     + "N. Next"
//                     + string(15, ' ')
//                     + "E. Exit";
//     printSection(footer, tableWidth, 5);
//     return 0;
// }

// #include <iostream>
// #include <thread>
// #include <chrono>

// void displayMessage() {
//     std::string messages[] = {"Original Text", "Updated Text"};
//     int index = 0;

//     while (true) {
//         // Di chuyển con trỏ về đầu dòng và xóa dòng hiện tại
//         system("clear");
//         std::cout << "\r\033[K" << messages[index] << std::flush<<std::endl;
//         std::cout << "\r\033[K" << messages[index] << std::flush<<std::endl;
//         std::cout << "\r\033[K" << messages[index] << std::flush<<std::endl;
//         std::cout << "\r\033[K" << messages[index] << std::flush<<std::endl;
//         std::cout << "\r\033[K" << messages[index] << std::flush<<std::endl;
//         std::cout << "\r\033[K" << messages[index] << std::flush<<std::endl;
//         std::cout << "\r\033[K" << messages[index] << std::flush<<std::endl;
//         std::cout << "\r\033[K" << messages[index] << std::flush<<std::endl;

//         // Chuyển đổi giữa các thông điệp
//         index = (index + 1) % 2;

//         // Chờ 1 giây trước khi cập nhật lại
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//     }
// }

// int main() {
//     std::thread displayThread(displayMessage);

//     // Đảm bảo chương trình không kết thúc ngay lập tức
//     displayThread.join();

//     return 0;
// }