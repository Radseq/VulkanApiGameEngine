﻿#include "pch.h"
#include "Game.hpp"

void Game::initWindow( )
{
    window.createWindow ({windowName, windowSize.width, windowSize.height});

    // deviceHandler.init(window.get());
    /*
    camera.type = Camera::CameraType::firstperson;
    camera.setPerspective (60.0F, windowSize, 0.1F, 512.0F);
    camera.setRotation (glm::vec3 (-12.0F, 159.0F, 0.0F));
    camera.setTranslation (glm::vec3 (18.0F, 22.5F, 57.5F));
    camera.movementSpeed = 7.5F;*/
}

void Game::setWindowNameArgs( )
{
    assert (context != nullptr);
    const std::string str (context->getDevice( ).GetDeviceProperties( ).deviceName.data( ));
    windowNameArg = str;
}

void Game::initVulkan( )
{
    createInstance( );
    createSurface( );
    pickPhysicalDevice( );
    createLogicalDevice( );
    setWindowNameArgs( );
}

void Game::createLogicalDevice( )
{
    context = localDevices->GetLocalDevice( ).front( ).get( );

    context->CreateCommandPool( );
}

void Game::cleanUp( )
{
    renderer->destroy( );

    context->Destroy( );

    vulkanInstance.getVkInstance( ).destroySurfaceKHR (window.getSurface( ));
    vulkanInstance.destroy( );

    window.destroy( );
}

void Game::run( )
{
    initWindow( );
    initVulkan( );

    renderer = std::make_unique<Renderer> (window, context, camera);
    renderer->init( );

    render( );

    cleanUp( );
}

void Game::createInstance( ) { vulkanInstance.CreateInstance (windowName, window.getRequiredInstanceExtensions( )); }
void Game::createSurface( ) { window.createWindowSurface (vulkanInstance.getVkInstance( )); }

void Game::pickPhysicalDevice( )
{
    localDevices = std::make_unique<GraphicCore::LocalDevices> (vulkanInstance, window.getSurface( ));
}

void Game::render( )
{
    while (!window.windowShouldClose( ))
    {
        m_Timer.Start( );

        if (window.isWindowResized( ))
        {
            window.checkFramBufferSize( );
            context->getVkDevice( ).waitIdle( );
            renderer->recreateSwapChain( );
        }

        renderer->draw( );

        m_Timer.End( );

        const auto tDiffSeconds = m_Timer.GetDurationInSeconds( );

        if (viewUpdated)
        {
            viewUpdated = false;
            viewChanged (tDiffSeconds);
        }

        update (tDiffSeconds);

        window.glPollEvents( );
    }

    context->getVkDevice( ).waitIdle( );
}

void Game::update (const float& deltaTime)
{
    frameTimer = deltaTime;
    ++frameCounter;

    if (camera.isMoving( )) { viewUpdated = true; }

    // Convert to clamped timer value
    if (!KeyCodes::Instance( ).keyBoard.p)
    {
        timer += timerSpeed * frameTimer;
        if (timer > 1.0) { timer -= 1.0F; }
    }

    fpsTimer += frameTimer;

    if (fpsTimer > 1.0F)
    {
        window.setTitle (windowNameArg, frameCounter);

        lastFPS      = frameCounter;
        fpsTimer     = 0.0F;
        frameCounter = 0;
    }
}

void Game::viewChanged (const float& deltaTime)
{
    camera.update(/*deltaTime*/);
    renderer->update( );
}
