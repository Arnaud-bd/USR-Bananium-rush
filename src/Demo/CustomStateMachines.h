#pragma once

enum class LivingEntityStates
{
	IDLE,
	ATTACK,
	DEAD,

	COUNT
};

enum class MonkeyStates
{
	IDLE,	//From parent
	ATTACK,
	DEAD,

	MOVE,	//new in child
	HEAL,

	COUNT
};

enum class MinionStates
{
	IDLE, 
	ATTACK,
	DEAD,
	MOVE,
	HEAL,

	GOTO_MINE,
	MINING,
	GOTO_STORAGE,
	STORE,
	BUILD,

	COUNT
};

class StateMachineLivingEntity : public StateMachine<LivingEntity>
{
public:
	StateMachineLivingEntity() : StateMachine<LivingEntity>() {};
};

class StateMachineMonkey : public StateMachineLivingEntity
{
public:
	StateMachineMonkey() : StateMachineLivingEntity() {};
};

class StateMachineMinion : public StateMachineMonkey
{
public:
	StateMachineMinion() : StateMachineMonkey() {};
};

//////////////////
/// CONDITIONS
//////////////////

/// Living Entity
class ConditionLivingEntity_TargetInRange : public Condition<LivingEntity>
{
public:
	bool OnTest(LivingEntity* pOwner) override;
};
class ConditionLivingEntity_IsDead : public Condition<LivingEntity>
{
public:
	bool OnTest(LivingEntity* pOwner) override;
};

/// Monkey
class ConditionMonkey_IsDestinationChanged : public Condition<LivingEntity>
{
public:
	bool OnTest(LivingEntity* pOwner) override;
};
class ConditionMonkey_IsDestinationReached : public Condition<LivingEntity>
{
public:
	bool OnTest(LivingEntity* pOwner) override;
};
class ConditionMonkey_IsAttackPriority : public Condition<LivingEntity>
{
public:
	bool OnTest(LivingEntity* pOwner) override;
};
class ConditionMonkey_IsNearHealTower : public Condition<LivingEntity>
{
public:
	bool OnTest(LivingEntity* pOwner) override;
};
class ConditionMonkey_IsFullyHealed : public Condition<LivingEntity>
{
public:
	bool OnTest(LivingEntity* pOwner) override;
};

/// Minion 
class ConditionMinion_IsFull : public Condition<LivingEntity>
{
public:
	bool OnTest(LivingEntity* pOwner) override;
};
class ConditionMinion_IsEmpty : public Condition<LivingEntity>
{
public:
	bool OnTest(LivingEntity* pOwner) override;
};
class ConditionMinion_IsNearMine : public Condition<LivingEntity>
{
public:
	bool OnTest(LivingEntity* pOwner) override;
};
class ConditionMinion_IsArrivedStorage : public Condition<LivingEntity>
{
public:
	bool OnTest(LivingEntity* pOwner) override;
};
class ConditionMinion_IsNearBuildingToBuild : public Condition<LivingEntity>
{
public:
	bool OnTest(LivingEntity* pOwner) override;
};
class ConditionMinion_FinishedBuilding: public Condition<LivingEntity>
{
public:
	bool OnTest(LivingEntity* pOwner) override;
};


//////////////////
/// ACTIONS
//////////////////

/// Living Entity
class ActionLivingEntity_Idle : public Action<LivingEntity>
{
public:
	void OnStart(LivingEntity* pOwner) override {}
	void OnUpdate(LivingEntity* pOwner) override {}
	void OnEnd(LivingEntity* pOwner) override {}
};
class ActionLivingEntity_Dead : public Action<LivingEntity>
{
public:
	void OnStart(LivingEntity* pOwner) override {}
	void OnUpdate(LivingEntity* pOwner) override {}
	void OnEnd(LivingEntity* pOwner) override {}
};
class ActionLivingEntity_Attack : public Action<LivingEntity>
{
public:
	void OnStart(LivingEntity* pOwner) override {};
	void OnUpdate(LivingEntity* pOwner) override;
	void OnEnd(LivingEntity* pOwner) override {}
};

/// Monkey
class ActionMonkey_Move : public Action<LivingEntity>
{
public:
	virtual void OnStart(LivingEntity* pOwner) override;
	virtual void OnUpdate(LivingEntity* pOwner) override;
	virtual void OnEnd(LivingEntity* pOwner) override;
};

class ActionMonkey_Idle : public Action<LivingEntity>
{
public:
	void OnStart(LivingEntity* pOwner) override {}
	void OnUpdate(LivingEntity* pOwner) override;
	void OnEnd(LivingEntity* pOwner) override {}
};

class ActionMonkey_Attack : public Action<LivingEntity>
{
public:
	void OnStart(LivingEntity* pOwner) override {};
	void OnUpdate(LivingEntity* pOwner) override;
	void OnEnd(LivingEntity* pOwner) override {}
};
class ActionMonkey_Heal : public Action<LivingEntity>
{
	float m_HealingTimer = 0.0f;
	float m_HealingStep = 1.0f;

public:
	void OnStart(LivingEntity* pOwner) override;
	void OnUpdate(LivingEntity* pOwner) override;
	void OnEnd(LivingEntity* pOwner) override;
};


/// Minion
class ActionMinion_GoToMine : public Action<LivingEntity>
{
public:
	void OnStart(LivingEntity* pOwner) override;
	void OnUpdate(LivingEntity* pOwner) override {}
	void OnEnd(LivingEntity* pOwner) override {}
};

class ActionMinion_Mining : public Action<LivingEntity>
{
	float m_MiningTimer = 0.0f;
	float m_MiningStep = 0.3f;

public:
	void OnStart(LivingEntity* pOwner) override;
	void OnUpdate(LivingEntity* pOwner) override;
	void OnEnd(LivingEntity* pOwner) override {}
};

class ActionMinion_GoToStorage : public Action<LivingEntity>
{
public:
	void OnStart(LivingEntity* pOwner) override;
	void OnUpdate(LivingEntity* pOwner) override {}
	void OnEnd(LivingEntity* pOwner) override {}
};

class ActionMinion_Store : public Action<LivingEntity>
{
public:
	void OnStart(LivingEntity* pOwner) override;
	void OnUpdate(LivingEntity* pOwner) override {}
	void OnEnd(LivingEntity* pOwner) override {}
};

class ActionMinion_Build : public Action<LivingEntity>
{
	float m_BuildingTimer = 0.0f;
	float m_BuildingStep = 0.5f;

public:
	void OnStart(LivingEntity* pOwner) override;
	void OnUpdate(LivingEntity* pOwner) override;
	void OnEnd(LivingEntity* pOwner) override;
};



