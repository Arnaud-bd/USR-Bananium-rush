#include "pch.h"

Monkey::Monkey() : LivingEntity()
{
	m_DestColorList["Green"] = {0.0f, 1.0f, 0.0f, 0.5f};
	m_DestColorList["Red"] = { 1.0f, 0.0f, 0.0f, 0.5f };

	ECS& ecs = ECS::Get();
	p_Velocity = ecs.AddComponent<VelocityComponent>(m_Entity);

	m_DestEntity = ecs.CreateEntity();
	p_DestTransform = ecs.AddComponent<TransformComponent>(m_DestEntity);
	p_DestTransform->SetWorldPosition({ 0, FLT_MAX, 0 });
	p_DestTransform->SetScale(0.5f, 1.5f, 0.5f);

	float Rot = Maths::Pi;
	p_DestTransform->RotateWorld(0.f, Rot, 0.f);

	p_DestMesh = ecs.AddComponent<MeshComponent>(m_DestEntity);
	p_DestMesh->SetGeometry(GeometryManager::Get()->GetGeometry(GEO_TYPE::PYRAMID));
	SetDestColor("Green");
	p_DestMesh->SetMaterial(MATERIAL_DEFAULT_COLOR_A_NAME);
	HideDestination();

	m_BaseColor = { 0.5f,0.5f, 0.5f, 1.0f };
	p_Mesh->SetColor(m_BaseColor);

	InitStateMachine();
}

void Monkey::Animation_Attack()
{
	float range = GetRange();
	if (m_AttackAnimationRunning == false || range > 10.0f)
		return;

	if (m_AnimState == AttackAnimationState::Start)
	{
		if (m_AttackTarget == -1)
		{
			m_AnimState = AttackAnimationState::Start;
			m_AttackAnimationRunning = false;
			m_SavedAttackDestPos = m_SavedStartPos;
			StopMoving();
			return;
		}
		m_SavedStartPos = GetPosition();
		m_SavedAttackDestPos = ECS::Get().GetComponent<TransformComponent>(m_AttackTarget)->GetWorldPosition();
		DashTo(m_SavedAttackDestPos);
		m_AnimState = AttackAnimationState::OnWay;
	}
	else if (m_AnimState == AttackAnimationState::OnWay)
	{
		if (m_AttackTarget == -1)
		{
			m_AnimState = AttackAnimationState::Start;
			m_AttackAnimationRunning = false;
			m_SavedAttackDestPos = m_SavedStartPos;
			StopMoving();
			return;
		}
		m_SavedAttackDestPos = ECS::Get().GetComponent<TransformComponent>(m_AttackTarget)->GetWorldPosition();
		DashTo(m_SavedAttackDestPos);

		XMFLOAT3 Distance = { 0,0,0 };
		Distance.x = abs(GetPosition().x - m_SavedAttackDestPos.x);
		Distance.y = abs(GetPosition().y - m_SavedAttackDestPos.y);
		Distance.z = abs(GetPosition().z - m_SavedAttackDestPos.z);

		if (Distance.x <= 0.1f && Distance.y <= 0.1f && Distance.z <= 0.1f)
		{
			m_AnimState = AttackAnimationState::GoBack;
			DashTo(m_SavedStartPos);
		}
	}
	else if (m_AnimState == AttackAnimationState::GoBack)
	{
		XMFLOAT3 Distance = { 0,0,0 };
		Distance.x = abs(m_SavedStartPos.x - GetPosition().x);
		Distance.y = abs(m_SavedStartPos.y - GetPosition().y);
		Distance.z = abs(m_SavedStartPos.z - GetPosition().z);

		if (Distance.x <= 0.1f && Distance.y <= 0.1f && Distance.z <= 0.1f)
		{
			m_AnimState = AttackAnimationState::Start;
			m_AttackAnimationRunning = false;
			m_SavedAttackDestPos = m_SavedStartPos;
			StopMoving();
			return;
		}
	}
}

