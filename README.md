# OpenGL-PerlinNoise

## Description

This repository explores the use of Perlin Noise in OpenGL, potentially as a technique to create terrain for a project. While originally envisioned for a project called "Vaat Disu De" (a story of a small honeybee and its mother), the core focus here is the implementation and demonstration of Perlin Noise generation within an OpenGL environment. "Vaat Disu De" aimed to use OpenGL (likely the fixed-function pipeline, FFP) to tell the story of a honeybee searching for a new home. Although the original "Vaat Disu De" demo had specific artistic goals, this repository serves as a technical showcase of Perlin Noise, which could be used in similar visual storytelling or world-building scenarios.

##### Perlin Noise Demo

[![Video Thumbnail](https://img.youtube.com/vi/i7GgMPFLzoI/hqdefault.jpg)](https://www.youtube.com/watch?v=i7GgMPFLzoI)

## Folder Structure

The repository structure is organized as follows:

*   `.gitignore`: Specifies intentionally untracked files that Git should ignore.
*   `MyIcon.ico`: Application icon file (Windows icon).
*   `MyMath.h`: C++ header file. Likely contains custom mathematical functions or data structures used in the project.
*   `OGL.cpp`: Main C++ source code file. Contains the core program logic and likely the `main()` function.
*   `OGL.h`: C++ header file. Might contain OpenGL-specific declarations or class definitions.
*   `OGL.rc`: Resource file.  This Windows resource file is used to embed application icons, version information, and other resources into the executable.
*   `OGLres`: (Likely a directory, but hard to be certain from the screenshot). If it's a directory, then it will contain resources used by the windows system and the OpenGL application.
*   `Terrain.cpp`: C++ source code file. Likely contains the implementation of the terrain generation using Perlin Noise.
*   `Utilities.h`: C++ header file. May contain helper functions or utility classes.
*   `build.bat`: Batch script for building the project on Windows. Automates the compilation process.
*   `camera.h`: Contains an implementation for camera logic.
*   `git_push.bat`: Batch script for pushing changes to the Git repository. A convenience script for streamlining Git operations.
*   `vmath.h`: C++ header file. Could contain vector and matrix math functions.

## Dependencies

Before building and running the application, ensure you have the following dependencies installed and configured:

*   **GLEW:** OpenGL Extension Wrangler Library. Download from [https://glew.sourceforge.net/](https://glew.sourceforge.net/)

**Installation and Configuration:**

1.  Download the appropriate versions of GLEW for your operating system and Visual Studio version.
2.  **GLEW:** Extract the downloaded archives. Copy the include files (`.h`) to a directory where Visual Studio can find them. Copy the library files (`.lib`) to a suitable library directory.

## Build Instructions (Using build.bat)

This project includes a `build.bat` batch script for easy compilation on Windows.

1.  **Clone the Repository:** Clone this repository to your local machine using the following command:

    ```bash
    git clone https://github.com/Tushar-Wagdare/OpenGL-PerlinNoise.git
    ```

2.  **Navigate to the Repository Directory:** Open a command prompt or PowerShell window and navigate to the directory where you cloned the repository.

    ```bash
    cd OpenGL-PerlinNoise
    ```

3.  **Run the Build Script:** Execute the `build.bat` script. This will compile the project.

    ```bash
    build.bat
    ```

    **Note:** The `build.bat` script assumes that you have a suitable C++ compiler (e.g., Visual Studio's command-line tools) installed and configured in your environment.

4.  **Run the Solution:** After the build completes successfully, the executable will be located in the `Debug` or `Release` directory (depending on the build configuration specified in the `build.bat` script). Run the executable.

