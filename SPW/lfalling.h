#pragma once

#include "Settings.h"
#include "GameBody.h"

class lfalling : public GameBody
{
public:
    lfalling(Scene& scene);
    virtual ~lfalling();

    virtual void Start() override;
    virtual void Render() override;
    virtual void FixedUpdate() override;
    virtual void OnCollisionEnter(GameCollision& collision) override;
    virtual void OnRespawn() override;

private:
    RE_Animator m_animator;

    enum class State
    {
        IDLE, FALLING, MONTING
    };
    State m_state;
};