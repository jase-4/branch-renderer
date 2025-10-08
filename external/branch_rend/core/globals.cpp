#include "globals.hpp"
unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 900;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

Window window;
