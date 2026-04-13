#include "pch.h"

////////////////
/// Conditions
////////////////

//LivingEntity
bool ConditionLivingEntity_TargetInRange::OnTest(LivingEntity* pOwner)
{
	pOwner->TryChooseTarget();
	return pOwner->IsCurrentTargetReachable();
}
bool ConditionLivingEntity_IsDead::OnTest(LivingEntity* pOwner)
{
	return pOwner->IsDead();
}

//Monkey
bool ConditionMonkey_IsDestinationChanged::OnTest(LivingEntity* pOwner)
{
	if (pOwner->IsSelected() == false)
		return false;

	if (InputsManager::GetButtonDown(Mouse::RIGHT) == false)
		return false;

	XMINT2 mousePos = InputsManager::GetMousePosition();
	Hit hit = RayCast::CastRay(mousePos, GROUND_LAYER);
	if (hit.m_CollidingID == -1)
		return false;

	return true;
}
bool ConditionMonkey_IsDestinationReached::OnTest(LivingEntity* pOwner)
{
	Monkey* owner = (Monkey*)pOwner;
	XMFLOAT3 position = owner->GetTransformComponent()->GetWorldPosition();
	XMFLOAT3 destination = owner->GetDestination();

	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR dest = XMLoadFloat3(&destination);
	XMVECTOR dir = dest - pos;
	XMVECTOR dist = XMVector3Length(dir);
	float distance = XMVectorGetX(dist);

	if (distance >= 1.f)
		return false;

	return true;
}

bool ConditionMonkey_IsAttackPriority::OnTest(LivingEntity* pOwner)
{
	Monkey* owner = (Monkey*)pOwner;
	return owner->IsAttackPriority();
}

bool ConditionMonkey_IsNearHealTower::OnTest(LivingEntity* pOwner)
{
	Monkey* owner = (Monkey*)pOwner;
	ColliderComponent* collider = owner->GetRangeColliderComponent();

	std::vector<LivingEntity*> buildings = SceneManager::Get()->GetActiveScene<SceneWorld>()->p_EntityManager->GetEntitiesByType(EntityMapIndex::HEALTOWER);

	for (auto d : buildings)
	{
		Building* building = (Building*)d;

		if (building->IsBuilt() == false)
			continue;

		int id = building->GetEntityID();

		if (collider->IsCollidingWith(id))
		{
			owner->SetNearestBuilding(building);
			return true;
		}
	}

	return false;
}
bool ConditionMonkey_IsFullyHealed::OnTest(LivingEntity* pOwner)
{
	Monkey* owner = (Monkey*)pOwner;

	if(owner->GetHealth() >= owner->GetMaxHealth())
		return true;

	return false;
}


// Minion 
bool ConditionMinion_IsFull::OnTest(LivingEntity* pOwner)
{
	Minion* owner = (Minion*)pOwner;

	if(owner->GetMaterialCount() == owner->GetMaxMaterialCount())
		return true;

	return false;
}

bool ConditionMinion_IsEmpty::OnTest(LivingEntity* pOwner)
{
	Minion* owner = (Minion*)pOwner;

	if (owner->GetMaterialCount() == 0)
		return true;

	return false;
}

bool ConditionMinion_IsNearMine::OnTest(LivingEntity* pOwner)
{
	Minion* owner = (Minion*)pOwner;

	ColliderComponent* collider = owner->GetRangeColliderComponent();

	std::vector<LivingEntity*> deposits = SceneManager::Get()->GetActiveScene<SceneWorld>()->m_Deposit;

	for (auto d : deposits)
	{
		int id = d->GetEntityID();

		if(collider->IsCollidingWith(id))
		{
			owner->SetMine((Deposit*)d);
			return true;
		}
	}

	return false;
}
bool ConditionMinion_IsArrivedStorage::OnTest(LivingEntity* pOwner)
{
	Minion* owner = (Minion*)pOwner;
	int storageID = owner->GetStorage()->GetEntityID();

	if (owner->GetRangeColliderComponent()->IsCollidingWith(storageID))
		return true;

	return false;
}

bool ConditionMinion_IsNearBuildingToBuild::OnTest(LivingEntity* pOwner)
{
	Minion* owner = (Minion*)pOwner;
	ColliderComponent* collider = owner->GetRangeColliderComponent();

	std::vector<LivingEntity*> buildings = SceneManager::Get()->GetActiveScene<SceneWorld>()->p_EntityManager->GetEntitiesByType(EntityMapIndex::BUILDING);

	for (auto d : buildings)
	{
		Building* building = (Building*)d;

		if (building->IsBuilt() == true)
			continue;

		int id = building->GetEntityID();

		if (collider->IsCollidingWith(id))
		{
			owner->SetNearestBuilding(building);
			return true;
		}
	}

	return false;
}
bool ConditionMinion_FinishedBuilding::OnTest(LivingEntity* pOwner)
{
	Minion* owner = (Minion*)pOwner;
	Building* building = owner->GetNearestBuilding();

	if (building == nullptr)
		return false;

	return owner->GetNearestBuilding()->IsBuilt();
}


