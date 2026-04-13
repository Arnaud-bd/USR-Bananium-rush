#pragma once

template <typename T>
class Factory
{
public:
	virtual T* Create() = 0;
};

class FactoryEntity : public Factory<LivingEntity> {};

class FactoryCard : public Factory<Card> {};

// - ENTITY FACTORY -

enum class EntityFacIndex
{
	LIVING_ENTITY,
	KAMRADE,
	CUTTER,
	DEPOSIT,
	BARRACK,
	NEXUS,
	MINION,
	HEALTOWER,
	BODYBUILDER,
	SOLDIER,

	COUNT
};

class FactoEntity_LivingEntity : public FactoryEntity
{
public: LivingEntity* Create() override;
};
class FactoEntity_Kamrade : public FactoryEntity
{
	public: LivingEntity* Create() override;
};
class FactoEntity_Cutter : public FactoryEntity
{
	public: LivingEntity* Create() override;
};
class FactoEntity_Ore : public FactoryEntity
{
public: LivingEntity* Create() override;
};
class FactoEntity_Barrack : public FactoryEntity
{
public: LivingEntity* Create() override;
};
class FactoEntity_Nexus : public FactoryEntity
{
public: LivingEntity* Create() override;
};
class FactoEntity_Minion : public FactoryEntity
{
public: LivingEntity* Create() override;
};
class FactoEntity_HealTower : public FactoryEntity
{
public: LivingEntity* Create() override;
};
class FactoEntity_Bodybuilder : public FactoryEntity
{
public: LivingEntity* Create() override;
};
class FactoEntity_Soldier : public FactoryEntity
{
public: LivingEntity* Create() override;
};


// - CARD FACTORY -

enum class CardFacIndex
{
	KAMRADE,
	CUTTER,
	BARRACK,
	MINION,
	HEALTOWER,
	BODYBUILDER,
	SOLDIER,

	COUNT
};

class FactoCardKamrade : public FactoryCard
{
	public: Card* Create() override;
};
class FactoCardCutter : public FactoryCard
{
	public: Card* Create() override;
};
class FactoCardBarrack : public FactoryCard
{
	public: Card* Create() override;
};
class FactoCardMinion : public FactoryCard
{
	public: Card* Create() override;
};
class FactoCardHealTower : public FactoryCard
{
	public: Card* Create() override;
};
class FactoCardBodybuilder : public FactoryCard
{
	public: Card* Create() override;
};
class FactoCardSoldier : public FactoryCard
{
	public: Card* Create() override;
};

class FactoryManager
{

	std::vector< FactoryCard* > m_FactoriesCard;
	std::vector< FactoryEntity* > m_FactoriesEntity;

	static inline FactoryManager* p_Instance = nullptr;

public:
	FactoryManager();
	static FactoryManager* Get();

	template<typename T>
	T* CreateEntity(EntityFacIndex index) { return (T*)m_FactoriesEntity[(int)index]->Create(); };
	template<typename T>
	T* CreateCard(CardFacIndex index) { return (T*)m_FactoriesCard[(int)index]->Create(); };

	LivingEntity* CreateEntity(int index) { return m_FactoriesEntity[(int)index]->Create(); };
	Card* CreateCard(int index) { return m_FactoriesCard[(int)index]->Create(); };

};