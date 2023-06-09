#include "StaticMap.h"
#include "Camera.h"
#include "Player.h"

StaticMap::StaticMap(Scene &scene, int width, int height) :
    GameBody(scene, Layer::TERRAIN_BACKGROUND), m_width(width), m_height(height)
{
    m_name = "StaticMap";

    m_tiles = new Tile*[width];
    for (int x = 0; x < width; x++)
    {
        m_tiles[x] = new Tile[height];
        for (int y = 0; y < height; y++)
        {
            Tile &tile = m_tiles[x][y];
            tile.collider = nullptr;
            tile.partIdx = 0;
            tile.type = Tile::Type::EMPTY;
        }
    }

    RE_Atlas* atlas2 = scene.GetAssetManager().GetAtlas(AtlasID::GTERRAIN);

    m_gterrainPart = atlas2->GetPart("Terrain");
    AssertNew(m_gterrainPart);

    m_gwoodPart = atlas2->GetPart("Wood");
    AssertNew(m_gwoodPart);

    m_goneWayPart = atlas2->GetPart("OneWay");
    AssertNew(m_goneWayPart);

    m_gspikePart = atlas2->GetPart("Spike");
    AssertNew(m_gspikePart);

    m_gsteepsloper = atlas2->GetPart("Terrain");
    AssertNew(m_gsteepsloper);

    m_gsteepslopel = atlas2->GetPart("Terrain");
    AssertNew(m_gsteepslopel);

    m_ggentlesloper = atlas2->GetPart("Terrain");
    AssertNew(m_ggentlesloper);

    m_ggentleslopel = atlas2->GetPart("Terrain");
    AssertNew(m_ggentleslopel);

    RE_Atlas *atlas = scene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);

    m_woodPart = atlas->GetPart("Wood");
    AssertNew(m_woodPart);

    m_oneWayPart = atlas->GetPart("OneWay");
    AssertNew(m_oneWayPart);

    m_terrainPart = atlas->GetPart("Terrain");
    AssertNew(m_terrainPart);

    m_spikePart = atlas->GetPart("Spike");
    AssertNew(m_spikePart);

    m_steepsloper = atlas->GetPart("Terrain");
    AssertNew(m_steepsloper);

    m_steepslopel = atlas->GetPart("Terrain");
    AssertNew(m_steepslopel);

    m_gentlesloper = atlas->GetPart("Terrain");
    AssertNew(m_gentlesloper);

    m_gentleslopel = atlas->GetPart("Terrain");
    AssertNew(m_gentleslopel);

    // Couleur des colliders en debug
    m_debugColor.r = 255;
    m_debugColor.g = 200;
    m_debugColor.b = 0;
}

StaticMap::~StaticMap()
{
    if (m_tiles)
    {
        for (int x = 0; x < m_width; x++)
        {
            delete[] m_tiles[x];
        }
        delete[] m_tiles;
    }
}

void StaticMap::SetTile(int x, int y, Tile::Type type)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    {
        assert(false);
        return;
    }

    Tile &tile = m_tiles[x][y];
    tile.partIdx = 0;
    tile.type = type;
}

