#pragma once

class CirclePlacement
{
	std::vector<Monkey*> m_Monkeys;
	float m_Radius = 1.0f;
	int m_Max = 5;

public:
	bool IsFull() { return m_Monkeys.size() >= m_Max; }
	XMFLOAT3 GetPosition(int index);
	std::vector<Monkey*>& GetMonkeys() { return m_Monkeys; }

	void Add(Monkey* _pMonkey);

	friend class DiskPlacement;
	friend class Group;
};
class DiskPlacement
{
	std::vector<CirclePlacement*> m_Circles;
	int m_MaxCircles;

public:
	DiskPlacement(int _maxCircles = 3, int _entityCountFirstCircle = 5, float _startRadius = 1.0f);

	void Add(Monkey* _pMonkey);

	friend class Group;
};

class Group
{
protected:

	DiskPlacement m_Placement;
	std::vector<Monkey*> m_InactiveMonkeys;

	Monkey* p_Leader = nullptr;
	bool m_isDestroy = false;

public:
	Group() = default;
	void Destroy();

	void Init(int _maxCircles = 3, int _entityCountFirstCircle = 5, float _startRadius = 1.0f);
	void FindNewLeader();
	void ReplaceMonkeys();
	void ShowHP();

	Monkey* GetLeader() { return p_Leader; }
	XMFLOAT3 GetLeaderPosition() { return p_Leader->GetTransformComponent()->GetWorldPosition(); }
	bool IsDestroy() { return m_isDestroy; }

	void AddFollower(Monkey* _pMonkey);
	void SetFollowerInactive(Monkey* _pMonkey) { m_InactiveMonkeys.push_back(_pMonkey); }
	void SetLeader(Monkey* _pMonkey) { p_Leader = _pMonkey; }
	void SetLeaderDestination(XMFLOAT3 _dest, bool _isAttackPriority);

	friend class EntityManager;
};