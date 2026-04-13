#include "pch.h"

/// KAMRADE ///

Kamrade::Kamrade() : Monkey()
{
	SetRangeDiameter(5.f);
	ShowRange();
	SetSpeed(SPEED_SLOW);
	SetName("Kamrade");

	p_Mesh->SetMaterial(MATERIAL_KAMRADALLY_NAME);
}

/// CUTTER ///

Cutter::Cutter() : Monkey()
{
	SetRangeDiameter(5.f);
	SetSpeed(SPEED_NORMAL);
	SetDamage(5.0f);
	ShowRange();
	SetName("Cutter");

	p_Mesh->SetMaterial(MATERIAL_CUTTERALLY_NAME);
}

/// BODYBUILDER ///

Bodybuilder::Bodybuilder()
{
	SetRangeDiameter(5.f);
	SetSpeed(SPEED_SLOW);
	SetDamage(4.0f);
	SetAttackSpeed(0.5f);
	SetMaxHealth(40);
	SetHealth(40);
	ShowRange();
	SetName("Bodybuilder");

	p_Mesh->SetMaterial(MATERIAL_BODYBUILDERALLY_NAME);
}

/// SOLDIER ///

Soldier::Soldier()
{
	SetRangeDiameter(15.f);
	SetSpeed(SPEED_SLOW);
	SetDamage(10.0f);
	SetAttackSpeed(0.5f);
	ShowRange();
	SetName("Soldier");

	p_Mesh->SetMaterial(MATERIAL_SOLDIERSALLY_NAME);
}

/// MINION ///

