#include "pch.h"
#include "WindowProperties.hpp"

bool                    WindowProperties::IsVSync( ) const { return m_VSync; }
const uint32_t&         WindowProperties::GetWindowWidth( ) const { return m_Width; }
const uint32_t&         WindowProperties::GetWindowHeight( ) const { return m_Height; }
const std::string_view& WindowProperties::GetWindowName( ) const { return m_Title; }
bool                    WindowProperties::IsFullScreen( ) const { return m_Fullscreen; }

void WindowProperties::SetVSync (const bool& isVSync) { m_VSync = isVSync; }
void WindowProperties::SetFullscreen (const bool& isFullScreen) { m_Fullscreen = isFullScreen; }
void WindowProperties::SetWindowTitle (const std::string_view& title) { m_Title = title; }
