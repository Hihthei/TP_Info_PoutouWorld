#include "lfalling.h"
#include "Scene.h"
#include "Graphics.h"
#include "LevelScene.h"
#include "Camera.h"
#include "Settings.h"
#include "Player.h"
#include "StaticMap.h"


lfalling::lfalling(Scene& scene) :
    GameBody(scene, Layer::TERRAIN), m_animator()
{
    m_name = "lfalling";


    // Animation "Base"
    RE_Atlas* atlas = scene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);
    AssertNew(atlas);
    RE_AtlasPart* part = atlas->GetPart("MovingPlatform");
    AssertNew(part);
    RE_TexAnim* anim = new RE_TexAnim(m_animator, "Base", part);

    m_debugColor.r = 255;
    m_debugColor.g = 0;
    m_debugColor.b = 255;
}

lfalling::~lfalling()
{

}

void lfalling::Start()
{
    SetToRespawn(true);

    // Joue l'animation par d�faut
    m_animator.PlayAnimation("Base");


    // Cr�e le corps
    PE_World& world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::KINEMATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "lfalling";
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Cr�e le collider
    PE_PolygonShape box(-1.5f, 0.0f, 1.5f, 0.5f);
    PE_ColliderDef colliderDef;
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
    colliderDef.shape = &box;
    colliderDef.friction = 2.5f;
    colliderDef.isOneWay = true;
    PE_Collider* collider = body->CreateCollider(colliderDef);
}

void lfalling::FixedUpdate()
{
    PE_Body* body = GetBody();
    PE_Vec2 position = body->GetPosition();
    PE_Vec2 velocity = body->GetLocalVelocity();
    if (m_state == State::FALLING)
    {
        body->SetVelocity(PE_Vec2(0.0f, -3.0f));
    }
    if (position.y < (GetStartPosition().y - 5.0f))
    {
        m_state = State::MONTING;
    }
    if (position.y >= (GetStartPosition().y) && m_state == State::MONTING)
    {
        m_state = State::IDLE;
    }
    if (m_state == State::MONTING)
    {
        body->SetVelocity(PE_Vec2(0.0f, 1.0f));

    }
    if (m_state == State::IDLE)
    {
        body->SetVelocity(PE_Vec2(0.0f, 0.0f));

    }

}

void lfalling::Render()
{
    SDL_Renderer* renderer = m_scene.GetRenderer();
    Camera* camera = m_scene.GetActiveCamera();

    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.w = 3.0f * scale;
    rect.h = 0.5f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    m_animator.RenderCopyF(&rect, RE_Anchor::SOUTH);
}

void lfalling::OnCollisionEnter(GameCollision& collision)
{
    if (collision.otherCollider->CheckCategory(CATEGORY_PLAYER))
    {
        m_state = State::FALLING;

    }


}

void lfalling::OnRespawn()
{
    m_state = State::IDLE;

    SetToRespawn(true);
    SetBodyEnabled(true);
    SetEnabled(true);

    PE_Body* body = GetBody();
    body->SetPosition(GetStartPosition() + PE_Vec2(0.5f, 0.0f));
    body->SetVelocity(PE_Vec2::zero);
    body->ClearForces();
}
