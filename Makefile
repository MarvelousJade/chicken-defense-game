CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra
LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer
DEBUG_FLAGS = -g -O0 -DDEBUG

SOURCES = main.cpp

HEADERS = Animation.h Atlas.h Bullet.h Camera.h Chicken.h ChickenFast.h ChickenMedium.h ChickenSlow.h Timer.h Vector2.h

OBJECTS = $(addprefix build/, $(SOURCES:.cpp=.o))
EXECUTABLE = atlas

all: build/$(EXECUTABLE)

build/$(EXECUTABLE): $(OBJECTS) | build
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

build/%.o: %.cpp $(HEADERS) | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

debug: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(SOURCES) -o build/$(EXECUTABLE)_debug $(LDFLAGS)

clean:
	rm -f $(OBJECTS) build/$(EXECUTABLE) build/$(EXECUTABLE)_debug

run:
	./build/$(EXECUTABLE)

.PHONY: all clean run debug
