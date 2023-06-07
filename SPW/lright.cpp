#include "lright.h"
#include "Scene.h"
#include "Graphics.h"
#include "LevelScene.h"
#include "Camera.h"
#include "Settings.h"
#include "Player.h"
#include "StaticMap.h"


lright::lright(Scene& scene) :
    GameBody(scene, Layer::TERRAIN), m_animator()
{
    m_name = "lright";


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

lright::~lright()
{

}

void lright::Start()
{
    SetToRespawn(true);

    // Joue l'animation par défaut
    m_animator.PlayAnimation("Base");


    // Crée le corps
    PE_World& world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::KINEMATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "lright";
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_PolygonShape box(-1.5f, 0.0f, 1.5f, 0.5f);
    PE_ColliderDef colliderDef;
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
    colliderDef.shape = &box;
    colliderDef.friction = 2.5f;
    PE_Collider* collider = body->CreateCollider(colliderDef);
    colliderDef.isOneWay = true;
}

void lright::FixedUpdate()
{
    PE_Body* body = GetBody();
    PE_Vec2 position = body->GetPosition();
    PE_Vec2 velocity = body->GetLocalVelocity();
    if (position.x > (GetStartPosition().x + 4.0f))
    {
        m_state = State::LEFTING;
    }
    if (position.x <= (GetStartPosition().x + 0.5f ))
    {
        m_state = State::RIGHTING;
    }
    if (m_state == State::RIGHTING)
    {
        body->SetVelocity(PE_Vec2(2.0f, 0.0f));
    }
    if (m_state == State::LEFTING)
    {
        body->SetVelocity(PE_Vec2(-2.0f, 0.0f));
    }
}

void lright::Render()
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

void lright::OnCollisionEnter(GameCollision& collision)
{
    if (collision.otherCollider->CheckCategory(CATEGORY_PLAYER))
    {

    }


}

void lright::OnRespawn()
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
