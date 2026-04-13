#pragma once

#define HORDE_CHECK_BUILDING_TIME 1.0f

// - HORDE -

enum class HordeType
{
	KAMRADE,
	CUTTER,
	BODYBUILDER,
	SOLDIER,

	COUNT
};
enum class HordeSize
{
	NONE,
	SMALL,
	MEDIUM,
	LARGE,

	COUNT
};

class Horde
{
protected:
	Group* p_Group = nullptr;
	Building* p_TargetBuilding = nullptr;
	std::string m_Material = "None";

	float m_CheckBuildingTimer = 0.0f;

	int m_Difficulty = 0;
	int m_FacEntityIndex = (int)EntityFacIndex::KAMRADE;
	int m_HordeType = (int)HordeType::KAMRADE;
	int m_EntityCount = 0;

	bool m_IsKilled = false;

	void GoToDestination();
	void FindDestination(XMFLOAT3 _dest);

	void Update(float _dt);

public:
	Horde(HordeType _HordeType, EntityFacIndex _Entity, int _Count);

	std::string GetMaterial();

	bool IsKilled() { return m_IsKilled; }

	void SpawnHorde(float _x, float _y, float _z);

	friend class Wave;
};

struct HordeFactory
{
	virtual Horde* CreateHorde(int _size) { return nullptr; }
};
class HordeStorage final
{
	std::vector<HordeFactory*> m_HordeFactories;

	static inline HordeStorage* p_Instance = nullptr;

	HordeStorage();
	static HordeStorage* Get();
public :

	static Horde* GetHorde(HordeType _horde, HordeSize _size = HordeSize::SMALL) { return Get()->m_HordeFactories[(int)_horde]->CreateHorde((int)_size); }
};

// - WAVE -

class Wave
{
protected:
	std::vector<Horde*> m_Hordes;
	int m_Difficulty = 0;

	bool m_IsStarted = false;

	void Update(float _dt);
	void Reset();
public:

	int GetDiffculty() { return m_Difficulty; }

	void AddHorde(Horde* _pHorde);
	void SpawnWave(float _x, float _y, float _z);

	friend class Level;
};

// - LEVEL - 

class Level
{
protected:
	std::vector<Wave*> m_Waves;

	float m_Duration = 0.0f;

	float m_DurationBetweenWave = 0.0f;
	float m_Timer = 0.0f;

	int m_ActualWave = 0;

	bool m_IsFinished = false;

	void Update(float _dt);
public:

	Level(float _waveDuration);

	bool IsFinished() { return m_IsFinished; }
	int GetActualWave() { return m_ActualWave; }
	int GetWavesCount() { return m_Waves.size(); }
	float GetWaveTimer() { return m_Timer; }
	float GetDurationBetweenWave() { return m_DurationBetweenWave; }

	void AddWave(Wave* _pWave);
	void SpawnWave(int _WaveIndex, float _x, float _y, float _z);

	friend class LevelManager;
};

// - Custom Hordes - 

// - Kamrade -
struct HordeKamrade : public Horde
{
	HordeKamrade(int _size) : Horde(HordeType::KAMRADE, EntityFacIndex::KAMRADE, _size * 5) { m_Difficulty = _size * 5 * 0.25f; }
};
struct HordeFactoryKamrade : public HordeFactory
{
	Horde* CreateHorde(int _size) { return new HordeKamrade(_size); }
};

// - Cutter -
struct HordeCutter : public Horde
{
	HordeCutter(int _size) : Horde(HordeType::CUTTER, EntityFacIndex::CUTTER, _size * 5) { m_Difficulty = _size * 5 * 1.0f; }
};
struct HordeFactoryCutter : public HordeFactory
{
	Horde* CreateHorde(int _size) { return new HordeCutter(_size); }
};

// - BODYBUILDER -
struct HordeBodybuilder : public Horde
{
	HordeBodybuilder(int _size) : Horde(HordeType::BODYBUILDER, EntityFacIndex::BODYBUILDER, _size * 5) { m_Difficulty = _size * 5 * 1.0f; }
};
struct HordeFactoryBodybuilder : public HordeFactory
{
	Horde* CreateHorde(int _size) { return new HordeBodybuilder(_size); }
};

// - Soldier -
struct HordeSoldier : public Horde
{
	HordeSoldier(int _size) : Horde(HordeType::SOLDIER, EntityFacIndex::SOLDIER, _size * 5) { m_Difficulty = _size * 5 * 1.0f; }
};
struct HordeFactorySoldier : public HordeFactory
{
	Horde* CreateHorde(int _size) { return new HordeSoldier(_size); }
};