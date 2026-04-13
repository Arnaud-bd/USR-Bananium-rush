#include "pch.h"

void LivingEntity::InitStateMachine()
{
	p_StateMachine = new StateMachineLivingEntity();
	p_StateMachine->Initialize(this, (int)LivingEntityStates::COUNT);
	//Dead
	ActionLivingEntity_Dead* pDead = p_StateMachine->CreateAction<ActionLivingEntity_Dead>((int)LivingEntityStates::DEAD);
	//Idle
	ActionLivingEntity_Idle* pIdle = p_StateMachine->CreateAction<ActionLivingEntity_Idle>((int)LivingEntityStates::IDLE);
	{	//-> Dead
		auto transition = pIdle->CreateTransition((int)LivingEntityStates::DEAD);
		transition->AddCondition<ConditionLivingEntity_IsDead>();
	}
	{	//-> Attack
		auto transition = pIdle->CreateTransition((int)LivingEntityStates::ATTACK);
		transition->AddCondition<ConditionLivingEntity_TargetInRange>();
	}
	//Attack
	ActionLivingEntity_Attack* pAttack = p_StateMachine->CreateAction<ActionLivingEntity_Attack>((int)LivingEntityStates::ATTACK);
	{	//-> Dead
		auto transition = pAttack->CreateTransition((int)LivingEntityStates::DEAD);
		transition->AddCondition<ConditionLivingEntity_IsDead>();
	}
	{	//-> Idle
		auto transition = pAttack->CreateTransition((int)LivingEntityStates::IDLE);
		transition->AddCondition<ConditionLivingEntity_TargetInRange>(false);
	}
	p_StateMachine->SetState((int)LivingEntityStates::IDLE);
}

void LivingEntity::Update(float _dt)
{
	p_StateMachine->Update();

	if (IsDead())
		return;

	if (InputsManager::GetButtonDown(Mouse::LEFT))
	{
		if (m_IsSelected)
			m_IsSelected = false;
	}

	if (m_IsDamaged == true)
	{
		if (m_FlashTimer >= m_FlahsDuring)
		{
			m_FlashTimer = 0;
			p_Mesh->SetColor(m_BaseColor);
			m_IsDamaged = false;
		}
		else 
		{
			m_FlashTimer += ECS::Get().GetDeltaTime();
		}
	}

	OnUpdate(_dt);
}

void LivingEntity::OnDamage(int _damage)
{
	m_IsDamaged = true;
	p_Mesh->SetColor(m_DamagedColor);
}

LivingEntity::LivingEntity()
{
	ECS& ecs = ECS::Get();

	m_Entity = ecs.CreateEntity();
	p_Transform = ecs.AddComponent<TransformComponent>(m_Entity);
	p_Collider = ecs.AddComponent<ColliderComponent>(m_Entity);
	p_Collider->SetType<ColliderAABB>();
	p_Collider->AddToLayer(SELECTION_LAYER);
	p_Collider->AddToLayer(COMBAT_COLLIDERS_LAYER);
	p_Mesh = ecs.AddComponent<MeshComponent>(m_Entity);
	p_Mesh->SetGeometry(GeometryManager::GetGeometry(GEO_TYPE::BOX_UV));

	m_RangeEntity = ecs.CreateEntity();
	p_RangeTransform = ecs.AddComponent<TransformComponent>(m_RangeEntity);
	p_RangeTransform->SetLocalPosition({ 0.0f, -p_Transform->GetScale().y * 0.5f + 0.01f, 0.0f });
	p_RangeMesh = ecs.AddComponent<MeshComponent>(m_RangeEntity);
	p_RangeCollider = ecs.AddComponent<ColliderComponent>(m_RangeEntity);
	p_RangeCollider->SetType<ColliderAABB>(); //HERE Warning Collider AABB on Sphere
	p_RangeCollider->AddToLayer(COMBAT_COLLIDERS_LAYER);
	p_RangeMesh->SetGeometry(GeometryManager::GetGeometry(GEO_TYPE::SPHERE));
	p_RangeMesh->SetColor({ 1.0f, 0.0f, 0.0f, 0.15f });
	p_RangeMesh->SetMaterial(MATERIAL_DEFAULT_COLOR_A_NAME);
	HideRange();

	ecs.GetEntity(m_Entity)->AddChild(m_RangeEntity);

	InitStateMachine();
}

