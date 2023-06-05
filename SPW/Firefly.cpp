#include "Firefly.h"
#include "Scene.h"

Firefly::Firefly(Scene& scene, Layer layer) :
    Collectable(scene, layer)
{
}

Firefly::~Firefly()
{
}

void Firefly::Collect(Collectable* Firefly)
{
}