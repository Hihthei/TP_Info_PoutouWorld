#pragma once

#include "Settings.h"
#include "GameBody.h"

class mouvante2 : public GameBody
{
public:
    mouvante2(Scene& scene);
    virtual ~mouvante2();

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