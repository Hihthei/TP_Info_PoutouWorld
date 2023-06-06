#include "Brick.h"
#include "Scene.h"
#include "Graphics.h"
#include "LevelScene.h"
#include "Camera.h"
#include "Settings.h"


Brick::Brick(Scene &scene) :
    GameBody(scene, Layer::TERRAIN), m_animator()
{
    m_name = "Brick";


    // Animation "Base"
    RE_Atlas* atlas = scene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);
    AssertNew(atlas);
    RE_AtlasPart* part = atlas->GetPart("Brick");
    AssertNew(part);
    RE_TexAnim* anim = new RE_TexAnim(m_animator, "Base", part);

    m_debugColor.r = 255;
    m_debugColor.g = 0;
    m_debugColor.b = 255;
}

Brick::~Brick()
{
    
}

void Brick::Start()
{
    SetToRespawn(true);

    // Joue l'animation par défaut
    m_animator.PlayAnimation("Base");


    // Crée le corps
    PE_World& world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "Brick";
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_PolygonShape box(-0.5f, 0.0f, 0.5f, 1.0f);
    PE_ColliderDef colliderDef;
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
    colliderDef.shape = &box;
    colliderDef.friction = 2.5f;
    PE_Collider* collider = body->CreateCollider(colliderDef);
}

void Brick::Render()
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

void Brick::OnCollisionEnter(GameCollision &collision)
{
    PE_Manifold& manifold = collision.manifold;
    PE_Collider* otherCollider = collision.otherCollider;

    if (otherCollider->CheckCategory(CATEGORY_PLAYER))
    {
        Player* player = dynamic_cast<Player*>(collision.gameBody);
        if (player == nullptr)
        {
            assert(false);
            return;
        }
        float angle = PE_AngleDeg(manifold.normal, PE_Vec2::down);
        if (fabsf(angle) > 125)
        {
            SetEnabled(false);
            SetToRespawn(true);
        }

        return;
    }
}

void Brick::OnRespawn()
{
}
