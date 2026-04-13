#pragma once

enum class EntityMapIndex
{
	MONKEY,
	BUILDING,
	BARRACK,
	ORE,
	NEXUS,
	HEALTOWER,
};

class EntityManager final
{
	std::vector<LivingEntity*> m_LivingEntities = std::vector<LivingEntity*>();
	std::vector<LivingEntity*> m_DestroyedEntities = std::vector<LivingEntity*>();
	std::map<int, std::vector<LivingEntity*>> m_EntitiesMap;

	void EraseLivingEntity(LivingEntity* _pLE);
	void ClearDestroyedEntities();

public:
	EntityManager() = default;
	void Update(float _dt);

	std::vector<LivingEntity*>& GetAllEntities() { return m_LivingEntities; }
	LivingEntity* GetLivingEntity(int _EntityIDInScene);
	std::vector<LivingEntity*>& GetEntitiesByType(EntityMapIndex _index) { return m_EntitiesMap[(int)_index]; }
	Building* GetNearBuilding(float _x, float _y, float _z, std::string _team);
	Building* GetNearBuilding(XMFLOAT3 _pos, std::string _team) { GetNearBuilding(_pos.x, _pos.y, _pos.z, _team); }

	void AddLivingEntity(LivingEntity* _Entity);
	void AddLivingEntity(LivingEntity* _Entity, EntityMapIndex _index);
	void RemoveLivingEntity(LivingEntity* _Entity) { m_DestroyedEntities.push_back(_Entity); }
	void RemoveLivingEntityFromMap(LivingEntity* _Entity, EntityMapIndex _index);
	
	friend class GameManager;
};

