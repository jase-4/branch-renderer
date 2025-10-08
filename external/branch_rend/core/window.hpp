#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Window {
public:
    GLFWwindow* window;  
    int width;
    int height;
    double xpos; 
    double ypos;
    double xoffset;
    double yoffset;

    Window();
    ~Window();

    int init();
    int should_close();
    void process_input();
    void swap_buffers();
    void poll_events();
    void terminate();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif // WINDOW_HPP
