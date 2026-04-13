#pragma once

class Group;

enum AttackAnimationState
{
	OnWay,
	GoBack,
	Start
};

class Monkey : public LivingEntity 
{
protected:

	Building* p_NearestBuilding = nullptr;

	Group* p_Group = nullptr;
	VelocityComponent* p_Velocity = nullptr;

	// -- Destionation Variable & Animation --
	XMFLOAT3 m_Destination = { 0, 0, 0 };
	TransformComponent* p_DestTransform = nullptr;
	MeshComponent* p_DestMesh = nullptr;

	std::map < std::string, XMFLOAT4> m_DestColorList;

	void SetDestColor(std::string _name);

	float m_DestAnim_Increment = 1.f;
	float m_DestAnim_RotSpeed = 3.f;
	int m_DestAnim_HighAltitud = 5;
	int m_DestAnim_LowAltitud = 2;
	bool m_DestAnim_StateUping = true;
	int m_DestEntity = -1;

	void Animation_DestEntity();

	// -- Movement Variable & Animation --
	float m_MoveSpeed = 1.0f;
	float m_MoveAnim_Increment = 0.9f;
	float m_MoveAnim_HighScale = 1.5f;
	float m_MoveAnim_LowScale = 1.f;
	bool m_MoveAnim_StateScaling = true;

	// -- Attack Variable & Animation --
	bool m_IsAttackPriority = false;
	bool m_AttackAnimationRunning = false;
	float m_AttackAnim_Increment = 0.9f;

	void Animation_Attack();

	XMFLOAT3 m_SavedStartPos;
	XMFLOAT3 m_SavedAttackDestPos;
	void DashTo(XMFLOAT3 _Destination);

	AttackAnimationState m_AnimState = AttackAnimationState::Start;

	// -- OTHER --
	virtual void InitStateMachine() override;

	virtual void OnUpdate(float _dt) override;
	virtual void OnDeath() override;
	virtual void OnClick() override;

public:

	int m_LastState = 0;

	Monkey();

	Group* GetGroup() { return p_Group; };
	XMFLOAT3 GetDestination() { return m_Destination; }
	Building* GetNearestBuilding() { return p_NearestBuilding; }
	float GetSpeed() { return m_MoveSpeed; }
	bool IsAttackPriority() { return m_IsAttackPriority; }

	void SetGroup(Group* _pGroup) { p_Group = _pGroup; }
	void SetSpeed(float _speed) { m_MoveSpeed = _speed;}
	void SetComponentSpeed(float _speed) { p_Velocity->SetVelocity(_speed);}
	void SetNearestBuilding(Building* _pBuilding) { p_NearestBuilding = _pBuilding; }

	// Update m_Destination and GoTo()
	void SetDestination(XMFLOAT3 _dest);
	void SetAttackPriority(bool _priority) { m_IsAttackPriority = _priority; }

	void Animation_Moving();
	void Play_Attack();

	void StopMoving();

	void Move(float _distance) { p_Transform->Move(_distance); }
	void Translate(float _x, float _y, float _z) { Translate({ _x, _y, _z }); }
	void Translate(const XMFLOAT3& _Translation) { p_Transform->TranslateWorld(_Translation); }
	void GoTo(float _x, float _y, float _z) { GoTo({ _x, _y, _z }); }
	void GoTo(XMFLOAT3 _Destination);
	void ShowDestination();
	void HideDestination();
};