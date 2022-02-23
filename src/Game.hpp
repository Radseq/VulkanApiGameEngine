#ifndef GAME_HPP
#define GAME_HPP

#include "LocalDevices.hpp"
#include "PhysicalDevice.hpp"
#include "Renderer.hpp"
#include "SwapChain.hpp"
#include "Timer.hpp"
#include "VulkanDevice.hpp"
#include "VulkanInstance.hpp"
#include "Window.hpp"
#include "camera.hpp"
#include "pch.h"

class Camera;
class Window;
class Renderer;
class GraphicCore::SwapChain;
class Timer;

class Game
{
    const std::string_view windowName {"Vulkan Api Game"};
    std::string            windowNameArg;
    const int              WIDTH  = Configuration::getInstance( )->GetConfigIntOption ("WindowWidth");
    const int              HEIGHT = Configuration::getInstance( )->GetConfigIntOption ("WindowHeight");
    // const int MAX_FRAMES_IN_FLIGHT = Configuration::getInstance()->GetConfigIntOption("MaxFramesInFlight");
    const int MAX_FPS = Configuration::getInstance( )->GetConfigIntOption ("MaxFps");

    GraphicCore::VulkanInstance vulkanInstance { };
    GraphicCore::VulkanDevice*  context = nullptr;

    vk::Extent2D windowSize {WIDTH, HEIGHT};

    Camera camera { };

    bool viewUpdated {false};

    Window                                     window { };
    std::unique_ptr<GraphicCore::LocalDevices> localDevices;  // vulkanInstance, Surface
    std::unique_ptr<Renderer>                  renderer;      // swapChain, context, camera

    // DeviceHandler deviceHandler{camera};

    // fps timer (one second interval)
    float fpsTimer = 0.0F;
    // Last frame time, measured using a high performance timer (if available)
    float frameTimer {0.0015f};
    // Frame counter to display fps
    uint32_t frameCounter {0};
    uint32_t lastFPS {0};
    // Defines a frame rate independent timer value clamped from -1.0...1.0
    // For use in animations, rotations, etc.
    float timer = 0.0F;
    // Multiplier for speeding up (or slowing down) the global timer
    float timerSpeed = 0.25F;

    Timer m_Timer { };

    void initWindow( );
    void setWindowNameArgs( );

    void initVulkan( );
    void cleanUp( );

    void createInstance( );
    void createSurface( );
    void pickPhysicalDevice( );
    void createLogicalDevice( );

    void update (const float& deltaTime);

    // Called when view change occurs
    // Can be overriden in derived class to e.g. update uniform buffers
    // Containing view dependant matrices
    void viewChanged (const float& deltaTime);
    void render( );

   public:
    void run( );
};

#endif  // GAME_HPP
