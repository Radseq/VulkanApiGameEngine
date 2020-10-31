#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>

//#include "../Render/Renderer.hpp"
#include "../Render/Renderer.hpp"
#include "../Window/Window.hpp"
#include "../entities/camera.hpp"
#include "Device/LocalDevices.hpp"
#include "Device/PhysicalDevice.hpp"
#include "Device/VulkanDevice.hpp"
#include "Framebuffer/SwapChain.hpp"
#include "Instance/VulkanInstance.hpp"

class Camera;
class Window;
class Renderer;
class GameCore::SwapChain;

class Game
{
    const std::string windowName = "Jakas Gra";
    std::string       windowNameArg;
    const std::string appName {"Vulkan"};
    const int         WIDTH  = Configuration::getInstance( )->GetConfigIntOption ("WindowWidth");
    const int         HEIGHT = Configuration::getInstance( )->GetConfigIntOption ("WindowHeight");
    // const int MAX_FRAMES_IN_FLIGHT = Configuration::getInstance()->GetConfigIntOption("MaxFramesInFlight");
    const int MAX_FPS = Configuration::getInstance( )->GetConfigIntOption ("MaxFps");

    GameCore::VulkanInstance vulkanInstance { };
    GameCore::VulkanDevice*  context = nullptr;

    vk::Extent2D windowSize {WIDTH, HEIGHT};

    Camera camera { };

    bool viewUpdated {false};

    Window                                  window { };
    std::unique_ptr<GameCore::LocalDevices> localDevices;  // vulkanInstance, Surface
    std::unique_ptr<GameCore::SwapChain>    swapChain;     // context
    std::unique_ptr<Renderer>               renderer;      // swapChain, context, camera

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

    void initWindow( );
    void initVulkan( );
    void cleanUp( );
    void createSwapChain( );

    void createInstance( );
    void createSurface( );
    void pickPhysicalDevice( );
    void createLogicalDevice( );

    void cleanupSwapChain( );

    void recreateSwapChain( );
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