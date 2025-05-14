#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "camera.hpp"
#include "window.hpp"



// Declare variables using extern (no initialization here)
extern const unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

extern float lastX;
extern float lastY;
extern bool firstMouse;

// timing
extern float deltaTime;
extern float lastFrame;

extern Camera camera;
extern Window window;

#endif
