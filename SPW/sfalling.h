#pragma once

#include "Settings.h"
#include "GameBody.h"

class sfalling : public GameBody
{
public:
    sfalling(Scene& scene);
    virtual ~sfalling();

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