void Minion::InitStateMachine()
{
	if (p_StateMachine != nullptr)
		delete(p_StateMachine);

	p_StateMachine = new StateMachineMinion();

	p_StateMachine->Initialize(this, (int)MinionStates::COUNT);

	//Dead
	ActionLivingEntity_Dead* pDead = p_StateMachine->CreateAction<ActionLivingEntity_Dead>((int)MinionStates::DEAD);

	//Idle
	ActionMonkey_Idle* pIdle = p_StateMachine->CreateAction<ActionMonkey_Idle>((int)MinionStates::IDLE);
	{	//-> Dead
		auto transition = pIdle->CreateTransition((int)MinionStates::DEAD);
		transition->AddCondition<ConditionLivingEntity_IsDead>();
	}
	{	//-> Moving
		auto transition = pIdle->CreateTransition((int)MinionStates::MOVE);
		transition->AddCondition<ConditionMonkey_IsDestinationChanged>();
	}
	{	//-> Mining
		auto transition = pIdle->CreateTransition((int)MinionStates::MINING);
		transition->AddCondition<ConditionMinion_IsNearMine>();
	}
	{	//-> Store
		auto transition = pIdle->CreateTransition((int)MinionStates::STORE);
		transition->AddCondition<ConditionMinion_IsArrivedStorage>();
		transition->AddCondition<ConditionMinion_IsEmpty>(false);
	}
	{	//-> Build
		auto transition = pIdle->CreateTransition((int)MinionStates::BUILD);
		transition->AddCondition<ConditionMinion_IsNearBuildingToBuild>();
	}
	{	//-> Heal
		auto transition = pIdle->CreateTransition((int)MinionStates::HEAL);
		transition->AddCondition<ConditionMonkey_IsNearHealTower>();
		transition->AddCondition<ConditionMonkey_IsFullyHealed>(false);
	}

	//Move
	ActionMonkey_Move* pMove = p_StateMachine->CreateAction<ActionMonkey_Move>((int)MinionStates::MOVE);
	{	//-> Dead
		auto transition = pMove->CreateTransition((int)MinionStates::DEAD);
		transition->AddCondition<ConditionLivingEntity_IsDead>();
	}
	{	//-> Idle
		auto transition = pMove->CreateTransition((int)MinionStates::IDLE);
		transition->AddCondition<ConditionMonkey_IsDestinationReached>();
	}
	{	//-> Move
		auto transition = pMove->CreateTransition((int)MonkeyStates::MOVE);
		transition->AddCondition<ConditionMonkey_IsDestinationChanged>();
	}

	//GoToMine
	ActionMinion_GoToMine* pGotoMine = p_StateMachine->CreateAction<ActionMinion_GoToMine>((int)MinionStates::GOTO_MINE);
	{	//-> Dead
		auto transition = pGotoMine->CreateTransition((int)MinionStates::DEAD);
		transition->AddCondition<ConditionLivingEntity_IsDead>();
	}

	//Mining
	ActionMinion_Mining* pMining = p_StateMachine->CreateAction<ActionMinion_Mining>((int)MinionStates::MINING);
	{	//-> Dead
		auto transition = pMining->CreateTransition((int)MinionStates::DEAD);
		transition->AddCondition<ConditionLivingEntity_IsDead>();
	}
	{	//-> Move
		auto transition = pMining->CreateTransition((int)MinionStates::MOVE);
		transition->AddCondition<ConditionMonkey_IsDestinationChanged>();
	}
	{	//-> GotoStorage
		auto transition = pMining->CreateTransition((int)MinionStates::GOTO_STORAGE);
		transition->AddCondition<ConditionMinion_IsFull>();
	}

	//GoToStorage
	ActionMinion_GoToStorage* pGotoStorage = p_StateMachine->CreateAction<ActionMinion_GoToStorage>((int)MinionStates::GOTO_STORAGE);
	{	//-> Dead
		auto transition = pGotoStorage->CreateTransition((int)MinionStates::DEAD);
		transition->AddCondition<ConditionLivingEntity_IsDead>();
	}

	//Store
	ActionMinion_Store* pStore = p_StateMachine->CreateAction<ActionMinion_Store>((int)MinionStates::STORE);
	{	//-> GoToMine
		auto transition = pStore->CreateTransition((int)MinionStates::GOTO_MINE);
		transition->AddCondition<ConditionMinion_IsEmpty>();
	}

	//Build
	ActionMinion_Build* pBuild = p_StateMachine->CreateAction<ActionMinion_Build>((int)MinionStates::BUILD);
	{	//-> Idle
		auto transition = pBuild->CreateTransition((int)MinionStates::IDLE);
		transition->AddCondition<ConditionMinion_FinishedBuilding>();
	}
	{	//-> Move
		auto transition = pBuild->CreateTransition((int)MinionStates::MOVE);
		transition->AddCondition<ConditionMonkey_IsDestinationChanged>();
	}

	//Heal
	ActionMonkey_Heal* pHeal = p_StateMachine->CreateAction<ActionMonkey_Heal>((int)MinionStates::HEAL);
	{	//-> Idle
		auto transition = pHeal->CreateTransition((int)MinionStates::IDLE);
		transition->AddCondition<ConditionMonkey_IsFullyHealed>();
	}
	{	//-> Move
		auto transition = pHeal->CreateTransition((int)MinionStates::MOVE);
		transition->AddCondition<ConditionMonkey_IsDestinationChanged>();
	}

	p_StateMachine->SetState((int)MinionStates::IDLE);
}

void Minion::StoreMaterials()
{
	int type = (int)p_Mine->GetType();

	switch (type)
	{
	case(0):

		SceneManager::Get()->GetActiveScene<SceneWorld>()->p_Player->AddToInventory({ m_MaterialCount, 0, 0 });
		break;
	case(1):

		SceneManager::Get()->GetActiveScene<SceneWorld>()->p_Player->AddToInventory({ 0, m_MaterialCount, 0 });
		break;
	case(2):

		SceneManager::Get()->GetActiveScene<SceneWorld>()->p_Player->AddToInventory({ 0, 0 ,m_MaterialCount });
		break;
	default:
		break;
	}

	m_MaterialCount = 0;
}

void Minion::CollectMaterial()
{
	m_MaterialCount++;

	if (m_MaterialCount > m_MaxMaterial)
		m_MaterialCount = m_MaxMaterial;
}

void Minion::OnUpdate(float _dt)
{
	Monkey::OnUpdate(_dt);
}

Minion::Minion() : Monkey()
{
	SetRangeDiameter(10.f);
	SetSpeed(SPEED_FAST);
	ShowRange();
	SetTeam(TEAM_ZENTIL);
	AddEnemy(TEAM_ARNO);
	p_Storage = SceneManager::Get()->GetActiveScene<SceneWorld>()->m_Allie_Nexus;
	SetName("Minion");
	p_Mesh->SetMaterial(MATERIAL_MINIONSALLY_NAME);
	InitStateMachine();
}

