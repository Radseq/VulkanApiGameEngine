#pragma once

#include "keycodes.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class KeysInput
{
	static void KeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonHandler(GLFWwindow* window, int button, int action, int mods);
	static void MouseMoveHandler(GLFWwindow* window, double posx, double posy);
	static void MouseScrollHandler(GLFWwindow* window, double xoffset, double yoffset);

	// Keyboard handling
	static void onKeyEvent(int key, int scancode, int action, int mods);

	static void onKeyPressed(int key, int mods);
	static void onKeyReleased(int key, int mods);

	// Mouse handling
	static void onMouseButtonEvent(int button, int action, int mods);

	static void onMousePressed(int button, int mods);
	static void onMouseReleased(int button, int mods) {};
	static void onMouseMoved(const glm::vec2& newPos);
	static void onMouseScrolled(float delta);

public:
	static void init();

	static glm::vec2 mousePos;
	static glm::vec2 oldMousePos;
	static glm::vec2 deltaPos;

	static float scrollDelta;

	static GLFWwindow* window;

	static struct {
		bool esc = false;
		bool f1 = false;
		bool f2 = false;
		bool f3 = false;
		bool f4 = false;
		bool f5 = false;
		bool a = false;
		bool b = false;
		bool d = false;
		bool f = false;
		bool g = false;
		bool l = false;
		bool n = false;
		bool o = false;
		bool p = false;
		bool s = false;
		bool t = false;
		bool w = false;
		bool space = false;
		//bool KEY_KPADD = false;
		//bool KEY_KPSUB = false;
	} keys;

	static struct {
		bool left = false;
		bool right = false;
		bool middle = false;
	} mouseButtons;
};
