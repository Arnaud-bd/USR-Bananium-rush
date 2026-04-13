#include "pch.h"

void LevelManager::Update(float _dt)
{
    if (m_Levels.size() == 0)
        return;

    m_Levels[m_ActualIndex]->Update(_dt);
}

Level* LevelManager::CreateLevel(float _waveDuration)
{
    Level* level = new Level(_waveDuration);
    m_Levels.push_back(level);
    return level;
}

void LevelManager::PlayLevel(Level* _level)
{
    for(int i = 0; i < m_Levels.size(); i++)
    {
        if (m_Levels[i] != _level)
            continue;
        PlayLevel(i);
        return;
    }
    D::Cout("LevelManager::PlayLevel : No level find to play -> " + std::to_string((std::uintptr_t)_level));
}
