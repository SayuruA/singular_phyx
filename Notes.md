## DLL (.dll) Files vs. Static Library (.a or .lib) Files

Both DLL and static library files are **binary files**. Here’s what that means and how they differ:

### What is a Binary File?
A binary file stores data in a format not directly readable by humans. It typically contains **machine code**—instructions that a computer’s processor can execute.

### DLLs (Dynamic Link Libraries)
- **Windows:** `.dll` files  
- **Linux/Unix:** `.so` (Shared Object) files  
- **Purpose:** Contain compiled code and data for **dynamic linking** at runtime.
- **How they work:** The code is loaded and linked to an application when it runs, or even later during its operation.
- **Format:** Compiled into machine-readable binary.

### Static Libraries
- **Windows:** `.lib` files  
- **Unix-like systems:** `.a` files  
- **Purpose:** Contain compiled code and data for **static linking** during compilation.
- **How they work:** The code is copied and embedded into the final executable during the linking phase, resulting in a larger executable that does **not** depend on external library files at runtime.
- **Format:** Also compiled into machine-readable binary.

### Polymorphism - Miscellaneous
- Use virtual and default on base class destructors. This will ensure derived class destructors are called correctly, from top-most derived class down to base class.

---

**Summary:**  
Both DLLs and static libraries contain pre-compiled, machine-readable instructions, making them binary files. The main difference is **when and how** their code is linked to your application.


## Understanding `gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);`

* **`glfwGetProcAddress`** → a *function* provided by GLFW that, given a string like `"glGenBuffers"`, **returns the function pointer** to that OpenGL call from the graphics driver.

* **`GLADloadproc`** → a *typedef for a function pointer type*. GLAD wants “a function that, given a name, returns the address of that OpenGL function.” It doesn’t care where it comes from.

* **`gladLoadGLLoader`** → the *initializer function* in GLAD. It takes a `GLADloadproc` function pointer (in this case, `glfwGetProcAddress`), then **queries and stores all the OpenGL function pointers** so you can use them later in your program.

* The **cast** `(GLADloadproc)glfwGetProcAddress` is just a way to tell the compiler: “yes, `glfwGetProcAddress` matches the signature GLAD expects — treat it as such.”

So the workflow is:

```
Your code → gladLoadGLLoader → uses glfwGetProcAddress → GPU driver → returns real OpenGL function addresses
```

