#include "pch.h"

Player::Player()
{
	ECS* ecs = &ECS::Get();
	m_ID = ecs->CreateEntity();

	p_CamTransform = ecs->AddComponent<TransformComponent>(m_ID);
	p_CamTransform->RotateWorld(-Maths::Pi / 4.f, Maths::Pi / 4.f, 0.0f);
	p_CamTransform->SetWorldPosition({ -20.5f, 36.f, -21.6f });

	p_Camera = ecs->AddComponent<CameraComponent>(m_ID);
	p_Camera->SetMainCamera(true);

	p_Movements = new PlayerMovements(this);
}

void Player::MoveCamera(XMFLOAT3 _translation)
{
	p_CamTransform->TranslateWorld(_translation);
}

void Player::Scroll(float _delta)
{
	XMFLOAT3 camPos = p_CamTransform->GetWorldPosition();

	XMFLOAT3 newPos = { camPos.x + 5000.f * -_delta, camPos.y + 5500.f * -_delta, camPos.z + 5000.f * _delta }; 

	if (newPos.y <= 1.f || newPos.y >= 100.f)
	{
		newPos.z = camPos.z;
		newPos.y = camPos.y;
		newPos.x = camPos.x;
	}

	p_CamTransform->SetWorldPosition(newPos);
}

void Player::Update()
{
	p_Movements->Update();
	p_Movements->HandleInputs();
}

bool Player::HaveEnoughRessource(XMINT3 _cost)
{
	if (_cost.x <= m_Inventory.x && _cost.y <= m_Inventory.y && _cost.z <= m_Inventory.z)
		return true;
	return false;
}

void Player::SubstractToInventory(XMINT3 _cost)
{
	m_Inventory.x -= _cost.x;
	m_Inventory.y -= _cost.y;
	m_Inventory.z -= _cost.z;
}

void Player::AddToInventory(XMINT3 _paid)
{
	m_Inventory.x += _paid.x;
	m_Inventory.y += _paid.y;
	m_Inventory.z += _paid.z;
}
