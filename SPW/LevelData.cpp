#include "LevelData.h"

LevelData::LevelData(const std::string &nameIn, const std::string &pathIn, ThemeID themeIDIn) :
    name(nameIn), path(pathIn), themeID(themeIDIn)
{
}

std::vector<LevelData> LevelData::Init()
{
    std::vector<LevelData> data;

    data.push_back(LevelData(
        u8"Montagnes",
        u8"../Assets/Level/Level1.txt",
        ThemeID::MOUNTAINS
    ));
    data.push_back(LevelData(
        u8"Demo",
        u8"../Assets/Level/LevelDemo.txt",
        ThemeID::SKY
    ));
    data.push_back(LevelData(
        u8"test",
        u8"../Assets/Level/test.txt",
        ThemeID::SKY
    ));
    data.push_back(LevelData(
        u8"test_Boss",
        u8"../Assets/Level/test_Boss.txt",
        ThemeID::LAKE
    ));
    data.push_back(LevelData(
        u8"LineRace",
        u8"../Assets/Level/LineRace.txt",
        ThemeID::SKY
    ));
    return data;
}
