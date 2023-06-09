#pragma once

#include "Settings.h"
#include "Enemy.h"

class Boss : public Enemy
{
public:
    Boss(Scene& scene);
    virtual ~Boss();

    virtual void Start() override;
    virtual void FixedUpdate() override;
    virtual void Render() override;
    virtual void OnRespawn() override;

    virtual void Damage(GameBody* damager) override;

    virtual void OnCollisionStay(GameCollision& collision) override;

private:
    RE_Animator m_animator;

    enum class State
    {
        IDLE, DYING, DEAD, ATTACKING
    };
    State m_state;

    float b_globalTimer;

    bool b_onScreen;

    int m_heartCount;
};