#include "Firefly.h"
#include "Collectable.h"
#include "Scene.h"
#include "Camera.h"
#include "LevelScene.h"
#include "Graphics.h"

Firefly::Firefly(Scene& scene, Layer layer) :
    Collectable(scene, layer)
{
    m_name = "Firefly";
    RE_Atlas* atlas = scene.GetAssetManager().GetAtlas(AtlasID::COLLECTABLE);
    AssertNew(atlas);

    // Animation "Idle"
    RE_AtlasPart *part = atlas->GetPart("Firefly");
    AssertNew(part);
    RE_TexAnim *flyingAnim = new RE_TexAnim(m_animator, "Idle", part);
    flyingAnim->SetCycleCount(-1);
    flyingAnim->SetCycleTime(0.3f);

    // Couleur des colliders en debug
    m_debugColor.r = 255;
    m_debugColor.g = 0;
    m_debugColor.b = 255;
}

Firefly::~Firefly()
{
}

void Firefly::Start()
{

    SetToRespawn(true);

    // Joue l'animation par défaut
    m_animator.PlayAnimation("Idle");

    // Crée le corps
    PE_World& world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "Firefly";
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_ColliderDef colliderDef;
    PE_CircleShape circle(PE_Vec2(-0.1f, 0.5f), 0.25f);
    colliderDef.isTrigger = true;
    colliderDef.filter.categoryBits = CATEGORY_COLLECTABLE;
    colliderDef.filter.maskBits = CATEGORY_PLAYER;
    colliderDef.shape = &circle;
    PE_Collider* collider = body->CreateCollider(colliderDef);
}

/*
void Firefly::Update()
{
    //TODO
    PE_Body* body = GetBody();
    PE_Vec2 position = body->GetPosition();

    //Si la luciole est encore en vie
    if (body->IsAwake() == true)
    {
        m_state = State::IDLE;
        return;
    }

    LevelScene* levelScene = dynamic_cast<LevelScene*>(&m_scene);
    if (levelScene == nullptr)
    {
        assert(false);
        return;
    }

    Player* player = levelScene->GetPlayer();
}
*/

void Firefly::Render()
{
    SDL_Renderer* renderer = m_scene.GetRenderer();
    Camera* camera = m_scene.GetActiveCamera();

    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.h = 1.0f * scale;
    rect.w = 1.0f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    m_animator.RenderCopyF(&rect, RE_Anchor::SOUTH);
}

void Firefly::OnRespawn()
{
    m_state = State::IDLE;

    SetBodyEnabled(true);
    SetEnabled(true);

    m_animator.StopAnimations();
    m_animator.PlayAnimation("Idle");
}

/*
void OnCollisionStay(GameCollision& collision)
{
    PE_Manifold& manifold = collision.manifold;
    PE_Collider* otherCollider = collision.otherCollider;

    // DID : Désactiver les collisions lorsque la luciole est prise

    if (m_state == State::PICKED)
    {
        collision.SetEnabled(false);
        return;
    }

    // Collision avec le joueur
    if (otherCollider->CheckCategory(CATEGORY_PLAYER))
    {
        Player* player = dynamic_cast<Player*>(collision.gameBody);
        if (player == nullptr)
        {
            assert(false);
            return;
        }
        float angle = PE_SignedAngleDeg(manifold.normal, PE_Vec2::down);
        if (fabsf(angle) > PLAYER_DAMAGE_ANGLE)
        {
            player->AddFirefly(1);
        }

        return;
    }
}
*/

void Firefly::Collect(GameBody* collector)
{
    Player* player = dynamic_cast<Player*>(collector);
    if (player == nullptr)
        return;

    m_state = State::PICKED;
    SetToRespawn(true);
    SetEnabled(false);
    player->AddFirefly(1);
}