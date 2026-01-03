# A Naive Attempt at Playing God — or simply, a Physics Engine from Scratch

I’ve always wanted to derive equations that explain why orbits collapse into elliptical paths rather than perfect circles, but I never quite got there. Now that I know how to code a little, I decided to cheat 😉. This project is essentially a C++ based small physics engine built from **scratch**, coupled with a rendering pipeline to visualize dynamic systems.

<br>

<div align="center" style="display: block;">
  <div style="width: 80%;">
    <img src="./demo/Orbits.gif" alt="Orbits Demo" width="100%">
    <p>
      <strong>
        All planets are initialized with different (random) velocities and evolve under Newtonian gravity.
        As a result, they naturally follow elliptical orbits rather than circular ones.
      </strong>
    </p>
  </div>
</div>



## Tech Stack

- **C++** for the core physics engine and rendering pipeline, written in an object-oriented style  
- **OpenGL** for graphics rendering  
- **GLSL** for vertex and fragment shaders  
- **GLM** for vector and matrix mathematics  
- Graphics pipeline structured according to standard OpenGL practices  
- **CMake** for build configuration and project setup  

## Far from Over!

As of 12/25, the engine can simulate a simple solar system with multiple planets interacting via gravity. There’s still a lot of room to improve both the physics accuracy and the overall architecture.

### To Do
- Add collision handling  
- Better abstractions for forces and bodies  
- Performance optimizations  

---

## Do-It-Yourself

## Wanna Start?

If you’re interested in building something similar, these resources are great starting points:

- [Generating vertices for spheres](https://www.songho.ca/opengl/gl_sphere.html)  
- [OpenGL Basics](https://learnopengl.com/Getting-started/OpenGL)  
- [Collision Detection Algorithms and More](https://winter.dev/articles/physics-engine)  

## Still Lacking Motivation?

- [Simulating Black Holes in C++ — kavan](https://www.youtube.com/watch?v=8-B6ryuBkCM)  
- [Designing a Physics Engine - Iain Winter](https://winter.dev/articles/physics-engine)

---
