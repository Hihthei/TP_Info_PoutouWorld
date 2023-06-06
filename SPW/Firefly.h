#pragma once

#include "Collectable.h"
#include "Settings.h"
#include "GameBody.h"

class Firefly : public Collectable
{
public:
    Firefly(Scene& scene, Layer layer);
    virtual ~Firefly();

    virtual void Start() override;
    virtual void Render() override;
    virtual void OnRespawn() override;

//    virtual void OnCollisionStay(GameCollision& collision) override;
    virtual void Collect(GameBody* collector);

private:
    RE_Animator m_animator;

    RE_Anchor m_currentAnimation;

    RE_Timer timerFirefly;
    float timeDelta;

    enum class State
    {
        IDLE, PICKED
    };
    State m_state;
};