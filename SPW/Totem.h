#include "Settings.h"
#include "GameBody.h"

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
    PE_Vec2 position;
    PE_Vec2 positionSpawnBoss;
};