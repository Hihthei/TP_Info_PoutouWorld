#pragma once

#include "Settings.h"
#include "GameBody.h"

class mouvante1 : public GameBody
{
public:
    mouvante1(Scene& scene);
    virtual ~mouvante1();

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