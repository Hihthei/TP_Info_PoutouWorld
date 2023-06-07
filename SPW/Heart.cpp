#include "Heart.h"
#include "Collectable.h"
#include "Scene.h"
#include "Camera.h"
#include "Player.h"
#include "LevelScene.h"
#include "Camera.h"

Heart::Heart(Scene &scene, Layer layer) :
    Collectable(scene, Layer::COLLECTABLE)
{
    m_name = "Heart";
    RE_Atlas* atlas = scene.GetAssetManager().GetAtlas(AtlasID::COLLECTABLE);
    AssertNew(atlas);

    // Animation "Idle"
    RE_AtlasPart* part = atlas->GetPart("Heart");
    AssertNew(part);
    RE_TexAnim* flyingAnim = new RE_TexAnim(m_animator, "Idle", part);
    flyingAnim->SetCycleCount(-1);
    flyingAnim->SetCycleTime(0.5f);

    // Animation "MoveY"
    RE_ShiftAnim* ShiftingAnimYFirstPath = new RE_ShiftAnim(m_animator, "MoveY", Vec2(0, -5.0f), Vec2(0, 5.0f));
    ShiftingAnimYFirstPath->SetCycleCount(-1);
    ShiftingAnimYFirstPath->SetCycleTime(1.125f);
    ShiftingAnimYFirstPath->AddFlags(RE_AnimFlag::ALTERNATE);
    ShiftingAnimYFirstPath->SetEasing(RE_EasingFct_Cos);

    // Couleur des colliders en debug
    m_debugColor.r = 255;
    m_debugColor.g = 0;
    m_debugColor.b = 255;
}

void Heart::Start()
{
    SetToRespawn(true);

    // Joue l'animation par défaut
    m_animator.PlayAnimation("Idle");
    m_animator.PlayAnimation("MoveY");

    // Crée le corps
    PE_World& world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.5f);
    bodyDef.name = "Heart";
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_ColliderDef colliderDef;
    PE_CircleShape circle(PE_Vec2(0.0f, 0.25f), 0.25f);
    colliderDef.isTrigger = true;
    colliderDef.filter.categoryBits = CATEGORY_COLLECTABLE;
    colliderDef.filter.maskBits = CATEGORY_PLAYER;
    colliderDef.shape = &circle;
    PE_Collider* collider = body->CreateCollider(colliderDef);
}

void Heart::Render()
{
    SDL_Renderer* renderer = m_scene.GetRenderer();
    Camera* camera = m_scene.GetActiveCamera();

    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.h = 0.8f * scale;
    rect.w = 0.8f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);

    m_currentAnimation = RE_Anchor::SOUTH;
    m_animator.RenderCopyF(&rect, m_currentAnimation);
}

void Heart::OnRespawn()
{
    m_state = State::IDLE;

    SetBodyEnabled(true);
    SetEnabled(true);

    m_animator.StopAnimations();
    m_animator.PlayAnimation("Idle");
}

void Heart::Collect(GameBody *collector)
{
    Player* player = dynamic_cast<Player*>(collector);
    if (player == nullptr)
        return;

    m_state = State::PICKED;
    SetToRespawn(true);
    SetEnabled(false);
    player->AddHeart(1);
}
