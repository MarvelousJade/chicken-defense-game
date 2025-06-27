#!/bin/bash

# Build script for Atlas game

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[*]${NC} $1"
}

print_error() {
    echo -e "${RED}[!]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[!]${NC} $1"
}

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check for required tools
print_status "Checking for required tools..."

if ! command_exists cmake; then
    print_error "CMake is not installed. Please install it with: sudo pacman -S cmake"
    exit 1
fi

if ! command_exists x86_64-w64-mingw32-gcc; then
    print_warning "MinGW-w64 is not installed. You won't be able to build for Windows."
    print_warning "Install it with: sudo pacman -S mingw-w64-gcc"
fi

# Parse command line arguments
BUILD_TYPE="Release"
TARGET="linux"

while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --windows)
            TARGET="windows"
            shift
            ;;
        --all)
            TARGET="all"
            shift
            ;;
        --clean)
            print_status "Cleaning build directories..."
            rm -rf build-linux build-windows
            exit 0
            ;;
        --help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  --debug      Build in debug mode"
            echo "  --windows    Build for Windows only"
            echo "  --all        Build for both Linux and Windows"
            echo "  --clean      Clean build directories"
            echo "  --help       Show this help message"
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Function to build for Linux
build_linux() {
    print_status "Building for Linux (${BUILD_TYPE})..."
    
    mkdir -p build-linux
    cd build-linux
    
    cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
    
    if [ $? -eq 0 ]; then
        make -j$(nproc)
        if [ $? -eq 0 ]; then
            print_status "Linux build successful! Executable: build-linux/Atlas"
        else
            print_error "Linux build failed!"
            exit 1
        fi
    else
        print_error "CMake configuration failed for Linux!"
        exit 1
    fi
    
    cd ..
}

# Function to build for Windows
build_windows() {
    print_status "Building for Windows (${BUILD_TYPE})..."
    
    # Check if MinGW is installed
    if ! command_exists x86_64-w64-mingw32-gcc; then
        print_error "MinGW-w64 is not installed. Cannot build for Windows."
        print_error "Install it with: sudo pacman -S mingw-w64-gcc"
        exit 1
    fi
    
    # Check if SDL2 libraries are set up
    if [ ! -d "SDL2-mingw" ]; then
        print_warning "SDL2 libraries for Windows not found."
        print_status "Running setup script..."
        
        if [ ! -f "setup_windows_sdl.sh" ]; then
            print_error "setup_windows_sdl.sh not found!"
            exit 1
        fi
        
        chmod +x setup_windows_sdl.sh
        ./setup_windows_sdl.sh
        
        # Verify the download was successful
        if [ ! -f "SDL2-mingw/include/SDL2/SDL.h" ]; then
            print_error "SDL2 setup failed! SDL.h not found."
            exit 1
        fi
    else
        print_status "SDL2 libraries found in SDL2-mingw/"
    fi
    
    # Clean build directory to ensure fresh configuration
    rm -rf build-windows
    mkdir -p build-windows
    cd build-windows
    
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw-w64-toolchain.cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
    
    if [ $? -eq 0 ]; then
        make -j$(nproc)
        if [ $? -eq 0 ]; then
            print_status "Windows build successful! Executable: build-windows/Atlas.exe"
            print_status "All required DLLs have been copied to the build directory."
        else
            print_error "Windows build failed!"
            exit 1
        fi
    else
        print_error "CMake configuration failed for Windows!"
        exit 1
    fi
    
    cd ..
}

# Main build logic
case $TARGET in
    linux)
        build_linux
        ;;
    windows)
        build_windows
        ;;
    all)
        build_linux
        build_windows
        ;;
esac

print_status "Build complete!"
