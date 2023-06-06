#include "Checkpoint.h"
#include "Scene.h"
#include "Camera.h"
#include "Player.h"

Checkpoint::Checkpoint(Scene &scene) :
    GameBody(scene, Layer::TERRAIN_BACKGROUND), m_animator(), m_isActive(false)
{
    m_name = "Checkpoint";
    RE_TexAnim* anim;
    RE_AtlasPart* part;
    RE_Atlas* atlas;

    // Animation "Empty"
    atlas = scene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);
    AssertNew(atlas);
    part = atlas->GetPart("CheckpointEmpty");
    AssertNew(part);
    anim = new RE_TexAnim(m_animator, "Empty", part);
    anim->SetCycleCount(0);

    // Animation "Full"
    atlas = scene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);
    AssertNew(atlas);
    part = atlas->GetPart("CheckpointFull");
    AssertNew(part);
    anim = new RE_TexAnim(m_animator, "Full", part);
    anim->SetCycleCount(0);


}

void Checkpoint::Start()
{
    // Joue l'animation par défaut
    m_animator.PlayAnimation("Empty");

    // Crée le corps
    PE_World& world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "Checkpoint";
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_PolygonShape box(-0.4f, 0.0f, 0.4f, 1.0f);
    PE_ColliderDef colliderDef;
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
    colliderDef.shape = &box;
    colliderDef.isTrigger = true;
    PE_Collider* collider = body->CreateCollider(colliderDef);
}

void Checkpoint::Render()
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

void Checkpoint::OnCollisionEnter(GameCollision &collision)
{
    if (collision.otherCollider->CheckCategory(CATEGORY_PLAYER))
    {
        m_animator.PlayAnimation("Full");
        Player* player = dynamic_cast<Player*>(collision.gameBody);
        player->SetStartPosition(GetStartPosition());

    }
}
