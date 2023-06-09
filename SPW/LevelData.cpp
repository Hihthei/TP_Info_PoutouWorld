#include "LevelData.h"

LevelData::LevelData(const std::string& nameIn, const std::string& pathIn, ThemeID themeIDIn) :
    name(nameIn), path(pathIn), themeID(themeIDIn)
{
}

std::vector<LevelData> LevelData::Init()
{
    std::vector<LevelData> data;

    data.push_back(LevelData(
        u8"Niveau 1 : Mountains",
        u8"../Assets/Level/Level1.txt",
        ThemeID::MOUNTAINS
    ));
    data.push_back(LevelData(
        u8"Niveau 2 : Aller vers le ciel",
        u8"../Assets/Level/Level2.txt",
        ThemeID::SKY
    ));
    data.push_back(LevelData(
        u8"Niveau 3 : Ice on Glace",
        u8"../Assets/Level/test_Boss.txt",
        ThemeID::LAKE
    ));
    data.push_back(LevelData(
        u8"BOSS in the arena",
        u8"../Assets/Level/levelboss.txt",
        ThemeID::LAKE
    ));
    data.push_back(LevelData(
        u8"LineRace",
        u8"../Assets/Level/highter.txt",
        ThemeID::SKY
    ));
    return data;
}
