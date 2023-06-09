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

    class Totem : public GameBody
    {
    public:
        Totem(Scene& scene);

        virtual void Start() override;
        virtual void Render() override;
        virtual void OnCollisionEnter(GameCollision& collision) override;

    private:
        RE_Animator m_animator;
        bool m_isActive;
    };

private:
    RE_Animator m_animator;

    enum class State
    {
        IDLE, DYING, DEAD, ATTACKING
    };
    State m_state;

    Totem* b_totem;

    float b_globalTimer;

    bool b_onScreen;
};