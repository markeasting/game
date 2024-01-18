#pragma once

#include "SDL_timer.h"
#include <utility>

class Timer {
public:

    Timer() = default;

    std::pair<float, float> update() {
        m_ticks = SDL_GetTicks();
        m_prevTime = m_time;
        m_time = m_ticks / 1000.0f;
        m_deltaTime = m_time - m_prevTime;

        return std::make_pair(m_time, m_deltaTime);
    }
    
    /** Ticks (ms) */
    uint32_t ticks() const { 
        return m_ticks; 
    }

    /** Current time (s) */
    float currentTime() const { 
        return m_time; 
    }

    /** Current time (s) */
    float seconds() const { 
        return m_time; 
    }

    /** Delta time since previous frame (s) */
    float dt() const { 
        return m_deltaTime; 
    }

private:
    uint32_t m_ticks = 0;

    float m_time = 0;
    float m_prevTime = 0;
    float m_deltaTime = 0;
};
