#pragma once

class EntityManager;
class StateMachineLivingEntity;

#define TEAM_ZENTIL "ZENTIL"
#define TEAM_ARNO "Arno"

class LivingEntity
{
protected:
	std::vector<std::string> m_Enemies = std::vector<std::string>();

	std::string m_Name = "LivingEntity";

	StateMachineLivingEntity* p_StateMachine = nullptr;

	TransformComponent* p_Transform = nullptr;
	MeshComponent* p_Mesh = nullptr;
	ColliderComponent* p_Collider = nullptr;
	
	TransformComponent* p_RangeTransform = nullptr;
	MeshComponent* p_RangeMesh = nullptr; //Can be activated to show the range of the entity
	ColliderComponent* p_RangeCollider = nullptr; //HERE Warning AABB on Sphere

	EntityManager* p_EntityManager = nullptr;

	float m_AttackSpeed = 1.0f;
	float m_AttackTimer = 0.0f;
	int m_AttackDamage = 1;

	int m_Entity = -1;
	int m_RangeEntity = -1;
	int m_AttackTarget = -1;

	int m_Health = 20;
	int m_MaxHealth = 20;
	bool m_IsDead = false;
	bool m_IsSelected = false;

	// Damage feel color
	bool m_IsDamaged = false;
	float m_FlahsDuring = 0.2f;
	float m_FlashTimer = 0;

	XMFLOAT4 m_BaseColor = { 0.5f, 0.5f, 0.5f, 1.f };
	XMFLOAT4 m_DamagedColor = { 1.0f, 0.5f, 0.5f, 1.0f };

	virtual void InitStateMachine();

	void Update(float _dt);
	virtual void OnUpdate(float _dt) {};	// override this for custom behavior
	virtual void OnClick() {};
	virtual void OnDamage(int _damage);	
	virtual void OnDeath() {};				// override this for custom behavior
	virtual void OnDestroy() {};			// override this for custom behavior

public:
	LivingEntity();

	bool IsInRange(XMINT2 _mousePos);
	bool IsCurrentTargetReachable();
	bool IsDead() { return m_IsDead; }
	bool IsSelected() { return m_IsSelected; }
	int GetEntityID() { return m_Entity; }
	int GetHealth() { return m_Health; }
	int GetMaxHealth() { return m_MaxHealth; }
	int GetStateMachineState();
	float GetRange() { return p_RangeTransform->GetScale().x; }
	XMFLOAT3 GetPosition() { return p_Transform->GetWorldPosition(); }
	std::string GetTeam() { return p_Collider->GetTag(); }
	std::string GetName() { return m_Name; }
	std::vector<std::string> GetEnemies() { return m_Enemies; }
	TransformComponent* GetTransformComponent() { return p_Transform; }
	ColliderComponent* GetColliderComponent() { return p_Collider; }
	ColliderComponent* GetRangeColliderComponent() { return p_RangeCollider; }

	void SetBaseColor(XMFLOAT4 _color) { m_BaseColor = _color; p_Mesh->SetColor(m_BaseColor); }
	void SetRangePos(float x, float y, float z) { p_RangeTransform->SetWorldPosition({ x,y,z }); }
	void SetRangeDiameter(float _range) { p_RangeTransform->SetScale(_range, 0.01f, _range); }
	void SetAttackSpeed(float _attackSpeed) { m_AttackSpeed = _attackSpeed; }
	void SetDamage(int _attackDamage) { m_AttackDamage = _attackDamage; }
	void SetHealth(int _health) { m_Health = _health; }
	void SetMaxHealth(int _maxHealth) { m_MaxHealth = _maxHealth; }
	void SetTeam(std::string _team);
	void SetName(std::string _name) { m_Name = _name; }
	void SetSelected(bool _selected) { m_IsSelected = _selected; }
	void AddEnemy(std::string _enemyTeam) { m_Enemies.push_back(_enemyTeam); }
	void SetPosition(float _x, float _y, float _z) { SetPosition({ _x, _y, _z }); }
	void SetPosition(const XMFLOAT3& _Position) { p_Transform->SetWorldPosition(_Position); }
	void Rotate(const XMFLOAT3& _Rotation) { Rotate(_Rotation.x, _Rotation.y, _Rotation.z); }
	void Rotate(float _yaw, float _pitch, float _roll) { p_Transform->RotateWorld(_yaw, _pitch, _roll); }
	void SetRotation(float _yaw, float _pitch, float _roll) { p_Transform->SetRotation(_yaw, _pitch, _roll); }
	void LookTo(const XMFLOAT3& _Target);
	void LookTo(XMVECTOR _Direction);
	 
	bool TryAttack();
	void TryChooseTarget();
	MeshComponent* GetMesh() { return p_Mesh; };

	void Click();
	void Kill();
	void DealDamage(int _damage);
	void GetHealed(int _heal);
	void ClearEnemies() { m_Enemies.clear(); }
	void ShowRange();
	void HideRange();
	void Death();
	void Destroy();

	friend class EntityManager;
};

