#include "Boss.h"
#include "nutdebile.h"
#include "Scene.h"
#include "Camera.h"
#include "LevelScene.h"
#include "Graphics.h"
#include "Utils.h"

Boss::Boss(Scene& scene) :
    Enemy(scene), b_animator(), b_state(State::IDLE), b_onScreen(false), b_heartCount(5), b_right(true), b_left(false)
{
    m_name = "Boss";

    RE_Atlas* atlas = scene.GetAssetManager().GetAtlas(AtlasID::BOSS);
    AssertNew(atlas);
    RE_AtlasPart* part;
    RE_TexAnim* idleAnim;
    RE_TexAnim* invincibleAnim;

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

    // Crée le collider droit
    PE_CapsuleShape capsule(PE_Vec2(0.09f, 0.40f), PE_Vec2(0.09f, 0.58f), 0.40f);
    PE_ColliderDef colliderDef;
    colliderDef.friction = 0.000f;
    colliderDef.filter.categoryBits = CATEGORY_ENEMY;
    colliderDef.filter.maskBits = CATEGORY_ENEMY | CATEGORY_PLAYER | CATEGORY_TERRAIN;
    colliderDef.shape = &capsule;
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
        Kill();
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

    b_skillsTimer += m_scene.GetFixedTimeStep();
    b_invincibleTimer += m_scene.GetFixedTimeStep();

    // Chrono si le joueur et invincible

    if (b_state == State::INVINCIBLE)
    {
        b_invicibleState = true;
        b_animator.PlayAnimation("Idle");

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
    
    float maxHValue = player->GetPosition().y + 5.0f;

    float maxXLeftValue = player->GetPosition().x - 5.0f;

    float maxXRightValue = player->GetPosition().x + 5.0f;

    float distX = PE_DistanceX(position, player->GetPosition());

    if (distX < -7.0f)
    {
        b_right = true;
        b_left = false;
    }
    else if (distX > 7.0f)
    {
        b_right = false;
        b_left = true;
    }

    position.x = PE_Clamp(position.x, maxXLeftValue, maxXRightValue);

    if (b_right)
    {
        body->SetVelocity(PE_Vec2(3.0f, 0.0f));
    }
    else if (b_left)
    {
        body->SetVelocity(PE_Vec2(-3.0f, 0.0f));
    }

    if (b_skillsTimer > 10.0f)
    {
        b_skillsTimer = 0.0f;
        nutdebile* nut = new nutdebile(m_scene);

        nut->SetSpawnStatue(true);

        PE_Vec2 setNutPosition = position + PE_Vec2(0.0f, -0.5f);

        nut->SetStartPosition(setNutPosition);
    }

    if (b_invincibleTimer > 15.0f)
        b_state = State::INVINCIBLE;
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
        
        float angleRight = PE_SignedAngleDeg(manifold.normal, (PE_Vec2::right));
        if (fabsf(angleRight) <= 10)
        {
            b_right = false;
            b_left = true;
        }
        /*
        float angleLeft = PE_SignedAngleDeg(manifold.normal, (PE_Vec2::left));
        if (fabsf(angleLeft) <= 10)
        {
            b_right = true;
            b_left = false;
        }*/
        
        collision.SetEnabled(false);
        return;
    }
    else if (otherCollider->CheckCategory(CATEGORY_ENEMY))
    {
        collision.SetEnabled(false);
        return;
    } 
}
