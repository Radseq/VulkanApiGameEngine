#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer
{
    std::chrono::time_point<std::chrono::steady_clock> m_Start, m_End;

    const float ms = 1000.0F;

   public:
    void  Start( );
    void  End( );
    float GetDurationInSeconds( );
    auto  GetDurationInMs( );
    auto  GetDurationInNano( );
};

#endif /* TIMER_HPP */