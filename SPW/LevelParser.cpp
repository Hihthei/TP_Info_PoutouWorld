#include "LevelParser.h"
#include "StaticMap.h"
#include "Brick.h"
#include "sfalling.h"
#include "lfalling.h"
#include "lright.h"
#include "sright.h"
#include "sleft.h"
#include "lleft.h"
#include "LevelScene.h"
#include "Checkpoint.h"
#include "Nut.h"
#include "Boss.h"
#include "Totem.h"
#include "nutdebile.h"
#include "Firefly.h"
#include "Heart.h"
#include "LevelEnd.h"
#include "Bonus.h"
#include "Camera.h"

// DID : Ajouter le fichier "Firefly.h"

LevelParser::LevelParser(const std::string &path)
{
    FILE *levelFile = fopen(path.c_str(), "rb");
    AssertNew(levelFile);

    bool isValidChar[128] = { 0 };
    isValidChar['\n'] = true;
    for (int i = '!'; i < '~'; i++)
    {
        isValidChar[i] = true;
    }

    fseek(levelFile, 0, SEEK_END);
    long fileSize = ftell(levelFile);
    rewind(levelFile);

    char *buffer = new char[fileSize];
    fread(buffer, 1, fileSize, levelFile);
    fclose(levelFile);
    levelFile = NULL;

    int height = 0;
    int width = 0;
    int i;
    for (i = 0; i < fileSize; i++)
    {
        char c = buffer[i];
        if (isValidChar[(int)c])
        {
            if (c == '\n')
            {
                height = 1;
                break;
            }
            else
            {
                width++;
            }
        }
        else
        {
            printf("INFO - Invalid char (%c)\n", c);
        }
    }
    if (height == 0)
    {
        printf("ERROR - Incorrect level file\n");
        assert(false);
        abort();
    }
    int w = 0;
    for (i = i + 1; i < fileSize; ++i)
    {
        char c = buffer[i];
        if (isValidChar[(int)c])
        {
            if (c == '\n')
            {
                if (w == 0)
                {
                    break;
                }
                height++;

                if (w != width)
                {
                    printf("ERROR - Incorrect level file\n");
                    assert(false);
                    abort();
                }
                w = 0;
            }
            else
            {
                w++;
            }
        }
        else
        {
            printf("INFO - Invalid char (%c)\n", c);
        }
    }

    // Initialisation du parser
    m_width = width;
    m_height = height;

    m_matrix = new char *[m_width];
    for (int x = 0; x < width; ++x)
    {
        m_matrix[x] = new char[height];
    }

    // Remplissage de la matrice du niveau
    int x = 0;
    int y = height - 1;
    for (i = 0; i < fileSize; ++i)
    {
        char c = buffer[i];
        if (isValidChar[(int)c])
        {
            if (c == '\n')
            {
                x = 0;
                y--;
                if (y < 0)
                {
                    break;
                }
            }
            else
            {
                m_matrix[x][y] = c;
                x++;
            }
        }
    }

    delete[] buffer;
}

LevelParser::~LevelParser()
{
    if (m_matrix)
    {
        for (int i = 0; i < m_width; ++i)
        {
            delete[] m_matrix[i];
        }
        delete[] m_matrix;
    }
}

