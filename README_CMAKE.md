# Building Atlas Game with CMake

This guide explains how to build the Atlas game for both Linux and Windows using CMake on Arch Linux.

## Prerequisites

### For Linux builds:
```bash
sudo pacman -S cmake sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

### For Windows cross-compilation:
```bash
sudo pacman -S cmake mingw-w64-gcc wget
```

## Quick Start

1. Make the scripts executable:
```bash
chmod +x build.sh setup_windows_sdl.sh
```

2. Build for Linux:
```bash
./build.sh
```

3. Build for Windows:
```bash
./build.sh --windows
```

4. Build for both platforms:
```bash
./build.sh --all
```

## Build Options

- `--debug`: Build in debug mode
- `--windows`: Build for Windows only
- `--all`: Build for both Linux and Windows
- `--clean`: Clean all build directories
- `--help`: Show help message

## Manual Building

### Linux Build

```bash
mkdir build-linux
cd build-linux
cmake ..
make -j$(nproc)
```

### Windows Build

1. First, download the SDL2 libraries:
```bash
./setup_windows_sdl.sh
```

2. Then build:
```bash
mkdir build-windows
cd build-windows
cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw-w64-toolchain.cmake
make -j$(nproc)
```

## Output

- **Linux**: `build-linux/Atlas`
- **Windows**: `build-windows/Atlas.exe` (with all required DLLs)

## Distribution

For Windows distribution, you need to include:
- `Atlas.exe`
- All `.dll` files from the build-windows directory
- The `assets` folder

You can create a distribution package:
```bash
cd build-windows
mkdir Atlas-Windows
cp Atlas.exe *.dll Atlas-Windows/
cp -r assets Atlas-Windows/
zip -r Atlas-Windows.zip Atlas-Windows/
```

## Troubleshooting

### SDL2 Headers Not Found
Make sure SDL2 development packages are installed:
```bash
sudo pacman -S sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

### MinGW Not Found
Install MinGW-w64:
```bash
sudo pacman -S mingw-w64-gcc
```

### Windows Executable Crashes
Ensure all DLL files are in the same directory as the executable. The build script automatically copies all required DLLs.

### Audio/Font Issues on Windows
Some Windows systems might need additional runtime libraries. The setup script downloads all necessary SDL2 dependencies.

## Project Structure

```
Atlas/
├── CMakeLists.txt              # CMake configuration
├── mingw-w64-toolchain.cmake   # Cross-compilation toolchain
├── setup_windows_sdl.sh        # SDL2 setup script
├── build.sh                    # Build automation script
├── main.cpp                    # Main source file
├── *.h                         # Header files
├── assets/                     # Game assets
├── build-linux/                # Linux build output
├── build-windows/              # Windows build output
└── SDL2-mingw/                 # Windows SDL2 libraries
