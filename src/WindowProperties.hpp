#ifndef WINDOW_PROPERTIES_HPP
#define WINDOW_PROPERTIES_HPP

#include "pch.h"

class WindowProperties
{
    std::string_view m_Title {"Vulkan Api Game"};
    uint32_t         m_Width {1280};
    uint32_t         m_Height {720};
    bool             m_VSync {0};
    bool             m_Fullscreen {false};

   public:
    WindowProperties (const std::string_view& title, const uint32_t& width, const uint32_t& height)
        : m_Title (title)
        , m_Width (width)
        , m_Height (height) { };

    bool                    IsVSync( ) const;
    const uint32_t&         GetWindowWidth( ) const;
    const uint32_t&         GetWindowHeight( ) const;
    const std::string_view& GetWindowName( ) const;
    bool                    IsFullScreen( ) const;

    void SetVSync (const bool& isVSync);
    void SetFullscreen (const bool& isFullScreen);
    void SetWindowTitle (const std::string_view& title);
};

#endif  // WINDOW_PROPERTIES_HPP
