#pragma once

#include <libudev.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

class USBDeviceScanner {
public:
    USBDeviceScanner();
    ~USBDeviceScanner();
    void scanDevices() ;
    std::vector<std::string> getUSBMountPoints() const ;
private:
    bool isUSBDevice(struct udev_device *dev);
    void mapDeviceNodesToMountPoints();
    std::string unescapeMountPoint(const std::string &mountPoint);
    struct udev *udev;
    std::vector<std::string> usbDevices;
    std::vector<std::string> usbMountPoints;
};


