#include "KeysInput.hpp"

void KeysInput::init()
{
	glfwSetKeyCallback(window, KeyboardHandler);
	glfwSetMouseButtonCallback(window, MouseButtonHandler);
	glfwSetCursorPosCallback(window, MouseMoveHandler);
	glfwSetScrollCallback(window, MouseScrollHandler);

	KeysInput::mousePos = glm::vec2(0,0);
	KeysInput::oldMousePos = glm::vec2(0, 0);
	KeysInput::deltaPos = glm::vec2(0, 0);

	KeysInput::scrollDelta = 0.0f;
}

void KeysInput::KeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//DeviceHandler* example = (DeviceHandler*)glfwGetWindowUserPointer(window);
	onKeyEvent(key, scancode, action, mods);
}

void KeysInput::MouseButtonHandler(GLFWwindow* window, int button, int action, int mods) {
	//DeviceHandler* example = (DeviceHandler*)glfwGetWindowUserPointer(window);
	onMouseButtonEvent(button, action, mods);
}

void KeysInput::MouseMoveHandler(GLFWwindow* window, double posx, double posy) {
	//DeviceHandler* example = (DeviceHandler*)glfwGetWindowUserPointer(window);
	onMouseMoved(glm::vec2(posx, posy));
}

void KeysInput::MouseScrollHandler(GLFWwindow* window, double xoffset, double yoffset) {
	//DeviceHandler* example = (DeviceHandler*)glfwGetWindowUserPointer(window);
	onMouseScrolled((float)yoffset);
}


void KeysInput::onKeyEvent(int key, int scancode, int action, int mods) {
	switch (action) {
	case GLFW_PRESS:
		onKeyPressed(key, mods);
		break;

	case GLFW_RELEASE:
		onKeyReleased(key, mods);
		break;

	default:
		break;
	}
}

void KeysInput::onMouseButtonEvent(int button, int action, int mods) {
	switch (action) {
	case GLFW_PRESS:
		onMousePressed(button, mods);
		break;

	case GLFW_RELEASE:
		onMouseReleased(button, mods);
		break;

	default:
		break;
	}
}

void KeysInput::onMousePressed(int button, int mods)
{
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		KeysInput::mouseButtons.left = GLFW_PRESS;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		KeysInput::mouseButtons.right = GLFW_PRESS;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		KeysInput::mouseButtons.middle = GLFW_PRESS;
		break;
	}
}

void KeysInput::onMouseScrolled(float delta)
{
	KeysInput::scrollDelta = delta;
}

void KeysInput::onMouseMoved(const glm::vec2 & newPos)
{
	glm::vec2 deltaPos = KeysInput::mousePos - newPos;

	KeysInput::oldMousePos = mousePos;
	KeysInput::mousePos = newPos;
}


void KeysInput::onKeyPressed(int key, int mods)
{
	switch (key) {
	case KEY_W:
		KeysInput::keys.w = true;
		break;
	case KEY_S:
		KeysInput::keys.s = true;
		break;
	case KEY_A:
		KeysInput::keys.a = true;
		break;
	case KEY_D:
		KeysInput::keys.d = true;
		break;

	case KEY_P:
		KeysInput::keys.p = true;
		//keyPaused = !keyPaused;
		break;

	case KEY_F1:
		KeysInput::keys.f1 = true;
		//uiVisibled = !uiVisibled;
		break;

	case KEY_ESCAPE:
		KeysInput::keys.esc = true;
		break;

	default:
		break;
	}
}

void KeysInput::onKeyReleased(int key, int mods)
{
	switch (key) {
	case KEY_W:
		KeysInput::keys.w = false;
		break;
	case KEY_S:
		KeysInput::keys.s = false;
		break;
	case KEY_A:
		KeysInput::keys.a = false;
		break;
	case KEY_D:
		KeysInput::keys.d = false;
		break;

	case KEY_P:
		KeysInput::keys.p = false;
		//keyPaused = !keyPaused;
		break;

	case KEY_F1:
		KeysInput::keys.f1 = false;
		//uiVisibled = !uiVisibled;
		break;

	case KEY_ESCAPE:
		KeysInput::keys.esc = false;
		break;

	default:
		break;
	}
}