void StaticMap::InitTiles()
{
    for (int x = 0; x < m_width; x++)
    {
        for (int y = 0; y < m_height; y++)
        {
            Tile &tile = m_tiles[x][y];
            Tile::Type type = GetTileType(x, y);

            switch (type)
            {
            case Tile::Type::GROUND:
                if (IsGround(x, y + 1) && IsGround(x + 1, y) && (IsGround(x + 1, y + 1) == false))
                    tile.partIdx = 7;
                else if (IsGround(x, y + 1) && IsGround(x - 1, y) && IsGround(x - 1, y + 1) == false)
                    tile.partIdx = 6;
                else if (IsGround(x, y + 1) && IsGround(x + 1, y) && IsGround(x - 1, y))
                {
                    tile.partIdx = 4;
                }
                else if (IsGround(x + 1, y) == false)
                {
                    if (IsGround(x - 1, y) && IsGround(x, y + 1) == false)
                        tile.partIdx = 2;
                    else if (IsGround(x, y + 1))
                        tile.partIdx = 5;
                    else tile.partIdx = 1;
                }
                else if (IsGround(x - 1, y) == false)
                {
                    if (IsGround(x + 1, y) && IsGround(x, y + 1) == false)
                        tile.partIdx = 0;
                    else if (IsGround(x, y + 1))
                        tile.partIdx = 3;
                    else tile.partIdx = 1;
                }
                else
                {
                    tile.partIdx = 1;
                }
                break;
            case Tile::Type::GGROUND:
                if (IsGround(x, y + 1) && IsGround(x + 1, y) && (IsGround(x + 1, y + 1) == false))
                    tile.partIdx = 7;
                else if (IsGround(x, y + 1) && IsGround(x - 1, y) && IsGround(x - 1, y + 1) == false)
                    tile.partIdx = 6;
                else if (IsGround(x, y + 1) && IsGround(x + 1, y) && IsGround(x - 1, y))
                {
                    tile.partIdx = 4;
                }
                else if (IsGround(x + 1, y) == false)
                {
                    if (IsGround(x - 1, y) && IsGround(x, y + 1) == false)
                        tile.partIdx = 0;
                    else if (IsGround(x, y + 1))
                        tile.partIdx = 5;
                    else tile.partIdx = 1;
                }
                else if (IsGround(x - 1, y) == false)
                {
                    if (IsGround(x + 1, y) && IsGround(x, y + 1) == false)
                        tile.partIdx = 2;
                    else if (IsGround(x, y + 1))
                        tile.partIdx = 3;
                    else tile.partIdx = 1;
                }
                else
                {
                    tile.partIdx = 1;
                }
                break;
            case Tile::Type::STEEP_SLOPE_R:
                if (IsGround(x , y + 1))
                    tile.partIdx = 11;
                else tile.partIdx = 10;
                break;
            case Tile::Type::STEEP_SLOPE_L:
                if (IsGround(x , y + 1))
                    tile.partIdx = 8;
                else tile.partIdx = 9;
                break;
            case Tile::Type::GSTEEP_SLOPE_R:
                if (IsGround(x, y + 1))
                    tile.partIdx = 11;
                else tile.partIdx = 10;
                break;
            case Tile::Type::GSTEEP_SLOPE_L:
                if (IsGround(x, y + 1))
                    tile.partIdx = 8;
                else tile.partIdx = 9;
                break;
            case Tile::Type::GENTLE_SLOPE_L:
                if (IsGround(x - 1, y) && IsGround(x + 1, y) && IsGround(x, y + 1) == false)
                    tile.partIdx = 12;
                else if (IsGround(x, y + 1))
                    tile.partIdx = 14;
                else
                    tile.partIdx = 13;
                break;
            case Tile::Type::GENTLE_SLOPE_R:
                if (IsGround(x + 1, y) && IsGround(x - 1, y) && IsGround(x, y + 1) == false )
                    tile.partIdx = 16;
                else if (IsGround(x, y + 1))
                    tile.partIdx = 17;
                else
                    tile.partIdx = 15;
                break;
            case Tile::Type::GGENTLE_SLOPE_L:
                if (IsGround(x - 1, y) && IsGround(x + 1, y) && IsGround(x, y + 1) == false)
                    tile.partIdx = 12;
                else if (IsGround(x, y + 1))
                    tile.partIdx = 14;
                else
                    tile.partIdx = 13;
                break;
            case Tile::Type::GGENTLE_SLOPE_R:
                if (IsGround(x + 1, y) && IsGround(x - 1, y) && IsGround(x, y + 1) == false)
                    tile.partIdx = 16;
                else if (IsGround(x, y + 1))
                    tile.partIdx = 17;
                else
                    tile.partIdx = 15;
                break;
            default:
                tile.partIdx = 0;
                break;
            }
            
        }
    }
}

