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

}

Nut::~Nut()
{
}

void Nut::Start()
{
    SetToRespawn(true);

    // Joue l'animation par défaut
    m_animator.PlayAnimation("Idle");

    // Crée le corps
    PE_World &world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::DYNAMIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "Nut";
    bodyDef.damping.SetZero();
    PE_Body *body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_CircleShape circle(PE_Vec2(0.0f, 0.45f), 0.45f);
    PE_ColliderDef colliderDef;
    colliderDef.friction = 0.005f;
    colliderDef.filter.categoryBits = CATEGORY_ENEMY ;
    colliderDef.filter.maskBits = CATEGORY_ENEMY | CATEGORY_PLAYER | CATEGORY_TERRAIN;
    colliderDef.shape = &circle;
    PE_Collider *collider = body->CreateCollider(colliderDef);

    // Endort le corps
    // Permet d'optimiser le calcul de la physique,
    // seuls les corps proches du joueur sont simulés
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
        m_state = State::IDLE;
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

    // DID : Mettre la noisette en mouvement à l'approche du joueur
    /*if (dist <= 5.0f && m_state == State::IDLE && position == GetStartPosition())
    {
        body->SetAwake(true);
        m_state = State::ATTACKING;
        if(player->GetPosition().x < position.x)
            body->SetVelocity(PE_Vec2(-3.0f, 10.0f));
        else
            body->SetVelocity(PE_Vec2(3.0f, 10.0f));
    }
    else if (position.y == GetStartPosition().y)
    {
        m_state = State::SPINNING;
        if (body->IsAwake() && dist <= 5.0f && m_state == State::SPINNING)
        {
            if (player->GetPosition().x < position.x)
                body->SetVelocity(PE_Vec2(-3.0f, 0.0f));
            else
                body->SetVelocity(PE_Vec2(3.0f, 0.0f));
        }
        else if (dist > 9.0f)
        {
            body->SetAwake(false);
            m_state = State::IDLE;
        }
    }  */

    if (dist > 6.0f)
    {
        body->SetAwake(false);
        return;
    }
    else if (dist <= 6.0f && m_state == State::IDLE)
    {
        // Le joueur est à moins de 5 tuiles de la noisette
        m_state = State::SPINNING;
        m_animator.PlayAnimation("Spinning");
        body->SetVelocity(PE_Vec2(-4.0f, 10.0f));
        body->SetAwake(false);
    } 

        // TODO : Mettre la noisette en mouvement à l'approche du joueur
       
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
    m_state = State::SPINNING;
    m_state = State::DYING;
}

void Nut::OnCollisionStay(GameCollision &collision)
{
    PE_Manifold &manifold = collision.manifold;
    PE_Collider *otherCollider = collision.otherCollider;

    // DID : Désactiver les collisions lorsque la noisette est en train de mourir

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
            player->Damage();
        }
        
        return;
    }
}
