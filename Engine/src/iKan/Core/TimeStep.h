// ******************************************************************************
//   File    : TimeStep.h
//   Project : i-Kan : Core
//
//   Created by Ashish
// ******************************************************************************

#pragma once

namespace iKan {
    
    // ******************************************************************************
    // Clasas to store Game Time in seconds, miliseconds nd micro seconds
    // ******************************************************************************
    class TimeStep
    {
    public:
        TimeStep(float time = 0.0f)
        : m_Time(time) {}
        
        ~TimeStep() = default;
        
        operator float() const { return m_Time; }
        
        float GetSeconds() const { return m_Time; }
        float GetMilliSeconds() const { return m_Time * 1000.0f; }
        float GetMicroSeconds() const { return m_Time * 1000000.0f; }
      
    private:
        float m_Time = 0.0f;
    };
    
}
