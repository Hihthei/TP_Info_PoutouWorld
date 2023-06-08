#pragma once

#include "Settings.h"
#include "UIObject.h"
#include "Text.h"
#include "Image.h"

class LevelScene;

class LevelHeader : public UIObject
{
public:
    LevelHeader(LevelScene &scene);


    virtual void Update() override;

private:
    LevelScene &m_levelScene;
    Text *m_fireflyCount;

    Text* m_lifeCount;
    int m_lifeCountInt;

    Text* m_heartCount;
    int m_heartCountInt;

};