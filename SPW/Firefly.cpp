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
}

void Firefly::Render()
{
}

void Firefly::OnRespawn()
{
}

void Firefly::Collect(Collectable* Firefly)
{
}