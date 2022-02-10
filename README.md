# MyGame

A school project from a few years ago.

![Screenshot](/docs/game.png)

## Setup development environment

### Windows

Requirements:

- [SFML Visual C++ 15 (2017) - 32-bit](https://www.sfml-dev.org/files/SFML-2.5.0-windows-vc15-32-bit.zip)
- [GLM 0.9.9.7 (other versions might work)](https://github.com/g-truc/glm/releases/download/0.9.9.7/glm-0.9.9.7.zip)
- [GLEW 2.1.0 (other versions might work)](https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download)

Set `GLEW_ROOT`, `SFML_DIR` and `GLM_ROOT_DIR`.
We'll extract these files to `L:\Sources`.
Feel free to place them elsewhere, but remember to change the environment variables accordingly.

For example:

- GLEW_ROOT = `L:\Sources\glew-2.1.0`
- SFML_DIR = `L:\Sources\SFML-2.5.0\lib\cmake\SFML`
- GLM_ROOT_DIR = `L:\Sources\glm`

Now run `setup-visual-studio.bat` and it should create `build/MyGame.sln`.  
Open this solution in Visual Studio and right click MyGame in the file explorer and click 'Set as StartUp Project'.  
You should now be able to run the game by pressing F5 or clicking on '(>) Local Windows Debugger'.

You might also need to [install OpenAL](https://openal.org/downloads/)

### Linux/macOS

```sh
# Install dependencies (for Linux)
sudo apt install libglew-dev
sudo apt install libglm-dev
sudo apt install libsfml-dev

# Run build script
./build.sh

# The build script also accepts a few (optional) arguments
./build.sh clean    # Force a clean build, can be combined with debug/release
./build.sh debug    # Debug build, the default
./build.sh release  # Release build
./build.sh help     # Print help
```

## Links

- [Other documents](docs/)
- [Canvas course](https://canvas.hu.nl/courses/7533)

## Group Members

- [ConsolePeasant92](https://github.com/ConsolePeasant92)
- [David Strootman](https://github.com/DavidStrootman)
- [Niels Post](https://github.com/Niels-Post)
- [Wouter Visser](https://github.com/Fusion86)
- [Youri de Vor](https://github.com/youridv1)
