#!/bin/bash

# Simple Emscripten build script for Arch Linux
# After installing: yay -S emscripten

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

print_status() {
    echo -e "${GREEN}[*]${NC} $1"
}

print_error() {
    echo -e "${RED}[!]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[!]${NC} $1"
}

# Source emscripten environment
source /etc/profile.d/emscripten.sh

# Check if Emscripten is available
if ! command -v emcc &> /dev/null; then
    print_error "Emscripten is not available!"
    print_error "Run: source /etc/profile.d/emscripten.sh"
    exit 1
fi

print_status "Emscripten found: $(emcc --version | head -1)"

# Clean and create build directory
print_status "Setting up build directory..."
rm -rf build-emscripten
mkdir -p build-emscripten

# Check if we have assets
if [ ! -d "assets" ]; then
    print_error "Assets directory not found! Make sure you have the assets folder."
    exit 1
fi

print_status "Building Atlas for WebAssembly..."

# Build command
emcc -std=c++20 \
    -I. \
    main.cpp \
    -s USE_SDL=2 \
    -s USE_SDL_IMAGE=2 \
    -s USE_SDL_TTF=2 \
    -s USE_SDL_MIXER=2 \
    -s SDL2_IMAGE_FORMATS='["png","jpg"]' \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s INITIAL_MEMORY=134217728 \
    -s FORCE_FILESYSTEM=1 \
    -s ASYNCIFY \
    --preload-file assets@assets \
    -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
    -s EXPORTED_FUNCTIONS='["_main"]' \
    -s DISABLE_EXCEPTION_CATCHING=0 \
    -O3 \
    -o build-emscripten/Atlas.html

if [ $? -eq 0 ]; then
    print_status "✅ Build successful!"
    print_status ""
    print_status "Generated files in build-emscripten/:"
    ls -la build-emscripten/
    print_status ""
    print_status "🚀 To test locally:"
    print_status "  cd build-emscripten"
    print_status "  python -m http.server 8000"
    print_status "  Open: http://localhost:8000/Atlas.html"
    print_status ""
    print_status "📦 For itch.io, zip these files:"
    print_status "  Atlas.html Atlas.js Atlas.wasm Atlas.data"
else
    print_error "❌ Build failed!"
    print_error "You may need to modify main.cpp for Emscripten compatibility."
    print_error "The main issue is usually the main loop - Emscripten needs emscripten_set_main_loop()."
    exit 1
fi
