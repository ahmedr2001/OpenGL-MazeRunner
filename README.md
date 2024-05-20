# Maze Runner

### Brief Description 

In Maze Runner: Car Edition, players take on the role of a high-tech, remote-controlled car
equipped with advanced sensors and powers. The goal is to navigate through a series of
increasingly complex mazes and reach the finish line as quickly as possible..
Challenges: Maneuver through twisting roads, avoid traps, and outsmart adversaries blocking
your path. Each level presents new challenges, testing your thinking and reflexes.
Players can compete against others on a global leaderboard, with the fastest times earning
the highest scores.
Features:
- Customizable car: Players can customize the car's appearance and performance, with a
range of upgrades and accessories available to unlock.
Are you ready to embark on an epic journey where every twist and turn leads you closer to
victory? prepare for the ultimate maze-solving adventure!"
### Utilized Technologies and Frameworks

- C++ OpenGL Libraries, such as: GLFW, GLAD and GLM.
- GLSL.
- C++ tinyobjloader.
- CMake.
- miniaudio

### Repository Structure

**1) assets:**
- Contains the 3d models, textures and implemented shaders for rendering and effects.

**2) common:**
- Contains the main classes of the program, such as: framebuffers, controllers, mesh drawing, ...etc.

**3) src:**
- Contains the class of the main scene, where everything is put together and the main entry point of the program.

**4) vendor:**
- Contains the sources of some libraries used in the program.

**5) CMakeLists.txt:**
- The text file for building the game using CMake.

### Implemented Shaders

- Texture Mapping.
- Sky Texture, using Volume Texture Mapping.
- post-processing effects

### Instructions

1) Make sure your graphics drivers are installed.
2) Install OpenGL on your machine.
3) Run CMakeLists.txt file using CMake, along with preferred C++ Compiler.

Have a nice day!
