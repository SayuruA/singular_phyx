# A Naive Attempt at Playing God — or Simply a Physics Engine from Scratch

I’ve always wanted to derive equations that explain why orbits collapse into elliptical paths rather than perfect circles, but I never quite got there. Now that I know how to code a little, I decided to cheat 😉.

![Demo video](./demo/Orbits.gif)

## Description

All planets are initialized with different (random) velocities and evolve under Newtonian gravity. As a result, they naturally follow elliptical orbits rather than circular ones.

This project is essentially a small physics engine built from scratch, coupled with a minimal rendering pipeline to visualize orbital motion.

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
- Improve numerical stability and integration methods  
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

## Lacking Motivation?

- [Simulating Black Holes in C++ — kavan](https://www.youtube.com/watch?v=8-B6ryuBkCM)  
- [Designing a Physics Engine](https://winter.dev/articles/physics-engine)

---
