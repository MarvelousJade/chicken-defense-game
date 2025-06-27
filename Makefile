CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
LDFLAGS_DYNAMIC = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer
LDFLAGS_STATIC = -static-libgcc -static-libstdc++ -Wl,-Bstatic -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lfreetype -lpng -ljpeg -lz -lvorbisfile -lvorbis -logg -lFLAC -lmpg123 -lopus -lopusfile -Wl,-Bdynamic -ldl -lpthread -lm -lasound -lpulse -lpulse-simple -lX11 -lXext -lXrandr -lXi -lXfixes -lXcursor -lXss

SOURCES = main.cpp
HEADERS = Animation.h Atlas.h Bullet.h Camera.h Chicken.h ChickenFast.h ChickenMedium.h ChickenSlow.h Timer.h Vector2.h
OBJECTS = $(addprefix build/, $(SOURCES:.cpp=.o))
EXECUTABLE = atlas

all: build/$(EXECUTABLE)

# Dynamic linking (for development)
build/$(EXECUTABLE): $(OBJECTS) | build
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS_DYNAMIC)

# Static linking (for distribution) - may not work on all systems
static: $(OBJECTS) | build
	$(CXX) $(OBJECTS) -o build/$(EXECUTABLE)_static $(LDFLAGS_STATIC)

# AppImage-ready build (recommended for distribution)
portable: $(OBJECTS) | build
	$(CXX) $(OBJECTS) -o build/$(EXECUTABLE)_portable $(LDFLAGS_DYNAMIC) -Wl,-rpath,'$$ORIGIN/lib'

build/%.o: %.cpp $(HEADERS) | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

# Create distribution package
dist: build/$(EXECUTABLE)
	@echo "Creating distribution package..."
	@mkdir -p dist/atlas-game
	@cp build/$(EXECUTABLE) dist/atlas-game/
	@cp -r assets dist/atlas-game/ 2>/dev/null || echo "Warning: assets folder not found"
	@echo '#!/bin/bash' > dist/atlas-game/run.sh
	@echo 'cd "$$(dirname "$$0")"' >> dist/atlas-game/run.sh
	@echo 'export LD_LIBRARY_PATH="./lib:$$LD_LIBRARY_PATH"' >> dist/atlas-game/run.sh
	@echo './$(EXECUTABLE)' >> dist/atlas-game/run.sh
	@chmod +x dist/atlas-game/run.sh
	@echo "Distribution package created in dist/atlas-game/"

# Create distribution with bundled libraries
dist-bundle: build/$(EXECUTABLE)
	@echo "Creating distribution package with bundled libraries..."
	@mkdir -p dist/atlas-game/lib
	@cp build/$(EXECUTABLE) dist/atlas-game/
	@cp -r assets dist/atlas-game/ 2>/dev/null || echo "Warning: assets folder not found"
	@echo "Copying SDL2 libraries..."
	@cp /usr/lib/libSDL2-2.0.so.0 dist/atlas-game/lib/ 2>/dev/null || echo "Warning: libSDL2 not found"
	@cp /usr/lib/libSDL2_ttf-2.0.so.0 dist/atlas-game/lib/ 2>/dev/null || echo "Warning: libSDL2_ttf not found"
	@cp /usr/lib/libSDL2_image-2.0.so.0 dist/atlas-game/lib/ 2>/dev/null || echo "Warning: libSDL2_image not found"
	@cp /usr/lib/libSDL2_mixer-2.0.so.0 dist/atlas-game/lib/ 2>/dev/null || echo "Warning: libSDL2_mixer not found"
	@echo '#!/bin/bash' > dist/atlas-game/run.sh
	@echo 'cd "$$(dirname "$$0")"' >> dist/atlas-game/run.sh
	@echo 'export LD_LIBRARY_PATH="./lib:$$LD_LIBRARY_PATH"' >> dist/atlas-game/run.sh
	@echo './$(EXECUTABLE)' >> dist/atlas-game/run.sh
	@chmod +x dist/atlas-game/run.sh
	@echo "Distribution package with bundled libraries created in dist/atlas-game/"

# Create zip for itch.io upload
zip: dist-bundle
	@cd dist && zip -r atlas-game-linux.zip atlas-game/
	@echo "Created dist/atlas-game-linux.zip ready for itch.io upload"

clean:
	rm -rf build/ dist/
	rm -f $(OBJECTS) build/$(EXECUTABLE) build/$(EXECUTABLE)_static build/$(EXECUTABLE)_portable

.PHONY: all clean static portable dist dist-bundle zip
