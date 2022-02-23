#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>

#include "KeyCodes.hpp"
#include "WindowProperties.hpp"
#include "vulkan/vulkan.hpp"

class Window
{
    vk::Extent2D   size;
    VkSurfaceKHR   rawSurface {nullptr};
    vk::SurfaceKHR cppSurface {nullptr};
    GLFWwindow*    window {nullptr};

    static void closeHandler (GLFWwindow* window);
    static void frameBufferSizeHandler (GLFWwindow* window, int width, int height);

    bool isWindowResize = false;

    glm::vec2 mousePos;
    glm::vec2 deltaPos;

   public:
    bool init( );
    Window( );

    void destroy( );

    void createWindowSurface (const vk::Instance& instance, const vk::AllocationCallbacks* pAllocator = nullptr);

    void createWindow (const WindowProperties&& windowProperties, const glm::ivec2& position = {INT_MIN, INT_MIN});

    void makeCurrent( ) const { glfwMakeContextCurrent (window); }
    void present( ) const { glfwSwapBuffers (window); }
    void showWindow (bool show = true) const;

    void setExtent2DWindowSize (const vk::Extent2D& size);
    void setTitle (const std::string& deviceName, const uint32_t& frameCounter = 0) const;
    void setSizeLimits (const glm::uvec2& minSize, const glm::uvec2& maxSize = { }) const;
    void setWindowSize (glm::ivec2& newSize) const;
    void setWindowShouldClose( ) const;

    bool        isWindowResized( ) const;
    void        waitEvents( );
    bool        isKeyPressed (int key) const;
    int         windowShouldClose( ) const;
    static void glPollEvents( );

    const glm::vec2&      getMousePos( ) const;
    const glm::vec2&      getDeltaPos( ) const;
    glm::vec2             getWindowSize( ) const;
    const VkSurfaceKHR&   getCSurface( ) const;
    const vk::SurfaceKHR& getSurface( ) const;
    vk::Extent2D          getExtent2DWindowSize( ) const;
    GLFWwindow*           getWindow( ) const;
    void                  checkFramBufferSize( );

    static std::vector<std::string> getRequiredInstanceExtensions( );
    static void keyboardHandler (GLFWwindow* glFwWindow, int key, int scancode, int action, int mods);
    static void mouseButtonHandler (GLFWwindow* glFwWindow, int button, int action, int mods);
    static void mouseMoveHandler (GLFWwindow* glFwWindow, double posx, double posy);
    static void mouseScrollHandler (GLFWwindow* glFwWindow, double xoffset, double yoffset);

    // Keyboard handling
    virtual void onKeyEvent (const int& key, const int& action, const int& mods);

    virtual void onKeyPressed (const int& key, const int& mods);
    virtual void onKeyReleased (const int& key, const int& mods);

    // Mouse handling
    virtual void onMouseButtonEvent (const int& button, const int& action, const int& mods);

    virtual void onMousePressed (const int& button, const int& mods);
    virtual void onMouseReleased (const int& button, const int& mods);
    virtual void onMouseMoved (const glm::vec2& newPos);
    virtual void onMouseScrolled (const float& delta);
    virtual void onWindowResized (const vk::Extent2D& newSize);
    virtual void onWindowClosed( ) { }
};

#endif  // WINDOW_HPP
