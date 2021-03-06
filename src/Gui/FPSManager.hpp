#pragma once

#include "../Common/Common.hpp"

class FPSManager {
   public:
    FPSManager(f32 targetFPS = 60);

    void SetTargetFPS(f32);
    f32 GetTargetFPS() const;
    f32 GetActualFPS() const;

    // In seconds
    f32 GetElapsed() const;

    void StartFrame();
    void EndFrameAndWait();

    u64 GetFrameCount() const;

   private:
    f32 m_TargetFPS;
    u64 m_MinFrameTime;

    u64 m_FrameCount = 0;

    u64 m_StartFrame;
    u64 m_Elapsed = 1;
};