#!/bin/bash

# Script to download and set up SDL2 libraries for Windows cross-compilation

set -e  # Exit on error

echo "Setting up SDL2 libraries for Windows cross-compilation..."

# Create directory for SDL2 libraries
mkdir -p SDL2-mingw/bin
mkdir -p SDL2-mingw/lib
mkdir -p SDL2-mingw/include/SDL2

# SDL2 version numbers (update these as needed)
SDL2_VERSION="2.28.5"
SDL2_IMAGE_VERSION="2.8.2"
SDL2_TTF_VERSION="2.22.0"
SDL2_MIXER_VERSION="2.8.0"

# Download SDL2
echo "Downloading SDL2..."
if ! wget -q "https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-devel-${SDL2_VERSION}-mingw.tar.gz" -O SDL2.tar.gz; then
    echo "Failed to download SDL2!"
    exit 1
fi
tar -xzf SDL2.tar.gz
cp -r SDL2-${SDL2_VERSION}/x86_64-w64-mingw32/bin/* SDL2-mingw/bin/
cp -r SDL2-${SDL2_VERSION}/x86_64-w64-mingw32/lib/* SDL2-mingw/lib/
cp -r SDL2-${SDL2_VERSION}/x86_64-w64-mingw32/include/SDL2/* SDL2-mingw/include/SDL2/
rm -rf SDL2-${SDL2_VERSION} SDL2.tar.gz

# Download SDL2_image
echo "Downloading SDL2_image..."
wget -q "https://github.com/libsdl-org/SDL_image/releases/download/release-${SDL2_IMAGE_VERSION}/SDL2_image-devel-${SDL2_IMAGE_VERSION}-mingw.tar.gz" -O SDL2_image.tar.gz
tar -xzf SDL2_image.tar.gz
cp -r SDL2_image-${SDL2_IMAGE_VERSION}/x86_64-w64-mingw32/bin/* SDL2-mingw/bin/
cp -r SDL2_image-${SDL2_IMAGE_VERSION}/x86_64-w64-mingw32/lib/* SDL2-mingw/lib/
cp -r SDL2_image-${SDL2_IMAGE_VERSION}/x86_64-w64-mingw32/include/SDL2/* SDL2-mingw/include/SDL2/
rm -rf SDL2_image-${SDL2_IMAGE_VERSION} SDL2_image.tar.gz

# Download SDL2_ttf
echo "Downloading SDL2_ttf..."
wget -q "https://github.com/libsdl-org/SDL_ttf/releases/download/release-${SDL2_TTF_VERSION}/SDL2_ttf-devel-${SDL2_TTF_VERSION}-mingw.tar.gz" -O SDL2_ttf.tar.gz
tar -xzf SDL2_ttf.tar.gz
cp -r SDL2_ttf-${SDL2_TTF_VERSION}/x86_64-w64-mingw32/bin/* SDL2-mingw/bin/
cp -r SDL2_ttf-${SDL2_TTF_VERSION}/x86_64-w64-mingw32/lib/* SDL2-mingw/lib/
cp -r SDL2_ttf-${SDL2_TTF_VERSION}/x86_64-w64-mingw32/include/SDL2/* SDL2-mingw/include/SDL2/
rm -rf SDL2_ttf-${SDL2_TTF_VERSION} SDL2_ttf.tar.gz

# Download SDL2_mixer
echo "Downloading SDL2_mixer..."
wget -q "https://github.com/libsdl-org/SDL_mixer/releases/download/release-${SDL2_MIXER_VERSION}/SDL2_mixer-devel-${SDL2_MIXER_VERSION}-mingw.tar.gz" -O SDL2_mixer.tar.gz
tar -xzf SDL2_mixer.tar.gz
cp -r SDL2_mixer-${SDL2_MIXER_VERSION}/x86_64-w64-mingw32/bin/* SDL2-mingw/bin/
cp -r SDL2_mixer-${SDL2_MIXER_VERSION}/x86_64-w64-mingw32/lib/* SDL2-mingw/lib/
cp -r SDL2_mixer-${SDL2_MIXER_VERSION}/x86_64-w64-mingw32/include/SDL2/* SDL2-mingw/include/SDL2/
rm -rf SDL2_mixer-${SDL2_MIXER_VERSION} SDL2_mixer.tar.gz

echo "SDL2 libraries setup complete!"
echo "You can now build the Windows executable using:"
echo "  mkdir build-windows"
echo "  cd build-windows"
echo "  cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw-w64-toolchain.cmake"
echo "  make"
