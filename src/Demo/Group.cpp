#include "pch.h"


XMFLOAT3 CirclePlacement::GetPosition(int index)
{
	if (index >= m_Max)
		return m_Monkeys[0]->GetGroup()->GetLeader()->GetPosition();

	Monkey* monkey = m_Monkeys[index];

	XMFLOAT3 pos = monkey->GetGroup()->GetLeader()->GetPosition();
	float theta = (index / (float)m_Max) * Maths::Pi * 2.0f;

	pos.x += m_Radius * cos(theta);
	pos.z += m_Radius * sin(theta);

	return pos;
}

void CirclePlacement::Add(Monkey* _pMonkey)
{
	int lastIndex = m_Monkeys.size();

	if (lastIndex >= m_Max)
		return;

	m_Monkeys.push_back(_pMonkey);
	XMFLOAT3 pos = GetPosition(lastIndex);

	_pMonkey->SetPosition(pos);
}

DiskPlacement::DiskPlacement(int _maxCircles, int _entityCountFirstCircle, float _startRadius)
{
	m_MaxCircles = _maxCircles;
	for (int i = 0; i < m_MaxCircles; i++)
	{
		CirclePlacement* circle = new CirclePlacement();
		circle->m_Max = _entityCountFirstCircle * (i + 1);
		circle->m_Radius = _startRadius * (i + 1);
		m_Circles.push_back(circle);
	}
}

void DiskPlacement::Add(Monkey* _pMonkey)
{
	float radius = _pMonkey->GetTransformComponent()->GetScale().x * 2.0f;
	for (int i = 0; i < m_Circles.size(); i++)
	{
		CirclePlacement* circle = m_Circles[i];
		if (circle->IsFull())
			continue;
		circle->Add(_pMonkey);
		return;
	}
}

void Group::Destroy()
{
	if (m_isDestroy)
		return;
	EntityManager* pEntityManager = SceneManager::Get()->GetActiveScene<SceneGameplay>()->p_EntityManager;
	UILivingEntityInfos* pUIEntityInfo = SceneManager::Get()->GetActiveScene<SceneWorld>()->p_UIEntityInfo;
	for (auto monkey : m_InactiveMonkeys)
	{
		pEntityManager->RemoveLivingEntity(monkey);
		pUIEntityInfo->RemoveEntityShown(monkey);
	}
	if(pUIEntityInfo->GetLivingEntityCount() == 0)
		pUIEntityInfo->Hide();
	m_InactiveMonkeys.clear();
	m_isDestroy = true;
}

void Group::FindNewLeader()
{
	for (auto circle : m_Placement.m_Circles)
	{
		auto& monkeys = circle->GetMonkeys();
		for (int i = 0; i < monkeys.size(); i++)
		{
			auto monkey = monkeys[i];
			if (monkey->IsDead())
				continue;
			SetLeader(monkey);
			monkeys.erase(monkeys.begin() + i);
			return;
		}
	}
	//If no leader available, group no longer exist
	Destroy();
}

void Group::ReplaceMonkeys()
{
	//Make a copy of old monkeys vector
	std::vector<std::vector<Monkey*>> tmpMonkeys;
	for (auto nCirclePlacement : m_Placement.m_Circles)
	{
		std::vector<Monkey*> circleMonkeys = nCirclePlacement->GetMonkeys();

		int lastIndex = circleMonkeys.size() - 1;
		for(int i = lastIndex; i >= 0; i--)
		{
			Monkey* monk = circleMonkeys[i];
			if (monk->IsDead() == false)
				continue;
			circleMonkeys.erase(circleMonkeys.begin() + i);
		}
		if (circleMonkeys.size() == 0)
			continue;

		tmpMonkeys.push_back(circleMonkeys);
	}

	Init(m_Placement.m_MaxCircles, m_Placement.m_Circles[0]->m_Max, m_Placement.m_Circles[0]->m_Radius);

	//Replace old monkeys with new positions
	for (int i = 0; i < tmpMonkeys.size(); i++)
	{
		auto& nMonkeysPlacement = tmpMonkeys[i];
		for (auto& nMonkeyPlacement : nMonkeysPlacement)
			AddFollower(nMonkeyPlacement);
	}
}

void Group::ShowHP()
{
	UILivingEntityInfos* pUIEntityInfo = SceneManager::Get()->GetActiveScene<SceneWorld>()->p_UIEntityInfo;
	pUIEntityInfo->ShowEntityHP(p_Leader);

	for (auto circle : m_Placement.m_Circles)
	{
		auto& monkeys = circle->GetMonkeys();
		for (int i = 0; i < monkeys.size(); i++)
		{
			pUIEntityInfo->ShowEntityHP(monkeys[i]);
		}
			
	}
	for(auto monkey : m_InactiveMonkeys)
	{
		pUIEntityInfo->RemoveEntityShown(monkey);
		pUIEntityInfo->ShowEntityHP(monkey);
	}	
}

void Group::Init(int _maxCircles, int _entityCountFirstCircle, float _startRadius)
{
	m_Placement = DiskPlacement(_maxCircles, _entityCountFirstCircle, _startRadius);
}

void Group::AddFollower(Monkey* _pMonkey)
{
	m_Placement.Add(_pMonkey);
}

void Group::SetLeaderDestination(XMFLOAT3 _dest, bool _isAttackPriority)
{
	p_Leader->SetDestination(_dest);
	p_Leader->SetAttackPriority(_isAttackPriority);

	ReplaceMonkeys();

	float halfScaleY = p_Leader->GetTransformComponent()->GetScale().y/2.0f;
	for(auto circle : m_Placement.m_Circles)
	{
		auto monkeys = circle->GetMonkeys();
		for (int i = 0; i < monkeys.size(); i++)
		{
			auto monkey = monkeys[i];
			XMFLOAT3 pos = circle->GetPosition(i);
			pos.x += _dest.x - p_Leader->GetPosition().x;
			pos.y -= halfScaleY; // To cancel offset in p_Leader->SetDestination
			pos.z += _dest.z - p_Leader->GetPosition().z;

			monkey->SetDestination(pos);
			monkey->HideDestination();
			monkey->SetAttackPriority(_isAttackPriority);
		}
	}
}
