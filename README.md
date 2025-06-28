# 🐔 Chicken Defense Game

A fast-paced arcade shooter where you defend your base from waves of invading chickens! Built with C++ and SDL2.

## 📹 Gameplay Preview

<div align="center">
  
  [![Gameplay Video](https://img.youtube.com/vi/ahL7-md2M_8/maxresdefault.jpg)](https://www.youtube.com/watch?v=ahL7-md2M_8)
  
  <!-- ![Gameplay GIF](./assets/gameplay.gif) -->
  
</div>

> Click the image above to watch the gameplay video!

## 🎮 Features

- **Three Types of Enemies**: Face off against Slow, Medium, and Fast chickens, each with unique speed and animations
- **Dynamic Difficulty**: Enemy spawn rate increases over time to keep you on your toes
- **Explosive Effects**: Satisfying explosion animations and sound effects when chickens are defeated
- **Score System**: Track your high score as you defend against the chicken invasion
- **Health System**: You have 10 hearts - lose one each time a chicken reaches the bottom
- **Camera Shake**: Feel the impact of your shots with screen shake effects
- **Background Music & SFX**: Immersive audio experience with background music and multiple sound effects

## 🛠️ Prerequisites

Before building the game, ensure you have the following installed:

- **C++ Compiler**: GCC with C++20 support
- **SDL2**: Core SDL2 library
- **SDL2_image**: For loading PNG/JPG textures
- **SDL2_ttf**: For rendering text
- **SDL2_mixer**: For audio playback
- **Make**: Build automation tool

### Installing Dependencies (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

### Installing Dependencies (macOS with Homebrew)
```bash
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

## 🏗️ Building the Game

1. Clone the repository:
```bash
git clone https://github.com/yourusername/chicken-defense.git
cd chicken-defense
```

2. Build the game using Make:
```bash
make
```

3. For debug build with debugging symbols:
```bash
make debug
```

4. To clean build files:
```bash
make clean
```

## 🚀 Running the Game

After building, run the game with:
```bash
make run
```

Or directly:
```bash
./build/atlas
```

## 🎯 How to Play

1. **Objective**: Defend your base by shooting down chickens before they reach the bottom of the screen
2. **Aim**: Move your mouse to aim the cannon
3. **Shoot**: Click to fire bullets at the chickens
4. **Survive**: You start with 10 hearts. Each chicken that reaches the bottom costs you 1 heart
5. **Score**: Earn points for each chicken you eliminate

## 🕹️ Controls

| Control | Action |
|---------|--------|
| Mouse Movement | Aim the cannon |
| Left Mouse Button | Fire bullets |
| ESC / Close Window | Quit game |


## 🔧 Technical Details

- **Language**: C++20
- **Graphics**: SDL2 with hardware-accelerated rendering
- **Frame Rate**: 144 FPS target
- **Resolution**: 1280x720
- **Architecture**: Object-oriented design with separate classes for game entities

### Key Components

- **Animation System**: Frame-based animation with customizable timing
- **Atlas System**: Efficient texture loading and management
- **Timer System**: Flexible timer implementation for game events
- **Camera System**: Includes shake effects for impact feedback
- **Entity Management**: Polymorphic chicken types with inheritance

## 🎨 Adding Custom Assets

To modify game assets:

1. Replace the PNG files in the `assets/` folder with your own
2. Maintain the same file naming convention for animations
3. Keep the same dimensions for proper rendering

## 🐛 Troubleshooting

**Game won't compile**: Ensure all SDL2 libraries are properly installed

**No audio**: Check that SDL2_mixer is installed and audio files are in the assets folder

**Performance issues**: Try reducing the window size or checking your graphics drivers

## 📝 License

This project is open source. Feel free to modify and distribute as needed.

## 🤝 Contributing

Contributions are welcome! Feel free to submit issues or pull requests.

---

*Happy chicken hunting! 🎯🐔*
