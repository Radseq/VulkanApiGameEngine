#include "pch.h"
#include "Window.hpp"

bool Window::init( ) { return GLFW_TRUE == glfwInit( ); }

Window::Window( ) { glfwInit( ); }

void Window::destroy( )
{
    glfwDestroyWindow (window);
    glfwTerminate( );
    window = nullptr;
}

bool Window::isKeyPressed (int key) const { return glfwGetKey (window, key) == GLFW_PRESS; }

GLFWwindow* Window::getWindow( ) const { return window; }

void Window::checkFramBufferSize( )
{
    int width = 0, height = 0;
    glfwGetFramebufferSize (window, &width, &height);
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize (window, &width, &height);
        glfwWaitEvents( );
    }

    isWindowResize = false;
}

void Window::setWindowSize (glm::ivec2& newSize) const { glfwGetWindowSize (window, &newSize.x, &newSize.y); }
int  Window::windowShouldClose( ) const { return glfwWindowShouldClose (window); }
void Window::setWindowShouldClose( ) const { glfwSetWindowShouldClose (window, 1); }

void Window::glPollEvents( ) { glfwPollEvents( ); }

const vk::SurfaceKHR& Window::getSurface( ) const { return cppSurface; }

std::vector<std::string> Window::getRequiredInstanceExtensions( )
{
    std::vector<std::string> result;
    uint32_t                 count = 0;
    const char**             names = glfwGetRequiredInstanceExtensions (&count);
    if (names && count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            VulkanGame::PassToVec (result, names [i]);
            // result.emplace_back (names [i]);
        }
    }

#ifdef NDEBUG

#else
    // result.emplace_back (VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    VulkanGame::PassToVec (result, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    return result;
}

void Window::createWindowSurface (const vk::Instance& instance, const vk::AllocationCallbacks* pAllocator)
{
    const auto result = static_cast<vk::Result> (glfwCreateWindowSurface (
        VkInstance (instance), window, reinterpret_cast<const VkAllocationCallbacks*> (pAllocator), &rawSurface));

    if (result != vk::Result::eSuccess) { throw std::runtime_error ("failed to create window surface!"); }

    vk::createResultValue (result, rawSurface, "vk::CommandBuffer::begin");

    cppSurface = vk::SurfaceKHR (rawSurface);
}

void Window::createWindow (const WindowProperties&& windowProperties, const glm::ivec2& position)
{
    glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
    auto* const monitor = glfwGetPrimaryMonitor( );

    const auto WindowName = windowProperties.GetWindowName( ).data( );

    if (windowProperties.IsFullScreen( ))
    {
        const auto* const mode = glfwGetVideoMode (monitor);
        window                 = glfwCreateWindow (mode->width, mode->height, WindowName, monitor, nullptr);
    }
    else
    {
        // vk::Extent2D WindowSize = (windowProperties.GetWindowWidth( ), windowProperties.GetWindowHeight( ));
        uint32_t width  = windowProperties.GetWindowWidth( );
        uint32_t height = windowProperties.GetWindowHeight( );

        size.width  = width;
        size.height = height;
        window      = glfwCreateWindow (size.width, size.height, WindowName, nullptr, nullptr);
        if (position != glm::ivec2 {INT_MIN, INT_MIN}) { glfwSetWindowPos (window, position.x, position.y); }
    }

    if (windowProperties.IsVSync( ))
        glfwSwapInterval (1);
    else
        glfwSwapInterval (0);

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

void Window::setExtent2DWindowSize (const vk::Extent2D& setSize)
{
    glm::ivec2 tempSize {setSize.width, setSize.height};
    setWindowSize (tempSize);
    size = setSize;
}

void Window::setTitle (const std::string& deviceName, const uint32_t& frameCounter) const
{
    /* const std::string windowTitle =
        "Game - " + deviceName + " - " + std::to_string (frameCounter) + " fps";
    */
    std::string&& windowTitle = "Game - " + deviceName + " - " + std::to_string (frameCounter) + " fps";

    glfwSetWindowTitle (window, windowTitle.c_str( ));
}

void Window::setSizeLimits (const glm::uvec2& minSize, const glm::uvec2& maxSize) const
{
    glfwSetWindowSizeLimits (window, minSize.x, minSize.y, (maxSize.x != 0) ? maxSize.x : minSize.x,
                             (maxSize.y != 0) ? maxSize.y : minSize.y);
}

void Window::onWindowResized (const vk::Extent2D& newSize)
{
    isWindowResize = true;
    size           = newSize;
}

bool Window::isWindowResized( ) const { return isWindowResize; }

const VkSurfaceKHR& Window::getCSurface( ) const { return rawSurface; }
const glm::vec2&    Window::getMousePos( ) const { return mousePos; }
const glm::vec2&    Window::getDeltaPos( ) const { return deltaPos; }

glm::vec2 Window::getWindowSize( ) const
{
    int width;
    int height;
    glfwGetWindowSize (window, &width, &height);
    glm::vec2 size {width, height};
    return size;
}

void Window::waitEvents( ) { glfwWaitEvents( ); }

void Window::showWindow (bool show) const
{
    if (show) { glfwShowWindow (window); }
    else
    {
        glfwHideWindow (window);
    }
}

void Window::closeHandler (GLFWwindow* window)
{
    Window* example = (Window*) glfwGetWindowUserPointer (window);
    example->onWindowClosed( );
}

void Window::frameBufferSizeHandler (GLFWwindow* window, int width, int height)
{
    Window* example = static_cast<Window*> (glfwGetWindowUserPointer (window));
    example->onWindowResized (vk::Extent2D (width, height));
}

void Window::onKeyPressed (const int& key, const int& mods)
{
    switch (key)
    {
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

    switch (key)
    {
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

void Window::onKeyReleased (const int& key, const int& mods)
{
    switch (key)
    {
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

void Window::onMouseScrolled (const float& delta) { KeyCodes::Instance( ).mouse.scrollDeta = delta; }

void Window::onMouseMoved (const glm::vec2& newPos)
{
    const glm::vec2 deltaPos = mousePos - newPos;
    if (deltaPos == glm::vec2( )) { return; }
    KeyCodes::Instance( ).mouse.deltaPos = deltaPos;

    mousePos = newPos;
}

void Window::keyboardHandler (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* example = (Window*) glfwGetWindowUserPointer (window);
    example->onKeyEvent (key, action, mods);
}

void Window::mouseButtonHandler (GLFWwindow* window, int button, int action, int mods)
{
    Window* example = (Window*) glfwGetWindowUserPointer (window);
    example->onMouseButtonEvent (button, action, mods);
}

void Window::mouseMoveHandler (GLFWwindow* window, double posx, double posy)
{
    Window* example = (Window*) glfwGetWindowUserPointer (window);
    example->onMouseMoved (glm::vec2 (posx, posy));
}

void Window::mouseScrollHandler (GLFWwindow* window, double xoffset, double yoffset)
{
    Window* example = (Window*) glfwGetWindowUserPointer (window);
    example->onMouseScrolled (float (yoffset));
}

void Window::onKeyEvent (const int& key, const int& action, const int& mods)
{
    switch (action)
    {
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

void Window::onMouseButtonEvent (const int& button, const int& action, const int& mods)
{
    switch (action)
    {
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

void Window::onMousePressed (const int& button, const int& mods)
{
    switch (button)
    {
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

void Window::onMouseReleased (const int& button, const int& mods)
{
    switch (button)
    {
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
