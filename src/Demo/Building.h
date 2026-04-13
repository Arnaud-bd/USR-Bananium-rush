#pragma once
class Building : public LivingEntity
{
protected:

	int m_FinalBuildingStep = 100;
	int m_CurrentBuildingStep = 0;
	bool m_IsBuilt = false;

	virtual void OnClick();
	virtual void OnBuild() {};
	virtual void OnDeath();
	virtual void OnDestroy();
public:
	Building();

	void Build();
	bool IsBuilt() { return m_IsBuilt; }
};

/// BARRACK ///

class Barrack : public Building
{
	virtual void OnDestroy();
public:
	Barrack();

	virtual void OnBuild();
};

/// NEXUS ///

class Nexus : public Building
{
	int m_Production = 1;
	float m_Time = 5;
	float m_Timer = 0;

public:
	Nexus();

	int GetProduction() { return m_Production; };
	bool TimerNexus();
};

/// ORES ///

enum class OreType
{
	BANANIUM,
	IRON,
	ROCK,

	COUNT
};
class Deposit : public Building
{
	OreType m_type = OreType::COUNT;

public:
	Deposit();

	void SetType(OreType _type);
	OreType GetType() { return m_type; }
};

/// HealTower ///

class HealTower : public Building
{
	bool m_showingRange = false;

	virtual void OnDestroy();
	virtual void OnClick();

public:
	HealTower();

	virtual void OnBuild();
};