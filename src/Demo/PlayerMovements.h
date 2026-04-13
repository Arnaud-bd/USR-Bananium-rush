#pragma once

class Player;

class PlayerMovements
{
	Player* p_Player;

	// - Drag Movement -
	XMINT2 MousePos_Start = { 0,0 };
	XMINT2 MousePos_End = { 0,0 };

	float m_speed = 100.0f;

	// - Cam Limit - 
	XMINT2 MovementLimit = { -250,250 };
	
public:
	bool MouseDrag = false;

	PlayerMovements(Player* _player);

	void Update();

	void HandleInputs();
};

