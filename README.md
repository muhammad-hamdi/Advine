# AdventureEngine

AdventureEngine is a WIP C++ game engine for my personal use in game development.

---

## Features

- Scene-graph system with Component-based game entities
- OpenGL renderer (for now)
- Assimp-based model loading
- In-editor scene hierarchy and entity inspector
- JSON-based scene serialization
- ImGui-based editor

---

## Build Instructions

### Prerequisites

- **CMake** ≥ 3.16
- **Visual Studio 2022** or compatible C++17 compiler
- **Git** (for cloning submodules / dependencies)

### Dependencies (automatically fetched via CMake)

- GLFW
- GLAD
- ImGui
- glm
- stb
- nlohmann/json
- Assimp

### Build Steps (Windows)

1. Clone the repository:
    ```bash
    git clone https://github.com/muhammad-hamdi/AdventureEngine.git
    cd AdventureEngine
    ```

2. Generate and build the project using CMake:
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build . --config Debug
    ```

3. Run the executable:
    ```bash
    .\build\Debug\AdventureEngine.exe
    ```

> Note: When running from VS or VSCode set the working directory to be the root of the repo since asset paths are resolved relative to root dir.

---

## Project Structure

```
AdventureEngine/
├── src/ # Engine source code
│ ├── core/ # Game and scene logic
│ ├── rendering/ # OpenGL renderer and GPU resource classes
│ ├── components/ # component class sources
│ └── assets/ # resource management classes (Textures, models, shaders)
├── assets/ # Textures, models, shaders
├── CMakeLists.txt # CMake build configuration
└── README.md
```


---

## Roadmap

- [ ] Material asset system and file browser
- [ ] Animation system
- [ ] Physics system
- [ ] Global Illumionation and PBR
- [ ] Path Tracing/Ray Tracing (lol, maybe)
- [ ] Better runtime/editor state separation
- [ ] Input and platform layer abstraction
- [ ] Abstract graphics backend for Vulkan/D3D support

---

## License

MIT – do whatever you want, just don't claim it's yours 😄