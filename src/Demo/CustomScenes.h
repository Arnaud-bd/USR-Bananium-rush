#pragma once

// -- GAMEPLAY -- 
class SceneGameplay : public Scene
{
public:
	GameManager* p_GM = nullptr;

	UIManager* p_UIManager = nullptr;
	EntityManager* p_EntityManager = nullptr;

	virtual void OnInit() override;

	virtual void OnUpdate() override;
};

// -------------------------------------------

// -- MENU -- 
class SceneMenu: public SceneGameplay
{
	int m_TextID;

	std::pair<float, float> m_ScalingBorn = { 1,2 };
	float m_ScalingValue = m_ScalingBorn.first;
	bool Scaling = false;
	float m_IncreaseSpeed = 0.2f;

public:

	void OnInit() override;

	void OnUpdate() override;
};

// -- WORLD -- 
class SceneWorld : public SceneGameplay
{
public:
	Player* p_Player = nullptr;

	// - EFFECT -
	BloodManager* p_BloodManager = nullptr;

	// - UI -
	int m_InventoryUI_ID;
	int m_NexusEnemyUI_ID;
	int m_UIWaveInfo;
	UIHand* p_UIHand;
	UILivingEntityInfos* p_UIEntityInfo;

	// - TEST -
	Monkey* m_Monkey1; //HERE Test to remove 

	bool m_Cinematic = true;
	CameraComponent* m_CinematicCamera;
	TransformComponent* m_CinematicCameraTrs;
	float m_CinematicSpeed = 5.f;

	// - MAP DEBUG -
	int m_GroundID;
	std::pair<float, float> m_WorldSize = { 250.f, 250.f };

	Nexus* m_Allie_Nexus;
	Nexus* m_Enemy_Nexus;

	std::vector<LivingEntity*> m_Deposit;
	int m_NumberOfIronSpot = 5;
	int m_NumberOfRockSpot = 10;
	int m_NumberOfBananiumSpot = 2;

	void SpawnOre(float finalX, float finalZ, OreType type, XMFLOAT4 color);

	// - MAP DEBUG -
	virtual void OnInit() override;
	virtual void OnUpdate() override;

	void UpdateUI();

	XMFLOAT3 m_Destination;
	void Goto(XMFLOAT3 _Destination);
};

// -- END -- 
class SceneEnd : public SceneGameplay
{
	int m_TextID;
	bool m_TextSetup = false;
public:
	void OnInit() override;

	void OnUpdate() override;
};

