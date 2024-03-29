#include "Nut.h"
#include "Scene.h"
#include "Camera.h"
#include "LevelScene.h"
#include "Graphics.h"
Nut::Nut(Scene &scene) :
    Enemy(scene), m_animator(), m_state(State::IDLE)
{
    m_name = "Nut";

    RE_Atlas *atlas = scene.GetAssetManager().GetAtlas(AtlasID::ENEMY);
    AssertNew(atlas);
    RE_AtlasPart* part;
    RE_TexAnim* idleAnim;
    RE_TexAnim* spinAnim;
    RE_TexAnim* dyAnim;

    // Animation "Idle"
    part = atlas->GetPart("NutIdle");
    AssertNew(part);
    idleAnim = new RE_TexAnim(m_animator, "Idle", part);
    idleAnim->SetCycleCount(0);

    // Animation "Spin"
    part = atlas->GetPart("NutSpinning");
    AssertNew(part);
    spinAnim = new RE_TexAnim(m_animator, "Spinning", part);
    spinAnim->SetCycleCount(0);
    spinAnim->SetCycleTime(0.4f);

    // Animation "Dying"
    part = atlas->GetPart("NutDying");
    AssertNew(part);
    dyAnim = new RE_TexAnim(m_animator, "Dying", part);
    dyAnim->SetCycleCount(0);
    dyAnim->SetCycleTime(0.4f);


}

Nut::~Nut()
{
}

void Nut::Start()
{
    SetToRespawn(true);

    // Joue l'animation par d�faut
    m_animator.PlayAnimation("Idle");

    // Cr�e le corps
    PE_World &world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::DYNAMIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.gravityScale = 5.0f;
    bodyDef.name = "Nut";
    bodyDef.damping.SetZero();
    PE_Body *body = world.CreateBody(bodyDef);
    SetBody(body);

    // Cr�e le collider
    PE_CircleShape circle(PE_Vec2(0.0f, 0.45f), 0.45f);
    PE_ColliderDef colliderDef;
    colliderDef.friction = 0.000f;
    colliderDef.filter.categoryBits = CATEGORY_ENEMY ;
    colliderDef.filter.maskBits = CATEGORY_ENEMY | CATEGORY_PLAYER | CATEGORY_TERRAIN;
    colliderDef.shape = &circle;
    PE_Collider *collider = body->CreateCollider(colliderDef);

    // Endort le corps
    // Permet d'optimiser le calcul de la physique,
    // seuls les corps proches du joueur sont simul�s
    body->SetAwake(false);
}

void Nut::FixedUpdate()
{
    
    PE_Body *body = GetBody();
    PE_Vec2 position = body->GetPosition();
    PE_Vec2 velocity = body->GetLocalVelocity();

    // Tue la noisette si elle tombe dans un trou
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

    LevelScene *levelScene = dynamic_cast<LevelScene *>(&m_scene);
    if (levelScene == nullptr)
    {
        assert(false);
        return;
    }

    Player *player = levelScene->GetPlayer();

    float dist = PE_Distance(position, player->GetPosition());

    if (dist > 24.0f)
    {
        body->SetAwake(false);
        

        return;
    }
    if (dist <= 14.0f)
    {
        
        // Le joueur est � moins de 5 tuiles de la noisette pour la premiere fois
        if (m_state == State::IDLE )
        {
            m_state = State::SPINNING;
            m_animator.PlayAnimation("Spinning");
            if (player->GetPosition().x < position.x)
                body->SetVelocity(PE_Vec2(-1.0f, 10.0f));
            else
                body->SetVelocity(PE_Vec2(1.0f, 10.0f));

            

            m_state = State::SPINNING;
        }
        //le joueur est de nouveau proche de la noisette
        else if((position.x <= ((GetStartPosition().x)-0.5f) || position.x >= ((GetStartPosition().x) + 3.0f))&& m_state != State::DYING )
        {
            if (player->GetPosition().x < position.x)
                body->SetVelocity(PE_Vec2(-3.0f, 0.0f));
            else
                body->SetVelocity(PE_Vec2(3.0f, 0.0f));
        }
    } 

    if (m_state == State::DYING2)
    {
        body->SetVelocity(PE_Vec2(0.0f, 7.0f));
        m_animator.PlayAnimation("Dying");
        m_state = State::DYING;
    }
       
}

void Nut::Render()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    Camera *camera = m_scene.GetActiveCamera();

    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.h = 1.0f * scale;
    rect.w = 1.0f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    m_animator.RenderCopyF(&rect, RE_Anchor::SOUTH);
}

void Nut::OnRespawn()
{
    m_state = State::IDLE;

    SetToRespawn(true);
    SetBodyEnabled(true);
    SetEnabled(true);

    PE_Body *body = GetBody();
    body->SetPosition(GetStartPosition() + PE_Vec2(0.5f, 0.0f));
    body->SetVelocity(PE_Vec2::zero);
    body->ClearForces();

    m_animator.StopAnimations();
    m_animator.PlayAnimation("Idle");
}

void Nut::Damage(GameBody *damager)
{
    // DID
    Player* player = dynamic_cast<Player*>(damager);
    if (player) player->Bounce();
    m_animator.PlayAnimation("Idle");
    m_state = State::DYING2;
}

void Nut::OnCollisionStay(GameCollision &collision)
{
    PE_Manifold &manifold = collision.manifold;
    PE_Collider *otherCollider = collision.otherCollider;

    // DID : D�sactiver les collisions lorsque la noisette est en train de mourir

    if (m_state == State::DYING)
    {
        collision.SetEnabled(false);
        return;
    }

    // Collision avec le joueur
    if (otherCollider->CheckCategory(CATEGORY_PLAYER))
    {
        Player *player = dynamic_cast<Player *>(collision.gameBody);
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
}
