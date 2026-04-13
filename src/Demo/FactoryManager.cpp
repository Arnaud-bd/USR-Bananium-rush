#include "pch.h"

FactoryManager::FactoryManager()
{
	m_FactoriesEntity.push_back(new FactoEntity_LivingEntity());	// 0	
	m_FactoriesEntity.push_back(new FactoEntity_Kamrade());			// 1 
	m_FactoriesEntity.push_back(new FactoEntity_Cutter());			// 2 
	m_FactoriesEntity.push_back(new FactoEntity_Ore());				// 3	
	m_FactoriesEntity.push_back(new FactoEntity_Barrack());			// 4	
	m_FactoriesEntity.push_back(new FactoEntity_Nexus());			// 5	
	m_FactoriesEntity.push_back(new FactoEntity_Minion());			// 6	
	m_FactoriesEntity.push_back(new FactoEntity_HealTower());		// 7	
	m_FactoriesEntity.push_back(new FactoEntity_Bodybuilder());		// 8
	m_FactoriesEntity.push_back(new FactoEntity_Soldier());			// 9

	m_FactoriesCard.push_back(new FactoCardKamrade());	// 0 
	m_FactoriesCard.push_back(new FactoCardCutter());	// 1 
	m_FactoriesCard.push_back(new FactoCardBarrack());	// 2 
	m_FactoriesCard.push_back(new FactoCardMinion());	// 3 
	m_FactoriesCard.push_back(new FactoCardHealTower());// 4 
	m_FactoriesCard.push_back(new FactoCardBodybuilder());	// 5 
	m_FactoriesCard.push_back(new FactoCardSoldier());	// 6 
}

FactoryManager* FactoryManager::Get()
{
	if (p_Instance == nullptr)
		p_Instance = new FactoryManager();
	return p_Instance;
}

// 0
LivingEntity* FactoEntity_LivingEntity::Create()
{
	LivingEntity* LE = new LivingEntity();
	SceneManager::Get()->GetActiveScene<SceneGameplay>()->p_EntityManager->AddLivingEntity(LE);
	return LE;
}
// 1
LivingEntity* FactoEntity_Kamrade::Create()
{
	Kamrade* kamrade = new Kamrade();
	auto sc = SceneManager::Get()->GetActiveScene<SceneGameplay>();
	sc->p_EntityManager->AddLivingEntity(kamrade);
	sc->p_EntityManager->AddLivingEntity(kamrade, EntityMapIndex::MONKEY);
	return kamrade;
}
// 2
LivingEntity* FactoEntity_Cutter::Create()
{
	Cutter* cutter = new Cutter();
	auto sc = SceneManager::Get()->GetActiveScene<SceneGameplay>();
	sc->p_EntityManager->AddLivingEntity(cutter);
	sc->p_EntityManager->AddLivingEntity(cutter, EntityMapIndex::MONKEY);
	return cutter;
}
// 3
LivingEntity* FactoEntity_Ore::Create()
{
	Deposit* deposit = new Deposit();
	SceneManager::Get()->GetActiveScene<SceneGameplay>()->p_EntityManager->AddLivingEntity(deposit);
	return deposit;
}
// 4
LivingEntity* FactoEntity_Barrack::Create()
{
	Barrack* barrack = new Barrack();
	auto sc = SceneManager::Get()->GetActiveScene<SceneGameplay>();
	sc->p_EntityManager->AddLivingEntity(barrack);
	sc->p_EntityManager->AddLivingEntity(barrack, EntityMapIndex::BUILDING);
	sc->p_EntityManager->AddLivingEntity(barrack, EntityMapIndex::BARRACK);
	return barrack;
}
// 5
LivingEntity* FactoEntity_Nexus::Create()
{
	Nexus* nexus = new Nexus();
	auto sc = SceneManager::Get()->GetActiveScene<SceneGameplay>();
	sc->p_EntityManager->AddLivingEntity(nexus);
	sc->p_EntityManager->AddLivingEntity(nexus, EntityMapIndex::BUILDING);
	return nexus;
}
// 6
LivingEntity* FactoEntity_Minion::Create()
{
	Minion* minion = new Minion();

	auto sc = SceneManager::Get()->GetActiveScene<SceneGameplay>();
	sc->p_EntityManager->AddLivingEntity(minion);
	sc->p_EntityManager->AddLivingEntity(minion, EntityMapIndex::MONKEY);
	return minion;
}
// 7
LivingEntity* FactoEntity_HealTower::Create()
{
	HealTower* tower = new HealTower();
	auto sc = SceneManager::Get()->GetActiveScene<SceneGameplay>();
	sc->p_EntityManager->AddLivingEntity(tower);
	sc->p_EntityManager->AddLivingEntity(tower, EntityMapIndex::BUILDING);
	sc->p_EntityManager->AddLivingEntity(tower, EntityMapIndex::HEALTOWER);
	return tower;
}
// 8
LivingEntity* FactoEntity_Bodybuilder::Create()
{
	Bodybuilder* bodybuilder = new Bodybuilder();

	auto sc = SceneManager::Get()->GetActiveScene<SceneGameplay>();
	sc->p_EntityManager->AddLivingEntity(bodybuilder);
	sc->p_EntityManager->AddLivingEntity(bodybuilder, EntityMapIndex::MONKEY);
	return bodybuilder;
}
// 9
LivingEntity* FactoEntity_Soldier::Create()
{
	Soldier* soldier = new Soldier();

	auto sc = SceneManager::Get()->GetActiveScene<SceneGameplay>();
	sc->p_EntityManager->AddLivingEntity(soldier);
	sc->p_EntityManager->AddLivingEntity(soldier, EntityMapIndex::MONKEY);
	return soldier;
}


Card* FactoCardKamrade::Create()
{
	return new CardKamrade();
}
Card* FactoCardCutter::Create()
{
	return new CardCutter();
}
Card* FactoCardBarrack::Create()
{
	return new CardBarrack();
}
Card* FactoCardMinion::Create()
{
	return new CardMinion();
}
Card* FactoCardHealTower::Create()
{
	return new CardHealTower();
}
Card* FactoCardBodybuilder::Create()
{
	return new CardBodybuilder();
}
Card* FactoCardSoldier::Create()
{
	return new CardSoldier();
}
