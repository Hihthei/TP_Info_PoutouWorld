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
    //Joue l'animation par défaut
    //TODO

    //Crée le corps
    PE_World &world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::DYNAMIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(5.0f, 10.0f);
    bodyDef.name = (char*)"Firefly";
    bodyDef.damping.SetZero();
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Création du collider
    PE_ColliderDef colliderDef;

    PE_CapsuleShape capsule(PE_Vec2(0.09f, 0.23f), PE_Vec2(0.09f, 0.78f), 0.23f);
    colliderDef.friction = 2.5f;
    colliderDef.filter.categoryBits = CATEGORY_PLAYER;
    colliderDef.shape = &capsule;
    PE_Collider* collider = body->CreateCollider(colliderDef);

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