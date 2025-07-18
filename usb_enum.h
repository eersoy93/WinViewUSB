#ifndef USB_ENUM_H
#define USB_ENUM_H

#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>

// Define USB device class GUID if not available
#ifndef GUID_DEVCLASS_USB
DEFINE_GUID(GUID_DEVCLASS_USB, 0x36fc9e60L, 0xc465, 0x11cf, 0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);
#endif

// Structure to hold USB device information
typedef struct {
    TCHAR deviceID[256];
    TCHAR description[256];
    TCHAR manufacturer[256];
    TCHAR driverVersion[64];
    TCHAR hardwareID[512];
} USBDeviceInfo;

// Function declarations
int EnumerateUSBDevices(void);
int EnumerateAllUSBDevices(void);
BOOL GetDeviceProperty(HDEVINFO deviceInfoSet, PSP_DEVINFO_DATA deviceInfoData, 
                      DWORD property, PTSTR buffer, DWORD bufferSize);
void PrintDeviceInfo(const USBDeviceInfo* deviceInfo, int deviceNumber);
void PrintHeader(void);

#endif // USB_ENUM_H
