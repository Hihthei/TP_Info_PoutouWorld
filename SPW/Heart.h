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
    virtual void FixedUpdate() override;
    virtual void Collect(GameBody *collector) override;
    enum class State
    {
        IDLE, PICKED, BANNIER,
    };
    State m_state;

private:
    RE_Animator m_animator;

    RE_Anchor m_currentAnimation;


};