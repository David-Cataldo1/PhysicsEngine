#include "Input.h"

std::unordered_map<int, KeyState> Input::s_KeyStates;
std::unordered_map<int, bool> Input::s_MouseButtonStates;
glm::vec2 Input::s_MousePosition = { 0.0f, 0.0f };
glm::vec2 Input::s_MouseScrollOffset = { 0.0f, 0.0f };

void Input::Init()
{
	s_KeyStates.clear();
	s_MouseButtonStates.clear();
}

void Input::OnUpdate()
{
	for (auto& [key, state] : s_KeyStates) {
		if (state == KeyState::Pressed)
			state = KeyState::Held;
	}
}

void Input::SetKeyState(int key, bool pressed)
{
	if (pressed)
	{
		if (s_KeyStates[key] != KeyState::Held)
			s_KeyStates[key] = KeyState::Pressed;
	}
	else
	{
		s_KeyStates[key] = KeyState::Released;
	}
}

bool Input::IsKeyPressed(int key)
{
	return s_KeyStates[key] == KeyState::Pressed;
}

bool Input::IsKeyDown(int key)
{
	return !(s_KeyStates[key] == KeyState::Released);
}

bool Input::IsKeyHeld(int key)
{
	return s_KeyStates[key] == KeyState::Held;
}

void Input::SetMouseButton(int button, bool pressed)
{
	s_MouseButtonStates[button] = pressed;
}

void Input::SetMousePosition(glm::vec2 pos)
{
	s_MousePosition = pos;
}

void Input::SetMouseScrollOffset(glm::vec2 offset)
{
	s_MouseScrollOffset = offset;
}

bool Input::IsMouseDown(int button)
{
	return s_MouseButtonStates[button];
}

glm::vec2 Input::GetMousePos()
{
	return s_MousePosition;
}

glm::vec2 Input::GetMouseScrollOffset()
{
	return s_MouseScrollOffset;
}