void Monkey::InitStateMachine()
{
	if (p_StateMachine != nullptr)
		delete(p_StateMachine);

	p_StateMachine = new StateMachineMonkey();

	p_StateMachine->Initialize(this, (int)MonkeyStates::COUNT);
	//Dead
	ActionLivingEntity_Dead* pDead = p_StateMachine->CreateAction<ActionLivingEntity_Dead>((int)MonkeyStates::DEAD);
	//Idle
	ActionMonkey_Idle* pIdle = p_StateMachine->CreateAction<ActionMonkey_Idle>((int)MonkeyStates::IDLE);
	{	//-> Dead
		auto transition = pIdle->CreateTransition((int)MonkeyStates::DEAD);
		transition->AddCondition<ConditionLivingEntity_IsDead>();
	}
	{	//-> Attack
		auto transition = pIdle->CreateTransition((int)MonkeyStates::ATTACK);
		transition->AddCondition<ConditionLivingEntity_TargetInRange>();
	}
	{	//-> Move
		auto transition = pIdle->CreateTransition((int)MonkeyStates::MOVE);
		transition->AddCondition<ConditionMonkey_IsDestinationChanged>();
	}
	{	//-> Heal
		auto transition = pIdle->CreateTransition((int)MonkeyStates::HEAL);
		transition->AddCondition<ConditionMonkey_IsNearHealTower>();
		transition->AddCondition<ConditionMonkey_IsFullyHealed>(false);
	}
	//Attack
	ActionMonkey_Attack* pAttack = p_StateMachine->CreateAction<ActionMonkey_Attack>((int)MonkeyStates::ATTACK);
	{	//-> Dead
		auto transition = pAttack->CreateTransition((int)MonkeyStates::DEAD);
		transition->AddCondition<ConditionLivingEntity_IsDead>();
	}
	{	//-> Idle
		auto transition = pAttack->CreateTransition((int)MonkeyStates::IDLE);
		transition->AddCondition<ConditionMonkey_IsAttackPriority>(false);
		transition->AddCondition<ConditionLivingEntity_TargetInRange>(false);
	}
	{	//-> Move
		auto transition = pAttack->CreateTransition((int)MonkeyStates::MOVE);
		transition->AddCondition<ConditionMonkey_IsDestinationChanged>();
	}
	{	//-> Move
		auto transition = pAttack->CreateTransition((int)MonkeyStates::MOVE);
		transition->AddCondition<ConditionMonkey_IsAttackPriority>();
		transition->AddCondition<ConditionLivingEntity_TargetInRange>(false);
		transition->AddCondition<ConditionMonkey_IsDestinationReached>(false);
	}
	//Move
	ActionMonkey_Move* pMove = p_StateMachine->CreateAction<ActionMonkey_Move>((int)MonkeyStates::MOVE);
	{	//-> Dead
		auto transition = pMove->CreateTransition((int)MonkeyStates::DEAD);
		transition->AddCondition<ConditionLivingEntity_IsDead>();
	}
	{	//-> Idle
		auto transition = pMove->CreateTransition((int)MonkeyStates::IDLE);
		transition->AddCondition<ConditionMonkey_IsDestinationReached>();
	}
	{	//-> Attack
		auto transition = pMove->CreateTransition((int)MonkeyStates::ATTACK);
		transition->AddCondition<ConditionMonkey_IsAttackPriority>();
		transition->AddCondition<ConditionLivingEntity_TargetInRange>();
	}
	{	//-> Move
		auto transition = pMove->CreateTransition((int)MonkeyStates::MOVE);
		transition->AddCondition<ConditionMonkey_IsDestinationChanged>();
		transition->AddCondition<ConditionMinion_IsNearMine>(false);
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

	p_StateMachine->SetState((int)MonkeyStates::IDLE);
}

void Monkey::SetDestColor(std::string _name)
{
	p_DestMesh->SetColor(m_DestColorList[_name]);
}

void Monkey::Animation_DestEntity()
{
	p_DestTransform->RotateWorld(m_DestAnim_RotSpeed * ECS::GetDeltaTime(), 0, 0);

	XMFLOAT3 pos = p_DestTransform->GetWorldPosition();
	XMFLOAT3 newPos = pos;

	if (m_DestAnim_StateUping)
		newPos.y += m_DestAnim_Increment * ECS::GetDeltaTime();
	else
		newPos.y -= m_DestAnim_Increment * ECS::GetDeltaTime();

	p_DestTransform->SetWorldPosition(newPos);

	if (newPos.y >= m_DestAnim_HighAltitud)
	{
		m_DestAnim_StateUping = false;
	}
	else if (newPos.y <= m_DestAnim_LowAltitud)
	{
		m_DestAnim_StateUping = true;
	}
}

void Monkey::OnUpdate(float _dt)
{
	if (IsSelected())
	{
		ShowRange(); //HERE Warning called all frames 

		// Update Animation Pointer Destination :3
		Animation_DestEntity();
	}
	else
	{
		HideRange();
	}

	XMFLOAT3 pos = p_Transform->GetWorldPosition();

	p_Transform->SetWorldPosition({ pos.x, 0.0f + p_Transform->GetScale().y / 2.f, pos.z});
	Animation_Attack();
}

void Monkey::OnDeath()
{
	BloodManager* bloodManager = SceneManager::Get()->GetActiveScene<SceneWorld>()->p_BloodManager;
	XMFLOAT3 pos = p_Transform->GetWorldPosition();
	pos.y -= p_Transform->GetScale().y / 2.0f;
	pos.y += 0.1f;// 0.1f is to prevent z-fighting with the ground
	bloodManager->CreateBlood(pos);

	if (p_Group == nullptr)
		return;

	p_Group->SetFollowerInactive(this);
	if (p_Group->GetLeader()->IsDead())
		p_Group->FindNewLeader();
}

void Monkey::OnClick()
{
	m_IsSelected = false; // cancel Click() effect from LivingEntity

	if (p_Group == nullptr)
		return;

	auto* p_UIEntityInfo = SceneManager::Get()->GetActiveScene<SceneWorld>()->p_UIEntityInfo;
	p_UIEntityInfo->ClearEntitiesShown();
	p_Group->ShowHP();

	if(p_Group->GetLeader()->IsDead())
		p_Group->FindNewLeader();
	p_Group->GetLeader()->SetSelected(true);
}

void Monkey::SetDestination(XMFLOAT3 _dest)
{
	m_Destination = _dest;
	m_Destination.y += p_Transform->GetScale().y/2.0f;
	p_DestTransform->SetWorldPosition({ _dest.x , _dest.y + m_DestAnim_LowAltitud, _dest.z });
	GoTo(m_Destination);
}

void Monkey::Animation_Moving()
{
	XMFLOAT3 scale = p_Transform->GetScale();

	if (m_MoveAnim_StateScaling)
	{
		scale.y += m_MoveAnim_Increment * ECS::GetDeltaTime();
	}
	else
	{
		scale.y -= m_MoveAnim_Increment * ECS::GetDeltaTime();
	}

	p_Transform->SetScale(scale.x, scale.y, scale.z);

	if (scale.y >= m_MoveAnim_HighScale)
	{
		m_MoveAnim_StateScaling = false;
	}
	else if (scale.y <= m_MoveAnim_LowScale)
	{
		m_MoveAnim_StateScaling = true;
	}
}

void Monkey::Play_Attack()
{
	if (m_AttackAnimationRunning == false)
		m_AttackAnimationRunning = true;
}

void Monkey::StopMoving()
{
	p_Velocity->SetVelocity(0);
}

void Monkey::DashTo(XMFLOAT3 _Destination)
{
	XMFLOAT3 position = p_Transform->GetWorldPosition();
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR dest = XMLoadFloat3(&_Destination);
	XMVECTOR dir = dest - pos;
	XMVECTOR dist = XMVector3Length(dir);
	float distance = XMVectorGetX(dist);
	dir = XMVector3Normalize(dir);
	XMFLOAT3 direction;
	XMStoreFloat3(&direction, dir);

	LookTo(dir);

	p_Transform->SetForward(direction);
	p_Velocity->SetVelocity(20);
}

void Monkey::GoTo(XMFLOAT3 _Destination)
{
	m_Destination = _Destination;
	XMFLOAT3 position = p_Transform->GetWorldPosition();
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR dest = XMLoadFloat3(&m_Destination);
	XMVECTOR dir = dest - pos;
	XMVECTOR dist = XMVector3Length(dir);
	float distance = XMVectorGetX(dist);
	dir = XMVector3Normalize(dir);
	XMFLOAT3 direction;
	XMStoreFloat3(&direction, dir);

	LookTo(dir);

	m_AttackTarget = -1;
	p_Transform->SetForward(direction);
	p_Velocity->SetVelocity(m_MoveSpeed);
	if (p_StateMachine->GetCurrentState() == (int)MonkeyStates::MOVE)
		return;
	p_StateMachine->SetState((int)MonkeyStates::MOVE);
}

void Monkey::ShowDestination()
{
	if (m_IsAttackPriority == true)
		SetDestColor("Red");
	else
		SetDestColor("Green");

	p_DestMesh->SetActive(true);
	p_DestMesh->Get3DObject()->SetActive(true);

	p_DestTransform->SetWorldPosition(p_DestTransform->GetWorldPosition());
}

void Monkey::HideDestination()
{
	p_DestMesh->SetActive(false);
	p_DestMesh->Get3DObject()->SetActive(false);
}
