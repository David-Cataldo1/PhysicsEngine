#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

enum class KeyState
{
	Released = 0,
	Pressed,
	Held
};

class Input
{
public:
	static void Init();
	static void OnUpdate();

	static void SetKeyState(int key, bool pressed);

	static bool IsKeyPressed(int key);
	static bool IsKeyDown(int key);
	static bool IsKeyHeld(int key);


	static void SetMouseButton(int button, bool pressed);
	static void SetMousePosition(glm::vec2 pos);
	static void SetMouseScrollOffset(glm::vec2 offset);

	static bool IsMouseDown(int button);
	static glm::vec2 GetMousePos();
	static glm::vec2 GetMouseScrollOffset();


private:
	static std::unordered_map<int, KeyState> s_KeyStates;
	static std::unordered_map<int, bool> s_MouseButtonStates;
	static glm::vec2 s_MousePosition;
	static glm::vec2 s_MouseScrollOffset;

};