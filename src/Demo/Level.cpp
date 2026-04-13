#include "pch.h"

void Horde::GoToDestination()
{
	if (p_TargetBuilding == nullptr)
		return;
	XMFLOAT3 dest = p_TargetBuilding->GetPosition();
	dest.y -= p_TargetBuilding->GetTransformComponent()->GetScale().y / 2.0f;
	p_Group->SetLeaderDestination(dest, true);
}
void Horde::FindDestination(XMFLOAT3 _dest)
{
	auto pEntityManager = SceneManager::Get()->GetActiveScene<SceneWorld>()->p_EntityManager;
	p_TargetBuilding = pEntityManager->GetNearBuilding(_dest.x, _dest.y, _dest.z, TEAM_ZENTIL);
}

Horde::Horde(HordeType _HordeType, EntityFacIndex _Entity, int _Count)
{
	m_FacEntityIndex = (int)_Entity;
	m_Material = GetMaterial();
	m_HordeType = (int)_HordeType;
	m_EntityCount = _Count;

	p_Group = new Group();
}

std::string Horde::GetMaterial()
{
	switch (m_FacEntityIndex)
	{
	case((int)EntityFacIndex::KAMRADE):
			return MATERIAL_KAMRADENEMY_NAME;
	case((int)EntityFacIndex::CUTTER):
			return MATERIAL_CUTTERENEMY_NAME;
	case((int)EntityFacIndex::MINION):
			return MATERIAL_MINIONSENEMY_NAME;
	case((int)EntityFacIndex::BODYBUILDER):
			return MATERIAL_BODYBUILDERENEMY_NAME;
	case((int)EntityFacIndex::SOLDIER):
			return MATERIAL_SOLDIERENEMY_NAME;
	default:
			return MATERIAL_MINIONSENEMY_NAME;
	}
}

void Horde::SpawnHorde(float _x, float _y, float _z)
{
	p_Group->Init(3, 4, 2.0f);

	for (int i = 0; i < m_EntityCount; i++)
	{
		Monkey* monk = FactoryManager::Get()->CreateEntity<Monkey>((EntityFacIndex)m_FacEntityIndex);

		monk->SetTeam(TEAM_ARNO);
		monk->AddEnemy(TEAM_ZENTIL);
		monk->SetGroup(p_Group);
		monk->GetMesh()->SetMaterial(m_Material);

		XMFLOAT3 spawnPosition = {_x, _y, _z};
		spawnPosition.y += monk->GetTransformComponent()->GetScale().y / 2.0f;
		monk->SetPosition(spawnPosition);

		if (i == 0) //Leader
		{
			p_Group->SetLeader(monk);
			continue;
		}
		p_Group->AddFollower(monk);
	}

	FindDestination(p_Group->GetLeader()->GetPosition());
	GoToDestination();
}
void Horde::Update(float _dt)
{
	if (IsKilled())
		return;

	if(p_Group->IsDestroy())
	{
		m_IsKilled = true;
		return;
	}

	if (p_Group->GetLeader()->IsDead())
		p_Group->FindNewLeader();

	m_CheckBuildingTimer += _dt;
	if (m_CheckBuildingTimer < HORDE_CHECK_BUILDING_TIME)
		return;
	m_CheckBuildingTimer = 0.0f;

	if (p_Group->GetLeader()->GetStateMachineState() == (int)LivingEntityStates::ATTACK)
		return;

	FindDestination(p_Group->GetLeader()->GetPosition());
	GoToDestination();
}

HordeStorage::HordeStorage()
{
	m_HordeFactories.push_back(new HordeFactoryKamrade());
	m_HordeFactories.push_back(new HordeFactoryCutter());
	m_HordeFactories.push_back(new HordeFactoryBodybuilder());
	m_HordeFactories.push_back(new HordeFactorySoldier());
}
HordeStorage* HordeStorage::Get()
{
	if (p_Instance == nullptr)
		p_Instance = new HordeStorage();

	return p_Instance;
}

void Wave::Reset()
{
	for(auto horde : m_Hordes)
	{
		int entityFacID = horde->m_FacEntityIndex;
		delete(horde);
		horde = HordeStorage::GetHorde((HordeType)entityFacID);
	}
}
void Wave::AddHorde(Horde* _pHorde)
{
	m_Hordes.push_back(_pHorde);
	m_Difficulty += _pHorde->m_Difficulty;
}
void Wave::SpawnWave(float _x, float _y, float _z)
{
	FactoryManager* FacM = FactoryManager::Get();
	for (auto horde : m_Hordes)
	{
		float nx = _x + (rand() % 10 - 5);
		float nz = _z + (rand() % 10 - 5);
		horde->SpawnHorde(nx, _y, nz);
	}
		
	m_IsStarted = true;
}
void Wave::Update(float _dt)
{
	for (auto horde : m_Hordes)
		horde->Update(_dt);
}


Level::Level(float _waveDuration)
{
	m_DurationBetweenWave = _waveDuration;
}
void Level::Update(float _dt)
{
	m_Timer += ECS::Get().GetDeltaTime();

	if (m_Timer >= m_DurationBetweenWave && m_ActualWave < m_Waves.size())
	{
		m_Timer = 0.0f;
		XMFLOAT3 nexusPos = SceneManager::Get()->GetActiveScene<SceneWorld>()->m_Enemy_Nexus->GetPosition();
		SpawnWave(m_ActualWave, nexusPos.x, 0, nexusPos.z);
		m_ActualWave++;
	}

	for (auto wave : m_Waves)
	{
		if (wave->m_IsStarted == false)
			continue;
		wave->Update(_dt);
	}
		
}
void Level::AddWave(Wave* _pWave)
{
	m_Waves.push_back(_pWave); 
	m_Duration = m_Waves.size() * m_DurationBetweenWave;
}
void Level::SpawnWave(int _WaveIndex, float _x, float _y, float _z)
{
	m_Waves[_WaveIndex]->SpawnWave(_x, _y, _z);
}
