#pragma once

#include "engine/rendering/RenderDevice.h"
#include "engine/rendering/RenderPipeline.h"

class StudioEngine
{
public:
    static StudioEngine& Get()
    {
        static StudioEngine instance;
        return instance;
    }

    bool Init()
    {
        if (!m_renderDevice.Init())
            return false;
        if (!m_renderPipeline.Init(&m_renderDevice))
            return false;
        return true;
    }

    Engine::RenderDevice* getRenderDevice() { return &m_renderDevice; }
    Engine::RenderPipeline* getRenderPipeline() { return &m_renderPipeline; }

private:
    Engine::RenderDevice m_renderDevice;
    Engine::RenderPipeline m_renderPipeline;
};