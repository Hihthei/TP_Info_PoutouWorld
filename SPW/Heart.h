#pragma once

#include "Collectable.h"
#include "Settings.h"
#include "Collectable.h"

class Heart : public Collectable
{
public:
    Heart(Scene &scene, Layer layer);

    virtual void Start() override;
    virtual void Render() override;
    virtual void OnRespawn() override;

    virtual void Collect(GameBody *collector) override;

private:
    RE_Animator m_animator;

    RE_Anchor m_currentAnimation;

    enum class State
    {
        IDLE, PICKED
    };
    State m_state;
};