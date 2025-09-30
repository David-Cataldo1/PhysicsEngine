#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "renderer/shader.h"

int main()
{
    // 1. Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // 2. Request an OpenGL 4.5 core profile context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS fix
#endif

    // 3. Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW + GLAD test", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 4. Load OpenGL function pointers with GLAD
    if (!gladLoadGL())
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // 5. Print driver info
    std::cout << "OpenGL vendor testset:   " << glGetString(GL_VENDOR) << "\n";
    std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version:  " << glGetString(GL_VERSION) << "\n";
    std::cout << message();

    // 6. Simple loop: clear the screen and poll events
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 7. Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

