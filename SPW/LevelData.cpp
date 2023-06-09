#include "LevelData.h"

LevelData::LevelData(const std::string &nameIn, const std::string &pathIn, ThemeID themeIDIn) :
    name(nameIn), path(pathIn), themeID(themeIDIn)
{
}

std::vector<LevelData> LevelData::Init()
{
    std::vector<LevelData> data;
    
    data.push_back(LevelData(
        u8"Niveau 1",
        u8"../Assets/Level/Level1.txt",
        ThemeID::MOUNTAINS
    ));
    data.push_back(LevelData(
        u8"Niveau 2",
        u8"../Assets/Level/LevelDemo.txt",
        ThemeID::SKY
    ));
    data.push_back(LevelData(
        u8"test",
        u8"../Assets/Level/levelboss.txt",
        ThemeID::SKY
    ));
    data.push_back(LevelData(
        u8"test_Boss",
        u8"../Assets/Level/test_Boss.txt",
        ThemeID::LAKE
    ));
    data.push_back(LevelData(
        u8"LineRace",
        u8"../Assets/Level/highter.txt",
        ThemeID::SKY
    ));
    return data;
}