void LivingEntity::SetTeam(std::string _team)
{
	p_Collider->SetTag(_team); 
}

void LivingEntity::Destroy()
{
	ECS::Get().RemoveEntity(m_Entity);
	ECS::Get().RemoveEntity(m_RangeEntity);
	delete(p_StateMachine);
	OnDestroy();
}

bool LivingEntity::TryAttack()
{
	float dt = ECS::GetDeltaTime();
	LivingEntity* target = p_EntityManager->GetLivingEntity(m_AttackTarget);
	m_AttackTimer += dt * m_AttackSpeed;
	if (m_AttackTimer < 1.0f)
		return false;
	m_AttackTimer = 0.0f;

	if (target == nullptr)
		return false;

	target->DealDamage(m_AttackDamage);
	return true;
}

bool LivingEntity::IsInRange(XMINT2 _mousePos)
{
	XMFLOAT3 scale = p_RangeTransform->GetScale();
	XMFLOAT3 pos = p_RangeTransform->GetWorldPosition();

	float radius = scale.x * 0.5f;

	float dist = sqrt(pow(abs(_mousePos.x - pos.x), 2) + pow(abs(_mousePos.y - pos.y), 2));

	if (dist < radius)
		return true;

	return false;
}

bool LivingEntity::IsCurrentTargetReachable()
{
	ECS& ecs = ECS::Get();

	if (m_AttackTarget == -1)
		return false;

	LivingEntity* LE = p_EntityManager->GetLivingEntity(m_AttackTarget);
	if (LE == nullptr)
		return false;
	if (LE->IsDead())
		return false;

	return p_RangeCollider->IsCollidingWith(m_AttackTarget);
}

int LivingEntity::GetStateMachineState()
{
	return p_StateMachine->GetCurrentState();
}

void LivingEntity::TryChooseTarget()
{
	auto allColliders = p_RangeCollider->GetColliding();
	for (ColliderComponent* other : allColliders)
	{
		for (std::string enemyTeam : m_Enemies)
		{
			if (other->GetTag() != enemyTeam)
				continue;

			int colliderEntityID = other->GetID();

			LivingEntity* LE = p_EntityManager->GetLivingEntity(colliderEntityID);
			if (LE == nullptr)
				continue;
			if (LE->IsDead())
				continue;

			m_AttackTarget = colliderEntityID;
			return;
		}
	}
}

void LivingEntity::Death()
{
	m_IsDead = true;
	p_Mesh->Get3DObject()->SetActive(false);
	HideRange();
	OnDeath();
}

void LivingEntity::LookTo(const XMFLOAT3& _Target)
{
	XMFLOAT3 position = p_Transform->GetWorldPosition();
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR dest = XMLoadFloat3(&_Target);
	XMVECTOR dir = dest - pos;
	dir = XMVector3Normalize(dir);
	LookTo(dir);
}

void LivingEntity::LookTo(XMVECTOR _Direction)
{
	float yaw = atan2f(
		XMVectorGetX(_Direction),
		XMVectorGetZ(_Direction)
	);
	float pitch = asinf(
		-XMVectorGetY(_Direction)
	);

	p_Transform->SetRotation(yaw, pitch, 0);
}

void LivingEntity::Click()
{
	m_IsSelected = true;
	OnClick();
}

void LivingEntity::Kill()
{
	Death();
}

void LivingEntity::DealDamage(int _damage)
{
	if(m_Health <= 0)
		return;
	m_Health -= _damage;
	if(m_Health <= 0)
		Death();
	OnDamage(_damage);
}

void LivingEntity::GetHealed(int _heal)
{
	if (m_Health <= 0)
		return;

	m_Health += _heal;

	if (m_Health > m_MaxHealth)
		m_Health = m_MaxHealth;
}

void LivingEntity::ShowRange()
{
	p_RangeMesh->SetActive(true);
	p_RangeMesh->Get3DObject()->SetActive(true);

	p_RangeTransform->SetWorldPosition(p_RangeTransform->GetWorldPosition()); 
}

void LivingEntity::HideRange()
{
	p_RangeMesh->SetActive(false);
	p_RangeMesh->Get3DObject()->SetActive(false);
}
