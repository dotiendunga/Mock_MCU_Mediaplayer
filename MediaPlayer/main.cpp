#include "main.hpp"
#include "Browser.hpp"
// #include "UART.hpp"
int main()
{
    
    Browser myBrowser;
    myBrowser.programFlow();
    // UARTInputData myUART;
    // myUART.setInterfaceAttribs(myUART.fd, B9600);
    // while(1)
    // {
    //     uint8_t buffer[4];
    //     myUART.userInputBuffer(buffer);
    //     cout << buffer;
    // }
    
    return 0;
}
