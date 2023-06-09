#include "Boss.h"
#include "Scene.h"
#include "Camera.h"
#include "LevelScene.h"
#include "Graphics.h"
#include "Utils.h"

Boss::Boss(Scene& scene) :
    Enemy(scene), b_animator(), b_state(State::IDLE), b_onScreen(false), b_heartCount(5)
{
    m_name = "Boss";

    RE_Atlas* atlas = scene.GetAssetManager().GetAtlas(AtlasID::BOSS);
    AssertNew(atlas);
    RE_AtlasPart* part;
    RE_TexAnim* idleAnim;

    // Animation "Idle"
    part = atlas->GetPart("Idle");
    AssertNew(part);
    idleAnim = new RE_TexAnim(b_animator, "Idle", part);
    idleAnim->SetCycleCount(0);
}

Boss::~Boss()
{
}

void Boss::Start()
{
    SetToRespawn(true);

    // Joue l'animation par défaut
    b_animator.PlayAnimation("Idle");

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

    // Chrono si le joueur et invincible

    if (b_state == State::INVINCIBLE)
    {
        b_invicibleState = true;

        b_state = State::INVINCIBLE_DELAY;
    }
    else if (b_invincibleDelay <= 2.0f && b_state == State::INVINCIBLE_DELAY)
        b_invincibleDelay += m_scene.GetFixedTimeStep();
    else
    {
        b_state = State::IDLE;
        b_invincibleDelay = 0.0f;
        b_invicibleState = false;
    }

    if (b_invicibleState)
        printf("%.2f\n", b_invincibleDelay);
    
    float maxHValue = player->GetPosition().y + 5.0f;

    float dist = PE_Distance(position, player->GetPosition());
    
    if (dist > 24.0f)
    {
        body->SetAwake(false);

        return;
    }
    if (dist <= 14.0f)
    {
        if (b_state != State::DEAD && b_state != State::DYING && position.y < (maxHValue - 2))
            body->SetVelocity(PE_Vec2(0.0f, 0.0f));
    }

    if (b_state == State::DEAD)
    {
        body->SetGravityScale(0.4f);
        b_animator.PlayAnimation("Idle");
        b_state = State::DYING;
    }
    
}

void Boss::Render()
{
    SDL_Renderer* renderer = m_scene.GetRenderer();
    Camera* camera = m_scene.GetActiveCamera();

    b_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.h = 1.0f * scale;
    rect.w = 1.0f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    b_animator.RenderCopyF(&rect, RE_Anchor::SOUTH);
}

void Boss::OnRespawn()
{
    b_state = State::IDLE;

    SetToRespawn(true);
    SetBodyEnabled(true);
    SetEnabled(true);

    PE_Body* body = GetBody();
    body->SetPosition(GetStartPosition() + PE_Vec2(0.5f, 0.0f));
    body->SetVelocity(PE_Vec2::zero);
    body->ClearForces();

    b_animator.StopAnimations();
    b_animator.PlayAnimation("Idle");
}

void Boss::AddHeart(int count)
{
    if (b_heartCount == 1)
    {
        Kill();
        return;
    }

    b_heartCount += count;

    printf("Il reste %d coeurs au boss\n", b_heartCount);
}

void Boss::Kill()
{
    m_scene.Quit();
}

void Boss::Damage(GameBody* damager)
{
    Player* player = dynamic_cast<Player*>(damager);
    if (player)
        player->Bounce();

    if (b_invicibleState)
    {
        return;
    }

    AddHeart(-1);

    b_state = State::INVINCIBLE;
}

void Boss::OnCollisionEnter(GameCollision& collision)
{
    const PE_Manifold& manifold = collision.manifold;
    PE_Collider* otherCollider = collision.otherCollider;


    
     if (otherCollider->CheckCategory(CATEGORY_PLAYER))
     {
         if (b_invicibleState)
             collision.SetEnabled(false);
             
     }
}

void Boss::OnCollisionStay(GameCollision& collision)
{
    PE_Manifold& manifold = collision.manifold;
    PE_Collider* otherCollider = collision.otherCollider;
    

    collision.SetEnabled(false);


    
    // Collision avec le joueur
    if (otherCollider->CheckCategory(CATEGORY_PLAYER))
    {
        if (b_invicibleState)
        {
            printf("ici c'est invincible\n");
            return;
        }

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
                player->Damage(-1);
        }
        return;
    }
    else if (otherCollider->CheckCategory(CATEGORY_TERRAIN))
    {
        collision.SetEnabled(false);
        return;
    }
    else if (otherCollider->CheckCategory(CATEGORY_ENEMY))
    {
        collision.SetEnabled(false);
        return;
    } 
}
