#include "window.h"
#include "../logging/log.h"

#include "../events/application_event.h"
#include "../events/key_event.h"
#include "../events/mouse_event.h"


Window::Window()
{
}

Window::~Window()
{
    Shutdown();
}

void Window::OnUpdate()
{
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

unsigned int Window::GetWidth() const
{
    return m_Data.Width;
}

unsigned int Window::GetHeight() const
{
    return m_Data.Height;
}

GLFWwindow* Window::GetGLFWWindow() const
{
    return m_Window;
}

double Window::GetTime() const
{
    return glfwGetTime();
}

void Window::SetEventCallback(const EventCallbackFn& callback)
{
    m_EventCallback = callback;
}

void Window::Init(const WindowProps& props)
{
    m_Data.Height = props.Height;
    m_Data.Width = props.Width;
    m_Data.Title = props.Title;
    m_Data.Vsync = props.Vsync;

    if (!glfwInit())
    {
        PY_CORE_ERROR("Could not initialize GLFW!");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
        PY_CORE_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        PY_CORE_ERROR("Failed to initialize GLAD");
        return;
    }


    // Set framebuffer resize callback
    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
        });


    using namespace Event;

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int, int action, int)
        {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

            if (action == GLFW_PRESS)
            {
                KeyPressedEvent e(key, 0);
                win->m_EventCallback(e);
            }
            else if (action == GLFW_RELEASE)
            {
                KeyReleasedEvent e(key);
                win->m_EventCallback(e);
            }
        });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int)
        {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

            if (action == GLFW_PRESS)
            {
                MouseButtonPressedEvent e(button);
                win->m_EventCallback(e);
            }
            else if (action == GLFW_RELEASE)
            {
                MouseButtonReleasedEvent e(button);
                win->m_EventCallback(e);
            }
        });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
        {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent e((float)x, (float)y);
            win->m_EventCallback(e);
        });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent e((float)xOffset, (float)yOffset);
            win->m_EventCallback(e);
        });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent e;
            win->m_EventCallback(e);
        });

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowResizeEvent e(width, height);
            win->m_EventCallback(e);
        });

    PY_INFO("Window initialized: {} ({}x{})", props.Title, props.Width, props.Height);

    m_Start = glfwGetTime();
    std::cout << "Bindless supported? " << std::boolalpha << (bool)GLAD_GL_ARB_bindless_texture << std::endl;

}

void Window::Shutdown()
{
    if (m_Window)
    {
        m_Finish = glfwGetTime();
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
        glfwTerminate();
    }
}