///////////
/// Action
///////////

//LivingEntity

void ActionLivingEntity_Attack::OnUpdate(LivingEntity* pOwner)
{
	pOwner->TryAttack();
}

//Monkey



// - Move -
void ActionMonkey_Move::OnStart(LivingEntity* pOwner)
{
	if (InputsManager::GetButtonDown(Mouse::RIGHT) == false)
		return;

	Monkey* owner = (Monkey*)pOwner;

	owner->SetComponentSpeed(owner->GetSpeed());
	
	if (owner->IsSelected() == false)
		return;

	XMINT2 mousePos = InputsManager::GetMousePosition();
	bool isAttackPriority = InputsManager::GetKey(Keyboard::LSHIFT);
	Hit hit = RayCast::CastRay(mousePos, GROUND_LAYER);

	owner->GetGroup()->SetLeaderDestination(hit.m_Point, isAttackPriority);
}

void ActionMonkey_Move::OnUpdate(LivingEntity* pOwner)
{
	Monkey* owner = (Monkey*)pOwner;

	owner->Animation_Moving();

	if (owner->IsSelected())
		owner->ShowDestination();
	else
		owner->HideDestination();
}
void ActionMonkey_Move::OnEnd(LivingEntity* pOwner)
{
	Monkey* owner = (Monkey*)pOwner;
	owner->StopMoving();
	owner->HideDestination();
}

// - Idle -
void ActionMonkey_Idle::OnUpdate(LivingEntity* pOwner)
{
	pOwner->TryChooseTarget();

	Monkey* owner = (Monkey*)pOwner;
	owner->Animation_Moving();
}

// - Attack -
void ActionMonkey_Attack::OnUpdate(LivingEntity* pOwner)
{
	Monkey* owner = (Monkey*)pOwner;

	if (owner->TryAttack())
		owner->Play_Attack();

}

void ActionMonkey_Heal::OnStart(LivingEntity* pOwner)
{
	m_HealingTimer = 0.0f;
	m_HealingStep = 0.2f;
}

void ActionMonkey_Heal::OnUpdate(LivingEntity* pOwner)
{
	float dt = ECS::Get().GetDeltaTime();
	m_HealingTimer += dt;

	if (m_HealingTimer >= m_HealingStep)
	{
		m_HealingTimer = 0.0f;

		Monkey* owner = (Monkey*)pOwner;
		owner->GetHealed(1.0f);
	}
}

void ActionMonkey_Heal::OnEnd(LivingEntity* pOwner)
{
	Monkey* owner = (Monkey*)pOwner;

	owner->SetNearestBuilding(nullptr);
}

//Minion

void ActionMinion_GoToMine::OnStart(LivingEntity* pOwner)
{
	Minion* owner = (Minion*)pOwner;
	Deposit* mine = owner->GetMine();

	owner->GetGroup()->SetLeaderDestination(mine->GetPosition(), false);
}

void ActionMinion_Mining::OnStart(LivingEntity* pOwner)
{
	m_MiningTimer = 0.0f;
	m_MiningStep = 0.3f;
}
void ActionMinion_Mining::OnUpdate(LivingEntity* pOwner)
{
	float dt = ECS::Get().GetDeltaTime();

	m_MiningTimer += dt;

	if (m_MiningTimer >= m_MiningStep)
	{
		m_MiningTimer = 0.0f;

		Minion* owner = (Minion*)pOwner;
		owner->CollectMaterial();
	}
}

void ActionMinion_GoToStorage::OnStart(LivingEntity* pOwner)
{

	Minion* owner = (Minion*)pOwner;

	Nexus* nexus = owner->GetStorage();

	XMFLOAT3 pos = nexus->GetPosition();
	pos.y = 0.0f;

	owner->GetGroup()->SetLeaderDestination(pos, false);
}

void ActionMinion_Store::OnStart(LivingEntity* pOwner)
{
	Minion* owner = (Minion*)pOwner;
	owner->StoreMaterials();
}

void ActionMinion_Build::OnStart(LivingEntity* pOwner)
{
	m_BuildingTimer = 0.0f;
	m_BuildingStep = 0.5f;
}

void ActionMinion_Build::OnUpdate(LivingEntity* pOwner)
{
	float dt = ECS::Get().GetDeltaTime();

	m_BuildingTimer += dt;

	if (m_BuildingTimer >= m_BuildingStep)
	{
		m_BuildingTimer = 0.0f;

		Minion* owner = (Minion*)pOwner;
		owner->GetNearestBuilding()->Build();
	}
}

void ActionMinion_Build::OnEnd(LivingEntity* pOwner)
{
	Minion* owner = (Minion*)pOwner;

	owner->SetNearestBuilding(nullptr);
}

