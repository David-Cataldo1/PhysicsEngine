#pragma once

#include <string>
#include <functional>
#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../events/event.h"

struct WindowProps
{
	std::string Title;
	unsigned int Width;
	unsigned int Height;
	bool Vsync;

	WindowProps(const std::string& title = "Test",
		unsigned int width = 1280,
		unsigned int height = 720,
		bool vsync = false)
		: Title(title), Width(width), Height(height), Vsync(vsync) {
	}
};

class Window
{
public:
	using EventCallbackFn = std::function<void(Event::Event&)>;

	Window();
	~Window();

	void OnUpdate();

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	GLFWwindow* GetGLFWWindow() const;

	double GetTime() const;

	void SetEventCallback(const EventCallbackFn& callback);

	void Init(const WindowProps& props);

	double m_Start;
	double m_Finish;


public:
	void Shutdown();


private:
	GLFWwindow* m_Window;
	EventCallbackFn m_EventCallback;

	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool Vsync;
	};

	WindowData m_Data;
};