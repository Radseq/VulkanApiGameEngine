#include "Timer.hpp"

void Timer::Start( ) { m_Start = std::chrono::high_resolution_clock::now( ); }
void Timer::End( ) { m_End = std::chrono::high_resolution_clock::now( ); }

float Timer::GetDurationInSeconds( )
{
    // float b = std::chrono::duration_cast<float, std::chrono::milliseconds> (m_End - m_Start).count( );
    return std::chrono::duration<float, std::milli> (m_End - m_Start).count( ) / ms;
}

auto Timer::GetDurationInMs( )
{
    return std::chrono::duration_cast<std::chrono::milliseconds> (m_End - m_Start).count( );
}

auto Timer::GetDurationInNano( )
{
    return std::chrono::duration_cast<std::chrono::nanoseconds> (m_End - m_Start).count( );
}