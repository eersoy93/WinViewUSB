# WinViewUSB - Windows USB Device Enumerator

A Windows console application written in C that enumerates and displays information about USB devices connected to the system using Windows API.

## Features

- Lists all USB devices connected to the system
- Displays detailed device information including:
  - Device ID
  - Description
  - Manufacturer
  - Driver Version
  - Hardware ID
- Error handling and validation
- Clean console output formatting

## Requirements

- Windows operating system
- MinGW-w64
- Windows SDK (for setupapi.h, devguid.h headers)
- Make utility (mingw32-make)

## Building the Application

### Using Make

```bash
# Build the application
make

# Build release version (optimized)
make release

# Build debug version (with debug symbols)
make debug

# Clean build artifacts
make clean

# Clean and rebuild
make rebuild

# Show help
make help
```

### Manual Compilation

```bash
gcc -Wall -Wextra -std=c99 -O2 -c main.c -o main.o
gcc main.o -o WinViewUSB.exe -lsetupapi -ladvapi32
```

## Usage

1. Open Command Prompt or PowerShell as Administrator (recommended for full device access)
2. Navigate to the project directory
3. Run the executable:

```bash
WinViewUSB.exe
```

or

```bash
make run
```

## Sample Output

```
=====================================
      USB Device Enumerator
    Windows API C Application
=====================================

Scanning for USB devices...

--- USB Device #1 ---
Device ID:      USB\VID_046D&PID_C52B\5&12345678&0&2
Description:    USB Composite Device
Manufacturer:   Logitech
Driver Version: 10.0.19041.1
Hardware ID:    USB\VID_046D&PID_C52B&REV_1200
----------------------------

--- USB Device #2 ---
Device ID:      USB\VID_8086&PID_0AE0\5&87654321&0&1
Description:    Intel(R) Wireless Bluetooth(R)
Manufacturer:   Intel Corporation
Driver Version: 22.120.0.4
Hardware ID:    USB\VID_8086&PID_0AE0&REV_0001
----------------------------

Total USB devices found: 2

Press any key to exit...
```

## File Structure

```
WinViewUSB/
├── main.c          # Main application source code
├── usb_enum.h      # Header file with function declarations
├── Makefile        # Build configuration
├── README.md       # This documentation
└── LICENSE         # License file
```

## Technical Details

The application uses the following Windows APIs:

- **SetupDi API**: For device enumeration and information retrieval
- **SetupDiGetClassDevs()**: Gets device information set for USB devices
- **SetupDiEnumDeviceInfo()**: Enumerates through devices
- **SetupDiGetDeviceRegistryProperty()**: Retrieves device properties
- **SetupDiGetDeviceInstanceId()**: Gets device instance ID

## Error Handling

The application includes comprehensive error handling:

- Validates API return values
- Displays meaningful error messages
- Proper cleanup of resources
- Graceful handling of access permissions

## Common Issues

### "Access Denied" Errors
Run the application as Administrator to access all device information.

### Missing Headers
Ensure Windows SDK is installed and properly configured.

### Compilation Errors
Make sure MinGW-w64 is installed and in your PATH.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

Copyright (c) 2025 Erdem Ersoy (eersoy93)

This project is licensed under the GPLv3 License - see the LICENSE file for details.

## Author

Created by Erdem Ersoy (eersoy93).

Created as a Windows USB device enumeration utility using C and Windows API.

