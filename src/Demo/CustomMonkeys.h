#pragma once

/// KAMRADE ///
class Kamrade : public Monkey
{
public:
	Kamrade();
};

/// CUTTER ///
class Cutter : public Monkey
{
public:
	Cutter();
};

/// BODYBUILDER ///
class Bodybuilder : public Monkey
{
public:
	Bodybuilder();
};

/// SOLDIER ///
class Soldier : public Monkey
{
public:
	Soldier();
};

/// MINION /// 
class Minion : public Monkey
{
	int m_MaterialCount = 0;
	int m_MaxMaterial = 5;

	int m_LastState = -1;

	Nexus* p_Storage = nullptr;
	Deposit* p_Mine = nullptr;

protected:
	virtual void InitStateMachine() override;
	virtual void OnUpdate(float _dt) override;

public:
	Minion();
	
	void ShowState();
	void SetMine(Deposit* _pMine) { p_Mine = _pMine; }
	void StoreMaterials();
	void CollectMaterial();
	Nexus* GetStorage() { return p_Storage; }
	Deposit* GetMine() { return p_Mine; }
	int GetMaterialCount() { return m_MaterialCount; }
	int GetMaxMaterialCount() { return m_MaxMaterial; }
};