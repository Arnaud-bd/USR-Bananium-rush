 #pragma once

class Player
{
	int m_ID;

	CameraComponent* p_Camera;

	PlayerMovements* p_Movements;

	XMINT3 m_Inventory = { 50,50,50};

public:
	// TO DO 
	TransformComponent* p_CamTransform;

	Player();

	void MoveCamera(XMFLOAT3 _translation);
	void Scroll(float _delta);
	void Update();

	XMINT3 GetInventory() { return m_Inventory; };
	CameraComponent* GetCamera() { return p_Camera; };
	bool HaveEnoughRessource(XMINT3 _cost);

	void SubstractToInventory(XMINT3 _cost);
	void AddToInventory(XMINT3 _paid);

	friend class GameManager;
};

