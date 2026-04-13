#pragma once

class Level;

class LevelManager
{
	std::vector<Level*> m_Levels;

	int m_ActualIndex = 0;

	void Update(float _dt);
public:

	Level* GetActualLevel() { return m_Levels[m_ActualIndex]; }

	template <typename L>
	L* CreateLevel();
	
	Level* CreateLevel(float _waveDuration);
	void PlayLevel(int _index) { m_ActualIndex = _index; }
	void PlayLevel(Level* _level);

	friend class SceneWorld;
};

template<typename L>
inline L* LevelManager::CreateLevel()
{
	L* level = new L();
	m_Levels.push_back(level);
	return level;
}
