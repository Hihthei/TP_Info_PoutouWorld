#pragma once

#include "Settings.h"
#include "GameBody.h"

struct Tile
{
    enum class Type : int
    {
        EMPTY, GROUND, GGROUND, WOOD, GWOOD, ONE_WAY, GONE_WAY, SPIKE, GSPIKE,
        STEEP_SLOPE_L, GSTEEP_SLOPE_L, STEEP_SLOPE_R, GSTEEP_SLOPE_R,
        GENTLE_SLOPE_L, GGENTLE_SLOPE_L, GENTLE_SLOPE_R, GGENTLE_SLOPE_R,
    };
    Type type;
    int partIdx;
    PE_Collider *collider;
};

class StaticMap : public GameBody
{
public:
    StaticMap(Scene &scene, int width, int height);
    virtual ~StaticMap();

    void SetTile(int x, int y, Tile::Type type);
    void InitTiles();

    virtual void Render() override;
    virtual void Start() override;
    virtual void OnCollisionStay(GameCollision &collision) override;

private:

    RE_AtlasPart *m_woodPart;
    RE_AtlasPart* m_gwoodPart;
    RE_AtlasPart *m_oneWayPart;
    RE_AtlasPart* m_goneWayPart;
    RE_AtlasPart *m_terrainPart;
    RE_AtlasPart* m_gterrainPart;
    RE_AtlasPart *m_spikePart;
    RE_AtlasPart* m_gspikePart;
    RE_AtlasPart* m_steepslopel;
    RE_AtlasPart* m_gsteepslopel;
    RE_AtlasPart* m_steepsloper;
    RE_AtlasPart* m_gsteepsloper;
    RE_AtlasPart* m_gentleslopel;
    RE_AtlasPart* m_ggentleslopel;
    RE_AtlasPart* m_gentlesloper;
    RE_AtlasPart* m_ggentlesloper;

    Tile **m_tiles;
    int m_width;
    int m_height;

    Tile::Type GetTileType(int x, int y) const;
    bool IsGround(int x, int y) const;
};
