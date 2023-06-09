#pragma once

#include "Settings.h"
#include "Enemy.h"

class nutdebile : public Enemy
{
public:
    nutdebile(Scene& scene);
    virtual ~nutdebile();

    virtual void Start() override;
    virtual void FixedUpdate() override;
    virtual void Render() override;
    virtual void OnRespawn() override;

    virtual void Damage(GameBody* damager) override;

    virtual void OnCollisionStay(GameCollision& collision) override;
    virtual void OnCollisionEnter(GameCollision& collision) override;

    virtual void SetSpawnStatue(bool bolleen);

private:
    RE_Animator m_animator;
    bool dir, dir2, dir3 ;
    enum class State
    {
        IDLE, SPINNING, DYING, DYING2, ATTACKING
    };
    State m_state;

    bool m_spawnedByBoss;
};