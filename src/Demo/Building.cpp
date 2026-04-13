#include "pch.h"

Building::Building() : LivingEntity()
{
}

void Building::OnClick()
{
	UILivingEntityInfos* pUILife = SceneManager::Get()->GetActiveScene<SceneWorld>()->p_UIEntityInfo;
	pUILife->ClearEntitiesShown();
	pUILife->ShowEntityHP(this);
}

void Building::Build()
{
	if (m_IsBuilt)
		return;

	m_CurrentBuildingStep++;

	p_Transform->SetScale(1.0f, 1.0f - (m_FinalBuildingStep - m_CurrentBuildingStep) * 0.01f, 1.0f);

	XMFLOAT3 pos = p_Transform->GetWorldPosition();
	p_Transform->SetWorldPosition({ pos.x, 0.0f + p_Transform->GetScale().y * 0.5f, pos.z});

	if (m_CurrentBuildingStep == m_FinalBuildingStep)
	{
		m_IsBuilt = true;
		OnBuild();
	}
}

void Building::OnDeath()
{
	UILivingEntityInfos* pUILife = SceneManager::Get()->GetActiveScene<SceneWorld>()->p_UIEntityInfo;
	pUILife->RemoveEntityShown(this);
	p_EntityManager->RemoveLivingEntity(this);
}
void Building::OnDestroy()
{
	p_EntityManager->RemoveLivingEntityFromMap(this, EntityMapIndex::BUILDING);
}

/// BARRACK ///

Barrack::Barrack() : Building()
{
	p_Mesh->SetMaterial(MATERIAL_BARACKBUILD_NAME);
	p_Transform->SetScale(1.0f, 0.01f, 1.0f);
	SetMaxHealth(300);
	SetHealth(300);
	SetName("Barrack");
}

void Barrack::OnBuild()
{
	SetRangeDiameter(15.0f);
	p_RangeCollider->AddToLayer(PLACEMENT_COLLIDERS_LAYER);


}
void Barrack::OnDestroy()
{
	Building::OnDestroy();
	p_EntityManager->RemoveLivingEntityFromMap(this, EntityMapIndex::BARRACK);
}

/// NEXUS ///

Nexus::Nexus() : Building()
{
	m_IsBuilt = true;

	SetMaxHealth(500);
	SetHealth(500);

	SetName("Nexus");
	SetRangeDiameter(10.0f);
	ShowRange();
}

bool Nexus::TimerNexus()
{
	m_Timer += ECS::Get().GetDeltaTime();
	if (m_Timer >= m_Time)
	{
		m_Timer = 0;
		return true;
	}
	return false;
}

/// ORES ///

Deposit::Deposit() : Building()
{

	m_IsBuilt = true;
	SetRangeDiameter(10.0f);
	SetName("Deposit");
}

void Deposit::SetType(OreType _type)
{
	m_type = _type;
}

/// HealTower ///

HealTower::HealTower()
{
	p_Mesh->SetMaterial(MATERIAL_BARACKHEALBUILD_NAME);
	p_Transform->SetScale(1.0f, 0.01f, 1.0f);

	SetMaxHealth(100);
	SetHealth(100);

	SetName("HealTower");
}

void HealTower::OnBuild()
{
	SetRangeDiameter(10.0f);
}

void HealTower::OnClick()
{
	Building::OnClick();

	if (m_showingRange)
	{
		HideRange();
		m_showingRange = false;
	}
	else
	{
		ShowRange();
		m_showingRange = true;
	}
}

void HealTower::OnDestroy()
{
	Building::OnDestroy();
	p_EntityManager->RemoveLivingEntityFromMap(this, EntityMapIndex::HEALTOWER);
}
