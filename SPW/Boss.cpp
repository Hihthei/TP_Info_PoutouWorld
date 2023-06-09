#include "Boss.h"
#include "Scene.h"
#include "Camera.h"
#include "LevelScene.h"
#include "Graphics.h"
#include "Utils.h"

Boss::Boss(Scene& scene) :
    Enemy(scene), m_animator(), m_state(State::IDLE), b_onScreen(false)
{
    m_name = "Boss";

    RE_Atlas* atlas = scene.GetAssetManager().GetAtlas(AtlasID::BOSS);
    AssertNew(atlas);
    RE_AtlasPart* part;
    RE_TexAnim* idleAnim;

    // Animation "Idle"
    part = atlas->GetPart("Idle");
    AssertNew(part);
    idleAnim = new RE_TexAnim(m_animator, "Idle", part);
    idleAnim->SetCycleCount(0);
}

Boss::~Boss()
{
}

void Boss::Start()
{
    SetToRespawn(true);

    // Joue l'animation par défaut
    m_animator.PlayAnimation("Idle");

    // Crée le corps
    PE_World& world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::DYNAMIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "Boss";
    bodyDef.damping.SetZero();
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_CircleShape circle(PE_Vec2(0.0f, 0.45f), 0.45f);
    PE_ColliderDef colliderDef;
    colliderDef.friction = 0.000f;
    colliderDef.filter.categoryBits = CATEGORY_ENEMY;
    colliderDef.filter.maskBits = CATEGORY_ENEMY | CATEGORY_PLAYER | CATEGORY_TERRAIN;
    colliderDef.shape = &circle;
    PE_Collider* collider = body->CreateCollider(colliderDef);

    body->SetGravityScale(0.0f);

    // Endort le corps
    // Permet d'optimiser le calcul de la physique,
    // seuls les corps proches du joueur sont simulés
    body->SetAwake(false);
}

void Boss::FixedUpdate()
{
    PE_Body* body = GetBody();
    PE_Vec2 position = body->GetPosition();
    PE_Vec2 velocity = body->GetLocalVelocity();

    // Tue le boss si il tombe dans un trou
    if (position.y < -2.0f)
    {
        SetEnabled(false);
        SetToRespawn(true);
        return;
    }

    if (body->IsAwake() == false)
    {
        return;
    }

    LevelScene* levelScene = dynamic_cast<LevelScene*>(&m_scene);
    if (levelScene == nullptr)
    {
        assert(false);
        return;
    }

    Player* player = levelScene->GetPlayer();

    b_globalTimer += m_scene.GetFixedTimeStep();

    

    
    float maxHValue = player->GetPosition().y + 7.0f;

    float dist = PE_Distance(position, player->GetPosition());
    
    if (dist > 24.0f)
    {
        body->SetAwake(false);

        return;
    }
    if (dist <= 14.0f)
    {
        if (m_state != State::DEAD && m_state != State::DYING && position.y < (maxHValue - 2))
        {
            body->SetVelocity(PE_Vec2(0.0f, 3.0f));
        }
        else if (m_state != State::DEAD && m_state != State::DYING && position.y > (maxHValue))
            body->SetVelocity(PE_Vec2(0.0f, -3.0f));
    }

    if (m_state == State::DEAD)
    {
        body->SetVelocity(PE_Vec2(0.0f, 7.0f));
        m_animator.PlayAnimation("Idle");
        m_state = State::DYING;
    }
    
}

void Boss::Render()
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

void Boss::OnRespawn()
{
    m_state = State::IDLE;

    SetToRespawn(true);
    SetBodyEnabled(true);
    SetEnabled(true);

    PE_Body* body = GetBody();
    body->SetPosition(GetStartPosition() + PE_Vec2(0.5f, 0.0f));
    body->SetVelocity(PE_Vec2::zero);
    body->ClearForces();

    m_animator.StopAnimations();
    m_animator.PlayAnimation("Idle");
}

void Boss::Damage(GameBody* damager)
{
    // DID
    Player* player = dynamic_cast<Player*>(damager);
    if (player) player->Bounce();
    m_animator.PlayAnimation("Idle");
    m_state = State::DEAD;
}

void Boss::OnCollisionStay(GameCollision& collision)
{
    PE_Manifold& manifold = collision.manifold;
    PE_Collider* otherCollider = collision.otherCollider;

    // DID : Désactiver les collisions lorsque la noisette est en train de mourir

    if (m_state == State::DYING)
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
            if (player->GetStatePlayer())
                player->Damage(0);
        }
        return;
    }
    else if (otherCollider->CheckCategory(CATEGORY_TERRAIN))
    {
        collision.SetEnabled(false);
        return;
    }
}