void StaticMap::Render()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    Camera *camera = m_scene.GetActiveCamera();

    PE_AABB view = camera->GetWorldView();
    int x0 = (int)view.lower.x - 1;
    int y0 = (int)view.lower.y - 1;
    int x1 = (int)view.upper.x + 2;
    int y1 = (int)view.upper.y + 2;

    x0 = PE_Max(x0, 0);
    y0 = PE_Max(y0, 0);
    x1 = PE_Min(x1, m_width);
    y1 = PE_Min(y1, m_height);

    for (int x = x0; x < x1; ++x)
    {
        for (int y = y0; y < y1; ++y)
        {
            Tile &tile = m_tiles[x][y];
            PE_Collider *collider = tile.collider;

            PE_Vec2 position((float)x, (float)y);
            SDL_FRect dst = { 0 };

            camera->WorldToView(position, dst.x, dst.y);
            float scale = camera->GetWorldToViewScale();
            dst.w = scale * 1.0f;
            dst.h = scale * 1.0f;

            switch (tile.type)
            {
            case Tile::Type::GROUND:
                m_terrainPart->RenderCopyF(tile.partIdx, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::GGROUND:
                m_gterrainPart->RenderCopyF(tile.partIdx, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::STEEP_SLOPE_L:
                m_terrainPart->RenderCopyF(tile.partIdx, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::STEEP_SLOPE_R:
                m_terrainPart->RenderCopyF(tile.partIdx, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::GENTLE_SLOPE_L:
                m_terrainPart->RenderCopyF(tile.partIdx, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::GENTLE_SLOPE_R:
                m_terrainPart->RenderCopyF(tile.partIdx, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::GSTEEP_SLOPE_L:
                m_gterrainPart->RenderCopyF(tile.partIdx, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::GSTEEP_SLOPE_R:
                m_gterrainPart->RenderCopyF(tile.partIdx, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::GGENTLE_SLOPE_L:
                m_gterrainPart->RenderCopyF(tile.partIdx, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::GGENTLE_SLOPE_R:
                m_gterrainPart->RenderCopyF(tile.partIdx, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::WOOD:
                m_woodPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::ONE_WAY:
                m_oneWayPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::SPIKE:
                m_spikePart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::GWOOD:
                m_gwoodPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::GONE_WAY:
                m_goneWayPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case Tile::Type::GSPIKE:
                m_gspikePart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            default:
                break;
            }
        }
    }
}

void StaticMap::Start()
{
    PE_World &world = m_scene.GetWorld();
    PE_Body *body = NULL;

    // Crée le corps
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position.SetZero();
    bodyDef.name = (char *)"StaticMap";
    body = world.CreateBody(bodyDef);
    AssertNew(body);
    SetBody(body);

    // Crée les colliders
    PE_Vec2 vertices[3];
    PE_PolygonShape polygon;
    PE_ColliderDef colliderDef;

    for (int x = 0; x < m_width; ++x)
    {
        for (int y = 0; y < m_height; ++y)
        {
            Tile &tile = m_tiles[x][y];
            if (tile.type == Tile::Type::EMPTY)
            {
                continue;
            }

            PE_Vec2 position((float)x, (float)y);
            bool newCollider = true;
            colliderDef.SetDefault();
            colliderDef.shape = &polygon;
            colliderDef.friction = 0.5f;
            colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
            colliderDef.userData.id = 0;

            switch (tile.type)
            {
            case Tile::Type::ONE_WAY:
                colliderDef.isOneWay = true;
                polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
                break;
            case Tile::Type::GONE_WAY:
                colliderDef.isOneWay = true;
                colliderDef.friction = 0.005f;
                polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
                break;

            case Tile::Type::GROUND:
                polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
                break;
            case Tile::Type::GGROUND:
                colliderDef.friction = 0.005f;
                polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
                break;
            case Tile::Type::WOOD:
                polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
                break;
            case Tile::Type::GWOOD:
                colliderDef.friction = 0.005f;
                polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
                break;

            case Tile::Type::SPIKE:
                colliderDef.userData.id = 1;

                vertices[0] = position + PE_Vec2(0.1f, 0.0f);
                vertices[1] = position + PE_Vec2(0.9f, 0.0f);
                vertices[2] = position + PE_Vec2(0.5f, 0.8f);
                polygon.SetVertices(vertices, 3);
                break;
            case Tile::Type::GSPIKE:
                colliderDef.userData.id = 2;

                vertices[0] = position + PE_Vec2(0.1f, 0.0f);
                vertices[1] = position + PE_Vec2(0.9f, 0.0f);
                vertices[2] = position + PE_Vec2(0.5f, 0.8f);
                polygon.SetVertices(vertices, 3);
                break;
            case Tile::Type::STEEP_SLOPE_R:

                colliderDef.userData.id = 10;

                vertices[0] = position + PE_Vec2(1.0f, 0.0f);
                vertices[1] = position + PE_Vec2(1.0f, 1.0f);
                vertices[2] = position + PE_Vec2(0.0f, 0.0f);
                polygon.SetVertices(vertices, 3);
                break;
            case Tile::Type::STEEP_SLOPE_L:
                colliderDef.userData.id = 9;

                vertices[0] = position + PE_Vec2(1.0f, 0.0f);
                vertices[1] = position + PE_Vec2(0.0f, 1.0f);
                vertices[2] = position + PE_Vec2(0.0f, 0.0f);
                polygon.SetVertices(vertices, 3);
                break;
            case Tile::Type::GSTEEP_SLOPE_R:
                colliderDef.friction = 0.005f;
                colliderDef.userData.id = 11;

                vertices[0] = position + PE_Vec2(1.0f, 0.0f);
                vertices[1] = position + PE_Vec2(1.0f, 1.0f);
                vertices[2] = position + PE_Vec2(0.0f, 0.0f);
                polygon.SetVertices(vertices, 3);
                break;
            case Tile::Type::GSTEEP_SLOPE_L:
                colliderDef.friction = 0.005f;
                colliderDef.userData.id = 8;

                vertices[0] = position + PE_Vec2(1.0f, 0.0f);
                vertices[1] = position + PE_Vec2(0.0f, 1.0f);
                vertices[2] = position + PE_Vec2(0.0f, 0.0f);
                polygon.SetVertices(vertices, 3);
                break;
            case Tile::Type::GENTLE_SLOPE_L:
                if (tile.partIdx == 12)
                {
                    vertices[0] = position + PE_Vec2(1.0f, 0.5f);
                    vertices[1] = position + PE_Vec2(0.0f, 1.0f);
                    vertices[2] = position + PE_Vec2(0.0f, 0.5f);
                    polygon.SetVertices(vertices, 3);

                }
                else if (tile.partIdx == 13)
                {
                    vertices[0] = position + PE_Vec2(1.0f, 0.0f);
                    vertices[1] = position + PE_Vec2(0.0f, 0.5f);
                    vertices[2] = position + PE_Vec2(0.0f, 0.0f);
                    polygon.SetVertices(vertices, 3);
                }
                else
                {
                    polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
                }
                break;
            case Tile::Type::GGENTLE_SLOPE_L:
                colliderDef.friction = 0.2f;
                if (tile.partIdx == 12)
                {
                    vertices[0] = position + PE_Vec2(1.0f, 0.5f);
                    vertices[1] = position + PE_Vec2(0.0f, 1.0f);
                    vertices[2] = position + PE_Vec2(0.0f, 0.5f);
                    polygon.SetVertices(vertices, 3);

                }
                else if (tile.partIdx == 13)
                {
                    vertices[0] = position + PE_Vec2(1.0f, 0.0f);
                    vertices[1] = position + PE_Vec2(0.0f, 0.5f);
                    vertices[2] = position + PE_Vec2(0.0f, 0.0f);
                    polygon.SetVertices(vertices, 3);
                }
                else
                {
                    polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
                }
                break;
            case Tile::Type::GENTLE_SLOPE_R:
                if (tile.partIdx == 16)
                {
                    vertices[0] = position + PE_Vec2(1.0f, 0.5f);
                    vertices[1] = position + PE_Vec2(1.0f, 1.0f);
                    vertices[2] = position + PE_Vec2(0.0f, 0.5f);
                    polygon.SetVertices(vertices, 3);
                }
                else if (tile.partIdx == 15)
                {
                    vertices[0] = position + PE_Vec2(1.0f, 0.0f);
                    vertices[1] = position + PE_Vec2(1.0f, 0.5f);
                    vertices[2] = position + PE_Vec2(0.0f, 0.0f);
                    polygon.SetVertices(vertices, 3);
                }
                else
                {
                    polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
                }
                break;
            case Tile::Type::GGENTLE_SLOPE_R:
                colliderDef.friction = 0.2f;
                if (tile.partIdx == 16)
                {
                    vertices[0] = position + PE_Vec2(1.0f, 0.5f);
                    vertices[1] = position + PE_Vec2(1.0f, 1.0f);
                    vertices[2] = position + PE_Vec2(0.0f, 0.5f);
                    polygon.SetVertices(vertices, 3);
                }
                else if (tile.partIdx == 15)
                {
                    vertices[0] = position + PE_Vec2(1.0f, 0.0f);
                    vertices[1] = position + PE_Vec2(1.0f, 0.5f);
                    vertices[2] = position + PE_Vec2(0.0f, 0.0f);
                    polygon.SetVertices(vertices, 3);
                }
                else
                {
                    polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
                }
                break;
            default:
                newCollider = false;
                break;
            }
            if (newCollider)
            {
                tile.collider = body->CreateCollider(colliderDef);
                AssertNew(tile.collider);
            }
            else
            {
                tile.collider = nullptr;
            }
        }
    }

    // Limite à gauche du monde
    polygon.SetAsBox(-1.0f, -2.0f, 0.5f, (float)m_height + 10.0f);
    colliderDef.SetDefault();
    colliderDef.friction = 0.0f;
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
    colliderDef.shape = &polygon;
    body->CreateCollider(colliderDef);

    // Limite à droite du monde
    polygon.SetAsBox((float)m_width-0.5, -2.0f, (float)m_width + 1.0f, (float)m_height + 10.0f);
    colliderDef.SetDefault();
    colliderDef.friction = 0.0f;
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
    colliderDef.shape = &polygon;
    body->CreateCollider(colliderDef);
}

void StaticMap::OnCollisionStay(GameCollision &collision)
{
    // On vérifie que la collision concerne une pique
    if (collision.collider->GetUserData().id != 1)
        return;

    if (collision.otherCollider->CheckCategory(CATEGORY_PLAYER))
    {
        Player *player = dynamic_cast<Player *>(collision.gameBody);
        if (player == nullptr)
        {
            assert(false);
            return;
        }

        if (!player->GetInvicibleStatue())
        {
            player->Damage(-1);
            player->Bounce();
        }
    }
}

Tile::Type StaticMap::GetTileType(int x, int y) const
{
    if (x < 0 || x >= m_width || y < 0)
        return Tile::Type::GROUND;
    else if (y >= m_height)
        return Tile::Type::EMPTY;
    else
        return m_tiles[x][y].type;
}

bool StaticMap::IsGround(int x, int y) const
{
    switch (GetTileType(x, y))
    {
    case Tile::Type::GROUND:
    case Tile::Type::STEEP_SLOPE_L:
    case Tile::Type::STEEP_SLOPE_R:
    case Tile::Type::GENTLE_SLOPE_L:
    case Tile::Type::GENTLE_SLOPE_R:
    case Tile::Type::GGROUND:
    case Tile::Type::GSTEEP_SLOPE_L:
    case Tile::Type::GSTEEP_SLOPE_R:
    case Tile::Type::GGENTLE_SLOPE_L:
    case Tile::Type::GGENTLE_SLOPE_R:
        return true;
    default:
        return false;
    }
}
