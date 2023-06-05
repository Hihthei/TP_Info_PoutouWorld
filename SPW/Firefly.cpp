#include "Firefly.h"
#include "Scene.h"

Firefly::Firefly(Scene& scene, Layer layer) :
    Collectable(scene, layer)
{
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
    bodyDef.damping.SetZero();
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_CircleShape circle(PE_Vec2(0.0f, 0.15f), 0.15f);
    PE_ColliderDef colliderDef;
    colliderDef.friction = 0.0f;
    colliderDef.filter.categoryBits = CATEGORY_COLLECTABLE;
    colliderDef.filter.maskBits = CATEGORY_PLAYER;
    colliderDef.shape = &circle;
    PE_Collider* collider = body->CreateCollider(colliderDef);

    // Endort le corps
    // Permet d'optimiser le calcul de la physique,
    // seuls les corps proches du joueur sont simulés
    body->SetAwake(false);
}

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

void Firefly::Collect(Collectable* Firefly)
{
    Player* player = dynamic_cast<Player*>(damager);
    if (player)
        player->Bounce();

    m_state = State::PICKED;
}