void LevelParser::InitScene(LevelScene &scene) const
{
    // Cr�e la TileMap
    StaticMap *map = new StaticMap(scene, m_width, m_height);

    for (int x = 0; x < m_width; ++x)
    {
        for (int y = 0; y < m_height; ++y)
        {
            PE_Vec2 position((float)x, (float)y);
            switch (m_matrix[x][y])
            {
            case '#':
                map->SetTile(x, y, Tile::Type::GROUND);
                break;
            case 'i':
                map->SetTile(x, y, Tile::Type::GGROUND);
                break;
            case 'W':
                map->SetTile(x, y, Tile::Type::WOOD);
                break;
            case 'w':
                map->SetTile(x, y, Tile::Type::GWOOD);
                break;
            case '=':
                map->SetTile(x, y, Tile::Type::ONE_WAY);
                break;
            case '-':
                map->SetTile(x, y, Tile::Type::GONE_WAY);
                break;
            case 'A':
                map->SetTile(x, y, Tile::Type::SPIKE);
                break;
            case 'a':
                map->SetTile(x, y, Tile::Type::GSPIKE);
                break;
            case '|':
                map->SetTile(x, y, Tile::Type::STEEP_SLOPE_L);
                break;
            case '(':
                map->SetTile(x, y, Tile::Type::GSTEEP_SLOPE_L);
                break;
            case '/':
                map->SetTile(x, y, Tile::Type::STEEP_SLOPE_R);
                break;
            case ')':
                map->SetTile(x, y, Tile::Type::GSTEEP_SLOPE_R);
                break;
            case 'l':
                map->SetTile(x, y, Tile::Type::GENTLE_SLOPE_L);
                break;
            case '[':
                map->SetTile(x, y, Tile::Type::GGENTLE_SLOPE_L);
                break;
            case 'R':
                map->SetTile(x, y, Tile::Type::GENTLE_SLOPE_R);
                break;
            case ']':
                map->SetTile(x, y, Tile::Type::GGENTLE_SLOPE_R);
                break;
            case 'S':
            {
                Player *player = scene.GetPlayer();
                player->SetStartPosition(position);
                break;
            }
            case 'F':
            {
                LevelEnd *levelEnd = new LevelEnd(scene);
                levelEnd->SetStartPosition(position);
                break;
            }
            case 'C':
            {
                //creer un checkpoint
                Checkpoint* checkpoint = new Checkpoint(scene);
                checkpoint->SetStartPosition(position);
                break;
            }
            case '!':
            {
                //creer un Boss
                Boss* boss = new Boss(scene);
                boss->SetStartPosition(position);
                break;
            }
            case 'T':
            {
                //creer le totem du Boss
                Totem* totem = new Totem(scene);
                totem->SetStartPosition(position);
                break;
            }
            case 'e':
            {
                Nut *nut = new Nut(scene);
                nut->SetStartPosition(position);
                break;
            }
            case 'E':
            {
                nutdebile* Nutdebile = new nutdebile(scene);
                Nutdebile->SetStartPosition(position);
                break;
            }
            case 'o':
            {
                // DID : Cr�er une luciolle
                Firefly* firefly = new Firefly(scene, Layer::TERRAIN);
                firefly->SetStartPosition(position);
                break;
            }
            case 'h':
            {
                // Cr�er un coeur
                Heart* heart = new Heart(scene, Layer::TERRAIN);
                heart->SetStartPosition(position);
                break;
            }
            case 'b':
            {
                // creer une brick cassable
                Brick* brick = new Brick(scene);
                brick->SetStartPosition(position);
                break;
            }
            case 'm':
            {
                // creer une petite plateforme qui tombe si l'on mone dessus
                sfalling* Sfalling = new sfalling(scene);
                Sfalling->SetStartPosition(position);
                break;
            }
            case 'M':
            {
                // creer une grande plateforme qui tombe si l'on mone dessus
                lfalling* Lfalling = new lfalling(scene);
                Lfalling->SetStartPosition(position);
                break;
            }
            case 'd':
            {
                // creer une petite plateforme mouvante qui se decale a droite
                sright* Sright = new sright(scene);
                Sright->SetStartPosition(position);
                break;
            }
            case 'D':
            {
                // creer une grande plateforme mouvante qui se decale a droite
                lright* Lright = new lright(scene);
                Lright->SetStartPosition(position);
                break;
            }
            case 'g':
            {
                // creer une petite plateforme mouvante qui se decale a gauche
                sleft* Sleft = new sleft(scene);
                Sleft->SetStartPosition(position);
                break;
            }
            case 'G':
            {
                // creer une grande plateforme mouvante qui se decale a gauche
                lleft* Lleft = new lleft(scene);
                Lleft->SetStartPosition(position);
                break;
            }
            case 'B':
            {
                // creer un block bonus
                Bonus* bonus = new Bonus(scene);
                bonus->SetStartPosition(position);
                break;
            }
            default:
                break;
            }
        }
    }
    map->InitTiles();

    PE_AABB bounds(0.0f, 0.0f, (float)m_width, 24.0f * 9.0f / 16.0f);
    Camera *camera = scene.GetActiveCamera();
    camera->SetWorldBounds(bounds);
}
