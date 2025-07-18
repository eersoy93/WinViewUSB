#include "usb_enum.h"

// Function to get device property from registry
BOOL GetDeviceProperty(HDEVINFO deviceInfoSet, PSP_DEVINFO_DATA deviceInfoData, 
                      DWORD property, PTSTR buffer, DWORD bufferSize) {
    DWORD dataType;
    return SetupDiGetDeviceRegistryProperty(deviceInfoSet, deviceInfoData, property,
                                          &dataType, (PBYTE)buffer, bufferSize, NULL);
}

// Function to print formatted device information
void PrintDeviceInfo(const USBDeviceInfo* deviceInfo, int deviceNumber) {
    _tprintf(_T("\n--- USB Device #%d ---\n"), deviceNumber);
    _tprintf(_T("Device ID:      %s\n"), deviceInfo->deviceID);
    _tprintf(_T("Description:    %s\n"), deviceInfo->description);
    _tprintf(_T("Manufacturer:   %s\n"), deviceInfo->manufacturer);
    _tprintf(_T("Driver Version: %s\n"), deviceInfo->driverVersion);
    _tprintf(_T("Hardware ID:    %s\n"), deviceInfo->hardwareID);
    _tprintf(_T("----------------------------\n"));
}

// Function to print application header
void PrintHeader(void) {
    _tprintf(_T("=====================================\n"));
    _tprintf(_T("      USB Device Enumerator\n"));
    _tprintf(_T("    Windows API C Application\n"));
    _tprintf(_T("=====================================\n\n"));
}

// Alternative USB enumeration using all devices
int EnumerateAllUSBDevices(void) {
    HDEVINFO deviceInfoSet;
    SP_DEVINFO_DATA deviceInfoData;
    DWORD deviceIndex = 0;
    int deviceCount = 0;
    USBDeviceInfo deviceInfo;
    TCHAR buffer[512];

    // Get all present devices
    deviceInfoSet = SetupDiGetClassDevs(NULL, NULL, NULL, 
                                       DIGCF_PRESENT | DIGCF_ALLCLASSES);
    
    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Error: Could not get device information set. Error code: %lu\n"), GetLastError());
        return -1;
    }

    // Initialize device info data structure
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    // Enumerate through all devices and filter USB ones
    while (SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData)) {
        // Get device instance ID to check if it's USB
        if (SetupDiGetDeviceInstanceId(deviceInfoSet, &deviceInfoData, 
                                     buffer, sizeof(buffer)/sizeof(TCHAR), NULL)) {
            // Check if device ID contains "USB" 
            if (_tcsstr(buffer, _T("USB\\")) != NULL) {
                // Clear device info structure
                ZeroMemory(&deviceInfo, sizeof(USBDeviceInfo));

                // Copy device ID
                _tcscpy_s(deviceInfo.deviceID, sizeof(deviceInfo.deviceID)/sizeof(TCHAR), buffer);

                // Get device description
                if (!GetDeviceProperty(deviceInfoSet, &deviceInfoData, SPDRP_DEVICEDESC, 
                                      deviceInfo.description, sizeof(deviceInfo.description))) {
                    _tcscpy_s(deviceInfo.description, sizeof(deviceInfo.description)/sizeof(TCHAR), _T("Unknown"));
                }

                // Get manufacturer
                if (!GetDeviceProperty(deviceInfoSet, &deviceInfoData, SPDRP_MFG, 
                                      deviceInfo.manufacturer, sizeof(deviceInfo.manufacturer))) {
                    _tcscpy_s(deviceInfo.manufacturer, sizeof(deviceInfo.manufacturer)/sizeof(TCHAR), _T("Unknown"));
                }

                // Get driver version (Note: SPDRP_DRIVER_VERSION is not always available)
                if (!GetDeviceProperty(deviceInfoSet, &deviceInfoData, SPDRP_DRIVER, 
                                      deviceInfo.driverVersion, sizeof(deviceInfo.driverVersion))) {
                    _tcscpy_s(deviceInfo.driverVersion, sizeof(deviceInfo.driverVersion)/sizeof(TCHAR), _T("Unknown"));
                }

                // Get hardware ID
                if (!GetDeviceProperty(deviceInfoSet, &deviceInfoData, SPDRP_HARDWAREID, 
                                      deviceInfo.hardwareID, sizeof(deviceInfo.hardwareID))) {
                    _tcscpy_s(deviceInfo.hardwareID, sizeof(deviceInfo.hardwareID)/sizeof(TCHAR), _T("Unknown"));
                }

                // Print device information
                PrintDeviceInfo(&deviceInfo, ++deviceCount);
            }
        }

        deviceIndex++;
    }

    // Check if enumeration failed for reasons other than no more devices
    DWORD lastError = GetLastError();
    if (lastError != ERROR_NO_MORE_ITEMS) {
        _tprintf(_T("Error during device enumeration. Error code: %lu\n"), lastError);
        SetupDiDestroyDeviceInfoList(deviceInfoSet);
        return -1;
    }

    // Clean up
    SetupDiDestroyDeviceInfoList(deviceInfoSet);

    return deviceCount;
}

