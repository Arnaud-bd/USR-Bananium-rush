#include "pch.h"

void CardKamrade::OnPlacement(XMFLOAT3 _worldPos)
{
	Group* group = new Group();
	group->Init(1, 4, 2.0f);

	for (int i = 0; i < 5; i++) //Get Entity number from card data
	{
		Kamrade* kamrade = FactoryManager::Get()->CreateEntity<Kamrade>(EntityFacIndex::KAMRADE);

		kamrade->SetTeam(TEAM_ZENTIL);
		kamrade->AddEnemy(TEAM_ARNO);

		XMFLOAT3 spawnPosition = _worldPos;
		spawnPosition.y += kamrade->GetTransformComponent()->GetScale().y / 2.0f;
		kamrade->SetPosition(spawnPosition);
		kamrade->SetGroup(group);
		
		if (i == 0) //Leader
		{
			group->SetLeader(kamrade);
			continue;
		}
		group->AddFollower(kamrade);
	}
}

void CardCutter::OnPlacement(XMFLOAT3 _worldPos)
{
	Group* group = new Group();
	group->Init(1, 4, 2.0f);

	for (int i = 0; i < 5; i++) //Get Entity number from card data
	{
		Cutter* cutter = FactoryManager::Get()->CreateEntity<Cutter>(EntityFacIndex::CUTTER);

		cutter->SetTeam(TEAM_ZENTIL);
		cutter->AddEnemy(TEAM_ARNO);

		XMFLOAT3 spawnPosition = _worldPos;
		spawnPosition.y += cutter->GetTransformComponent()->GetScale().y / 2.0f;
		cutter->SetPosition(spawnPosition);
		cutter->SetGroup(group);

		if (i == 0) //Leader
		{
			group->SetLeader(cutter);
			continue;
		}
		group->AddFollower(cutter);
	}
}

void CardBarrack::OnPlacement(XMFLOAT3 _worldPos)
{
	Barrack* barrack = FactoryManager::Get()->CreateEntity<Barrack>(EntityFacIndex::BARRACK);

	barrack->SetTeam(TEAM_ZENTIL);
	barrack->AddEnemy(TEAM_ARNO);

	XMFLOAT3 spawnPosition = _worldPos;
	spawnPosition.y += barrack->GetTransformComponent()->GetScale().y / 2.0f;
	barrack->SetPosition(spawnPosition);
}

void CardMinion::OnPlacement(XMFLOAT3 _worldPos)
{
	Group* group = new Group();
	group->Init(1, 4, 2.0f);

	for (int i = 0; i < 5; i++) //Get Entity number from card data
	{
		Minion* kamrade = FactoryManager::Get()->CreateEntity<Minion>(EntityFacIndex::MINION);

		kamrade->SetTeam(TEAM_ZENTIL);
		kamrade->AddEnemy(TEAM_ARNO);

		XMFLOAT3 spawnPosition = _worldPos;
		spawnPosition.y += kamrade->GetTransformComponent()->GetScale().y / 2.0f;
		kamrade->SetPosition(spawnPosition);
		kamrade->SetGroup(group);

		if (i == 0) //Leader
		{
			group->SetLeader(kamrade);
			continue;
		}
		group->AddFollower(kamrade);
	}
}

void CardHealTower::OnPlacement(XMFLOAT3 _worldPos)
{
	HealTower* healTower = FactoryManager::Get()->CreateEntity<HealTower>(EntityFacIndex::HEALTOWER);

	healTower->SetTeam(TEAM_ZENTIL);
	healTower->AddEnemy(TEAM_ARNO);

	XMFLOAT3 spawnPosition = _worldPos;
	spawnPosition.y += healTower->GetTransformComponent()->GetScale().y / 2.0f;
	healTower->SetPosition(spawnPosition);
}
void CardBodybuilder::OnPlacement(XMFLOAT3 _worldPos)
{
	Group* group = new Group();
	group->Init(1, 4, 2.0f);

	for (int i = 0; i < 5; i++)
	{
		Bodybuilder* bodybuilder = FactoryManager::Get()->CreateEntity<Bodybuilder>(EntityFacIndex::BODYBUILDER);

		bodybuilder->SetTeam(TEAM_ZENTIL);
		bodybuilder->AddEnemy(TEAM_ARNO);

		XMFLOAT3 spawnPosition = _worldPos;
		spawnPosition.y += bodybuilder->GetTransformComponent()->GetScale().y / 2.0f;
		bodybuilder->SetPosition(spawnPosition);
		bodybuilder->SetGroup(group);

		if (i == 0) //Leader
		{
			group->SetLeader(bodybuilder);
			continue;
		}
		group->AddFollower(bodybuilder);
	}
}

void CardSoldier::OnPlacement(XMFLOAT3 _worldPos)
{
	Group* group = new Group();
	group->Init(1, 4, 2.0f);

	for (int i = 0; i < 5; i++)
	{
		Soldier* soldier = FactoryManager::Get()->CreateEntity<Soldier>(EntityFacIndex::SOLDIER);

		soldier->SetTeam(TEAM_ZENTIL);
		soldier->AddEnemy(TEAM_ARNO);

		XMFLOAT3 spawnPosition = _worldPos;
		spawnPosition.y += soldier->GetTransformComponent()->GetScale().y / 2.0f;
		soldier->SetPosition(spawnPosition);
		soldier->SetGroup(group);

		if (i == 0) //Leader
		{
			group->SetLeader(soldier);
			continue;
		}
		group->AddFollower(soldier);
	}
}
