# test_1

## Build Instructions

1. **Install Dependencies**
   - Make sure you have [GLFW](https://www.glfw.org/) and [GLAD](https://glad.dav1d.de/) downloaded.
   - Place the GLAD include directory at the top of your project's include path.

2. **Compile**
   - Open a terminal in `src` folder.
   - Use the following command to build for MinGW:

     ```sh
     cmake .. -G "MinGW Makefiles"
     ```
   - Then compile with:
     ```sh
     mingw32-make
     ```

   - Adjust the paths as needed for your setup.

3. **Run**
   - Execute the program:
     ```sh
     .\test.exe
     ```

## Misc Instructions

- **GLAD Include**: Always keep the GLAD include (`#include <glad/glad.h>`) at the very top of your source files before any other OpenGL headers.
- **GLFW**: Make sure the GLFW DLL is available in your executable's directory if using the pre-built binaries.
- **Debugging**: Use `-g` flag with `g++` for debug builds.
- **Cleaning**: Remove build artifacts with:
  ```sh
  del test.exe
  ```

---