#pragma once

#include "Settings.h"
#include "GameBody.h"

class sright : public GameBody
{
public:
    sright(Scene& scene);
    virtual ~sright();

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