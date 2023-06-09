#include "Boss.h"
#include "Scene.h"
#include "Camera.h"
#include "Player.h"

Boss::Totem::Totem(Scene& scene) :
    GameBody(scene, Layer::TERRAIN_BACKGROUND), m_animator(), m_isActive(false)
{
    m_name = "Boss";
    RE_TexAnim* anim;
    RE_AtlasPart* part;
    RE_Atlas* atlas;

    // Animation "Totem"
    atlas = scene.GetAssetManager().GetAtlas(AtlasID::BOSS);
    AssertNew(atlas);
    part = atlas->GetPart("Totem");
    AssertNew(part);
    anim = new RE_TexAnim(m_animator, "Totem", part);
    anim->SetCycleCount(0);
}

void Boss::Totem::Start()
{
    // Joue l'animation par d�faut
    m_animator.PlayAnimation("Totem");

    // Cr�e le corps
    PE_World& world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "Totem";
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Cr�e le collider
    PE_PolygonShape box(-0.4f, 0.0f, 0.4f, 1.0f);
    PE_ColliderDef colliderDef;
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
    colliderDef.shape = &box;
    colliderDef.isTrigger = true;
    PE_Collider* collider = body->CreateCollider(colliderDef);
}

void Boss::Totem::Render()
{
    SDL_Renderer* renderer = m_scene.GetRenderer();
    Camera* camera = m_scene.GetActiveCamera();
    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.w = 1.0f * scale;
    rect.h = 1.0f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    m_animator.RenderCopyF(&rect, RE_Anchor::SOUTH);
}

void Boss::Totem::OnCollisionEnter(GameCollision& collision)
{
    Player* player = dynamic_cast<Player*>(collision.gameBody);
    if (player == nullptr)
        return;

    if (player->GetStatePlayer())
    {
        m_isActive = true;
        printf("1.0\n");
    }
}