// Main USB enumeration function
int EnumerateUSBDevices(void) {
    HDEVINFO deviceInfoSet;
    SP_DEVINFO_DATA deviceInfoData;
    DWORD deviceIndex = 0;
    int deviceCount = 0;
    USBDeviceInfo deviceInfo;

    // Get device information set for USB devices
    deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_USB, NULL, NULL, 
                                       DIGCF_PRESENT);
    
    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Error: Could not get device information set. Error code: %lu\n"), GetLastError());
        // Try alternative method
        _tprintf(_T("Trying alternative enumeration method...\n\n"));
        return EnumerateAllUSBDevices();
    }

    // Initialize device info data structure
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    // Enumerate through all USB devices
    while (SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData)) {
        // Clear device info structure
        ZeroMemory(&deviceInfo, sizeof(USBDeviceInfo));

        // Get device instance ID
        if (!SetupDiGetDeviceInstanceId(deviceInfoSet, &deviceInfoData, 
                                       deviceInfo.deviceID, sizeof(deviceInfo.deviceID)/sizeof(TCHAR), NULL)) {
            _tcscpy_s(deviceInfo.deviceID, sizeof(deviceInfo.deviceID)/sizeof(TCHAR), _T("Unknown"));
        }

        // Get device description
        if (!GetDeviceProperty(deviceInfoSet, &deviceInfoData, SPDRP_DEVICEDESC, 
                              deviceInfo.description, sizeof(deviceInfo.description))) {
            _tcscpy_s(deviceInfo.description, sizeof(deviceInfo.description)/sizeof(TCHAR), _T("Unknown"));
        }

        // Get manufacturer
        if (!GetDeviceProperty(deviceInfoSet, &deviceInfoData, SPDRP_MFG, 
                              deviceInfo.manufacturer, sizeof(deviceInfo.manufacturer))) {
            _tcscpy_s(deviceInfo.manufacturer, sizeof(deviceInfo.manufacturer)/sizeof(TCHAR), _T("Unknown"));
        }

        // Get driver version (Note: SPDRP_DRIVER_VERSION is not always available)
        if (!GetDeviceProperty(deviceInfoSet, &deviceInfoData, SPDRP_DRIVER, 
                              deviceInfo.driverVersion, sizeof(deviceInfo.driverVersion))) {
            _tcscpy_s(deviceInfo.driverVersion, sizeof(deviceInfo.driverVersion)/sizeof(TCHAR), _T("Unknown"));
        }

        // Get hardware ID
        if (!GetDeviceProperty(deviceInfoSet, &deviceInfoData, SPDRP_HARDWAREID, 
                              deviceInfo.hardwareID, sizeof(deviceInfo.hardwareID))) {
            _tcscpy_s(deviceInfo.hardwareID, sizeof(deviceInfo.hardwareID)/sizeof(TCHAR), _T("Unknown"));
        }

        // Print device information
        PrintDeviceInfo(&deviceInfo, ++deviceCount);

        deviceIndex++;
    }

    // Check if enumeration failed for reasons other than no more devices
    DWORD lastError = GetLastError();
    if (lastError != ERROR_NO_MORE_ITEMS) {
        _tprintf(_T("Error during device enumeration. Error code: %lu\n"), lastError);
        SetupDiDestroyDeviceInfoList(deviceInfoSet);
        return -1;
    }

    // Clean up
    SetupDiDestroyDeviceInfoList(deviceInfoSet);

    return deviceCount;
}

// Main function
int main(void) {
    int deviceCount;
    
    // Set locale to system default for proper character support
    setlocale(LC_ALL, "");
    
    // Set console to UTF-8 for international character support
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // Enable Unicode console mode for international character support
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsole, &dwMode);
    dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, dwMode);

    // Print application header
    PrintHeader();

    _tprintf(_T("Scanning for USB devices...\n"));

    // Enumerate USB devices
    deviceCount = EnumerateUSBDevices();

    if (deviceCount < 0) {
        _tprintf(_T("Failed to enumerate USB devices.\n"));
        return 1;
    }

    _tprintf(_T("\nTotal USB devices found: %d\n"), deviceCount);

    if (deviceCount == 0) {
        _tprintf(_T("No USB devices detected on this system.\n"));
    }

    _tprintf(_T("\nPress any key to exit...\n"));
    
    // Wait for key press with Unicode support
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD inputRecord;
    DWORD numRead;
    do {
        ReadConsoleInput(hStdin, &inputRecord, 1, &numRead);
    } while (inputRecord.EventType != KEY_EVENT || !inputRecord.Event.KeyEvent.bKeyDown);

    return 0;
}
