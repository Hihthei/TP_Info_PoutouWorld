#pragma once

#include "Settings.h"
#include "GameBody.h"

class sleft : public GameBody
{
public:
    sleft(Scene& scene);
    virtual ~sleft();

    virtual void Start() override;
    virtual void Render() override;
    virtual void FixedUpdate() override;
    virtual void OnCollisionEnter(GameCollision& collision) override;
    virtual void OnRespawn() override;

private:
    RE_Animator m_animator;

    enum class State
    {
        IDLE, RIGHTING, LEFTING
    };
    State m_state;
};