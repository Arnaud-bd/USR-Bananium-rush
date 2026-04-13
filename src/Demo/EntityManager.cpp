#include "pch.h"

void EntityManager::EraseLivingEntity(LivingEntity* _pLE)
{
	for (int i = 0; i < m_LivingEntities.size(); i++)
	{
		if (m_LivingEntities[i] == _pLE)
		{
			m_LivingEntities.erase(m_LivingEntities.begin() + i);
			return;
		}
	}
}

void EntityManager::ClearDestroyedEntities()
{
	for(LivingEntity* entity : m_DestroyedEntities)
	{
		EraseLivingEntity(entity);
		entity->Destroy();
		delete(entity);
	}
	m_DestroyedEntities.clear();
}

void EntityManager::Update(float _dt)
{
	for(LivingEntity* entity : m_LivingEntities)
		entity->Update(_dt);

	ClearDestroyedEntities();
}

LivingEntity* EntityManager::GetLivingEntity(int _EntityIDInScene)
{
	for(LivingEntity* entity : m_LivingEntities)
	{
		if (entity->m_Entity == _EntityIDInScene)
			return entity;
		if(entity->m_RangeEntity == _EntityIDInScene)
			return entity;
	}

	return nullptr;
}

Building* EntityManager::GetNearBuilding(float _x, float _y, float _z, std::string _team)
{
	float minDistance = FLT_MAX;
	auto buildings = GetEntitiesByType(EntityMapIndex::BUILDING);
	LivingEntity* building = nullptr;
	for (LivingEntity* build : buildings)
	{
		if (build->GetTeam() != _team)
			continue;
		XMFLOAT3 pos = build->GetPosition();
		XMFLOAT3 v = XMFLOAT3();
		v.x = pos.x - _x;
		v.y = pos.y - _y;
		v.z = pos.z - _z;
		float distance = sqrt(pow(v.x, 2)+ pow(v.y, 2)+ pow(v.y, 2));
		if (distance > minDistance)
			continue;
		minDistance = distance;
		building = build;
	}

	return (Building*)building;
}


void EntityManager::AddLivingEntity(LivingEntity* _Entity)
{
	_Entity->p_EntityManager = this;
	m_LivingEntities.push_back(_Entity);
}

void EntityManager::AddLivingEntity(LivingEntity* _Entity, EntityMapIndex _index)
{
	m_EntitiesMap[(int)_index].push_back(_Entity);
}

void EntityManager::RemoveLivingEntityFromMap(LivingEntity* _Entity, EntityMapIndex _index)
{
	auto& vector = GetEntitiesByType(_index);

	for (int i = 0; i < vector.size(); i++)
	{
		LivingEntity* entity = vector[i];
		if (entity == _Entity)
		{
			vector.erase(vector.begin() + i);
			return;
		}
	}

}
