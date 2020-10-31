#include "Window.hpp"

bool Window::init( ) { return GLFW_TRUE == glfwInit( ); }

Window::Window( ) { glfwInit( ); }

void Window::destroy( ) {
    glfwDestroyWindow (window);
    glfwTerminate( );
    window = nullptr;
}

bool Window::isKeyPressed (int key) const { return glfwGetKey (window, key) == GLFW_PRESS; }

GLFWwindow* Window::getWindow( ) const { return window; }

void Window::setWindowSize (glm::ivec2& newSize) const { glfwGetWindowSize (window, &newSize.x, &newSize.y); }
int  Window::windowShouldClose( ) const { return glfwWindowShouldClose (window); }
void Window::setWindowShouldClose( ) const { glfwSetWindowShouldClose (window, 1); }

void Window::glPollEvents( ) { glfwPollEvents( ); }

const vk::SurfaceKHR& Window::getSurface( ) const { return cppSurface; }

std::vector<std::string> Window::getRequiredInstanceExtensions( ) {
    std::vector<std::string> result;
    uint32_t                 count = 0;
    const char**             names = glfwGetRequiredInstanceExtensions (&count);
    if (names && count) {
        for (size_t i = 0; i < count; ++i) { result.emplace_back (names [i]); }
    }

#ifdef NDEBUG

#else
    result.emplace_back (VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    return result;
}

/*
std::vector<std::string> Window::getRequiredInstanceExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef NDEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        return extensions;
}*/

void Window::createWindowSurface (const vk::Instance& instance, const vk::AllocationCallbacks* pAllocator) {
    const auto result = static_cast<vk::Result> (glfwCreateWindowSurface (
        VkInstance (instance), window, reinterpret_cast<const VkAllocationCallbacks*> (pAllocator), &rawSurface));

    if (result != vk::Result::eSuccess) { throw std::runtime_error ("failed to create window surface!"); }

    vk::createResultValue (result, rawSurface, "vk::CommandBuffer::begin");

    cppSurface = vk::SurfaceKHR (rawSurface);
}

void Window::createWindow (const bool& fullscreen, const std::string& windowTitle, const glm::uvec2& windowSize,
                           const glm::ivec2& position) {
    glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
    auto* const monitor = glfwGetPrimaryMonitor( );

    if (fullscreen) {
        const auto mode = glfwGetVideoMode (monitor);
        window          = glfwCreateWindow (mode->width, mode->height, windowTitle.c_str( ), monitor, nullptr);
    } else {
        size.width  = windowSize.x;
        size.height = windowSize.y;
        window      = glfwCreateWindow (windowSize.x, windowSize.y, windowTitle.c_str( ), nullptr, nullptr);
        if (position != glm::ivec2 {INT_MIN, INT_MIN}) { glfwSetWindowPos (window, position.x, position.y); }
    }

    glfwSetWindowUserPointer (window, this);
    glfwSetWindowCloseCallback (window, closeHandler);
    glfwSetFramebufferSizeCallback (window, frameBufferSizeHandler);

    glfwSetKeyCallback (window, keyboardHandler);
    glfwSetMouseButtonCallback (window, mouseButtonHandler);
    glfwSetCursorPosCallback (window, mouseMoveHandler);
    glfwSetScrollCallback (window, mouseScrollHandler);

    // glfwSetWindowUserPointer(window, &deviceHandler);

    if (!window) { throw std::runtime_error ("Could not create window"); }
}

vk::Extent2D Window::getExtent2DWindowSize( ) const { return size; }

void Window::setExtent2DWindowSize (const vk::Extent2D& setSize) {
    // ReSharper disable once CppInconsistentNaming
    glm::ivec2 tempSize {setSize.width, setSize.height};
    setWindowSize (tempSize);
    size = setSize;
}

void Window::setTitle (const std::string& deviceName, const uint32_t& frameCounter) const {
    const std::string windowTitle = "Game - " + deviceName + " - " + std::to_string (frameCounter) + " fps";

    glfwSetWindowTitle (window, windowTitle.c_str( ));
}

void Window::setSizeLimits (const glm::uvec2& minSize, const glm::uvec2& maxSize) const {
    glfwSetWindowSizeLimits (window, minSize.x, minSize.y, (maxSize.x != 0) ? maxSize.x : minSize.x,
                             (maxSize.y != 0) ? maxSize.y : minSize.y);
}

void Window::onWindowResized (const glm::uvec2& newSize) { isWindowResize = true; }

void Window::windowResize( ) {
    int width = 0, height = 0;

    glfwGetFramebufferSize (window, &width, &height);

    isWindowResize = false;
    size           = vk::Extent2D (width, height);
}

bool Window::isWindowResized( ) { return isWindowResize; }

const VkSurfaceKHR& Window::getCSurface( ) const { return rawSurface; }
const glm::vec2&    Window::getMousePos( ) const { return mousePos; }
const glm::vec2&    Window::getDeltaPos( ) const { return deltaPos; }
float&              Window::getZoomSpeed( ) { return zoomSpeed; }

glm::vec2 Window::getWindowSize( ) const {
    int width;
    int height;
    glfwGetWindowSize (window, &width, &height);
    glm::vec2 size {width, height};
    return size;
}

void Window::waitEvents( ) { glfwWaitEvents( ); }

void Window::showWindow (bool show) const {
    if (show) {
        glfwShowWindow (window);
    } else {
        glfwHideWindow (window);
    }
}

void Window::closeHandler (GLFWwindow* window) {
    Window* example = (Window*) glfwGetWindowUserPointer (window);
    example->onWindowClosed( );
}

void Window::frameBufferSizeHandler (GLFWwindow* window, int width, int height) {
    // void * a = glfwGetWindowUserPointer(window);
    Window* example = static_cast<Window*> (glfwGetWindowUserPointer (window));
    example->onWindowResized (glm::uvec2 (width, height));
}

void Window::onKeyPressed (const int& key, const int& mods) {
    switch (key) {
        case KEY_W:
            KeyCodes::Instance( ).keyBoard.w = true;
            break;
        case KEY_S:
            KeyCodes::Instance( ).keyBoard.s = true;
            break;
        case KEY_A:
            KeyCodes::Instance( ).keyBoard.a = true;
            break;
        case KEY_D:
            KeyCodes::Instance( ).keyBoard.d = true;
            break;
        default:
            break;
    }

    switch (key) {
        case KEY_P:
            KeyCodes::Instance( ).keyBoard.p = true;
            break;

        case KEY_F1:
            KeyCodes::Instance( ).keyBoard.f1 = true;
            break;

        case KEY_ESCAPE:
            KeyCodes::Instance( ).keyBoard.esc = true;
            break;

        default:
            break;
    }
}

void Window::onKeyReleased (const int& key, const int& mods) {
    switch (key) {
        case KEY_W:
            KeyCodes::Instance( ).keyBoard.w = false;
            break;
        case KEY_S:
            KeyCodes::Instance( ).keyBoard.s = false;
            break;
        case KEY_A:
            KeyCodes::Instance( ).keyBoard.a = false;
            break;
        case KEY_D:
            KeyCodes::Instance( ).keyBoard.d = false;
            break;
        default:
            break;
    }
}

void Window::onMouseScrolled (const float& delta) {
    KeyCodes::Instance( ).mouse.scrollDeta = delta;
    // camera.translate(glm::vec3(0.0F, 0.0F, (float)delta * 0.005f * zoomSpeed));
}

void Window::onMouseMoved (const glm::vec2& newPos) {
    const glm::vec2 deltaPos = mousePos - newPos;
    if (deltaPos == glm::vec2( )) { return; }
    KeyCodes::Instance( ).mouse.deltaPos = deltaPos;

    mousePos = newPos;
}

void Window::keyboardHandler (GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* example = (Window*) glfwGetWindowUserPointer (window);
    example->onKeyEvent (key, action, mods);
}

void Window::mouseButtonHandler (GLFWwindow* window, int button, int action, int mods) {
    Window* example = (Window*) glfwGetWindowUserPointer (window);
    example->onMouseButtonEvent (button, action, mods);
}

void Window::mouseMoveHandler (GLFWwindow* window, double posx, double posy) {
    Window* example = (Window*) glfwGetWindowUserPointer (window);
    example->onMouseMoved (glm::vec2 (posx, posy));
}

void Window::mouseScrollHandler (GLFWwindow* window, double xoffset, double yoffset) {
    Window* example = (Window*) glfwGetWindowUserPointer (window);
    example->onMouseScrolled (float (yoffset));
}

void Window::onKeyEvent (const int& key, const int& action, const int& mods) {
    switch (action) {
        case GLFW_PRESS:
            onKeyPressed (key, mods);
            break;

        case GLFW_RELEASE:
            onKeyReleased (key, mods);
            break;

        default:
            break;
    }
}

void Window::onMouseButtonEvent (const int& button, const int& action, const int& mods) {
    switch (action) {
        case GLFW_PRESS:
            onMousePressed (button, mods);
            break;

        case GLFW_RELEASE:
            onMouseReleased (button, mods);
            break;

        default:
            break;
    }
}

void Window::onMousePressed (const int& button, const int& mods) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            KeyCodes::Instance( ).mouse.left = GLFW_PRESS;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            KeyCodes::Instance( ).mouse.right = GLFW_PRESS;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            KeyCodes::Instance( ).mouse.middle = GLFW_PRESS;
            break;
        default:
            break;
    }
}

void Window::onMouseReleased (const int& button, const int& mods) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            KeyCodes::Instance( ).mouse.left = GLFW_RELEASE;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            KeyCodes::Instance( ).mouse.right = GLFW_RELEASE;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            KeyCodes::Instance( ).mouse.middle = GLFW_RELEASE;
            break;
        default:
            break;
    }
}
