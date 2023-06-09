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

    void AddHeart(int count);
    void Kill();
    virtual void Damage(GameBody* damager) override;

    virtual void OnCollisionEnter(GameCollision& collision) override;
    virtual void OnCollisionStay(GameCollision& collision) override;

private:
    RE_Animator b_animator;

    enum class State
    {
        IDLE, DYING, DEAD, INVINCIBLE, INVINCIBLE_DELAY
    };
    State b_state;

    float b_globalTimer;
    float b_invincibleDelay;

    bool b_onScreen;
    bool b_invicibleState;

    int b_heartCount;
};