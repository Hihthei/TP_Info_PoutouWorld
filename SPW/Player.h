#pragma once

#include "Settings.h"
#include "GameBody.h"

#define PLAYER_DAMAGE_ANGLE 55.0f

class Player : public GameBody
{
public:
    Player(Scene &scene);
    virtual ~Player();

    virtual void Start() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void FixedUpdate() override;
    virtual void OnRespawn() override;
    virtual void DrawGizmos() override;

    virtual void OnCollisionEnter(GameCollision &collision) override;
    virtual void OnCollisionStay(GameCollision &collision) override;

    int GetFireflyCount() const;
    int GetHeartCount() const;
    int GetLifeCount() const;
    void AddFirefly(int count);
    void AddHeart(int count);
    void Damage(int count);
    void Kill();
    void Bounce();

private:
    void WakeUpSurroundings();

    enum class State
    {
        IDLE, WALKING, RUNNING, SKIDDING, FALLING
    };
    State m_state;

    enum class State_Player
    {
        ALIVE, DYING, DEAD, INVINCIBLE
    };
    State_Player m_statePlayer;

    RE_Animator m_animator;

    float m_hDirection;

    float m_speed;
    float m_timerSpeed;

    float m_timerDead;
    float m_invincibleDelay;

    float m_animSpeedValue;

    //vrai -> il vient de changer de sens
    //faux -> il était déjà dans ce sens
    bool m_stateSwitchRunning;

    bool m_jump;
    bool m_jumpHold;
    float m_jumpDelay;

    bool m_onGround;
    bool m_bounce;
    bool m_facingRight;

    int m_lifeCount = 3;
    int m_heartCount;
    int m_fireflyCount;
};


inline void Player::Bounce()
{
    m_bounce = true;
}

inline int Player::GetFireflyCount() const
{
    return m_fireflyCount;
}

inline int Player::GetHeartCount() const
{
    return m_heartCount;
}

inline int Player::GetLifeCount() const
{
    return m_lifeCount;
}