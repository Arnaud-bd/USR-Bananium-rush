#include "pch.h"

PlayerMovements::PlayerMovements(Player* _player)
{
	p_Player = _player;
}

void PlayerMovements::Update()
{
	ECS* ecs = &ECS::Get();
	XMFLOAT3 Pos = p_Player->p_CamTransform->GetWorldPosition();
}

void PlayerMovements::HandleInputs()
{
	ECS* ecs = &ECS::Get();

	float dt = ecs->GetDeltaTime();

	if (InputsManager::GetKey(Keyboard::D))
	{
		p_Player->MoveCamera({ 50.0f * dt, 0.0f, 50.0f * dt });
	}
	if (InputsManager::GetKey(Keyboard::Q))
	{
		p_Player->MoveCamera({ -50.0f * dt, 0.0f, -50.0f * dt });
	}
	if (InputsManager::GetKey(Keyboard::Z))
	{
		p_Player->MoveCamera({ -50.0f * dt, 0.0f, 50.0f * dt });
	}
	if (InputsManager::GetKey(Keyboard::S))
	{
		p_Player->MoveCamera({ 50.0f * dt, 0.0f, -50.0f * dt });
	}
	if (InputsManager::GetMouseWheelUp() || InputsManager::GetMouseWheelDown())
	{
		float delta = InputsManager::GetMouseWheelDelta() * dt;
		p_Player->Scroll(delta);
	}

	if (InputsManager::GetButtonDown(Mouse::MIDDLE))
	{
		XMINT2 mousePos = InputsManager::GetMousePosition();
		MousePos_Start = mousePos;
		MouseDrag = true;
	}

	if (InputsManager::GetButton(Mouse::MIDDLE))
	{
		XMINT2 mousePos = InputsManager::GetMousePosition();

		XMINT2 VecteurDirecteur;
		VecteurDirecteur.x = mousePos.x - MousePos_Start.x;
		VecteurDirecteur.y = mousePos.y - MousePos_Start.y;

		XMFLOAT3 right = p_Player->p_CamTransform->GetRight();
		XMFLOAT3 forward = p_Player->p_CamTransform->GetForward();

		right.y = 0.0f;
		forward.y = 0.0f;

		XMVECTOR r = XMVector3Normalize(XMLoadFloat3(&right));
		XMVECTOR f = XMVector3Normalize(XMLoadFloat3(&forward));

		XMVECTOR move = -(r * (VecteurDirecteur.x * m_speed * dt) - f * (VecteurDirecteur.y * m_speed * dt));

		XMFLOAT3 finalMove;
		XMStoreFloat3(&finalMove, move);

		p_Player->MoveCamera(finalMove);

		MousePos_Start = mousePos;
	}

	if (InputsManager::GetButtonDown(Mouse::LEFT))
	{
		XMINT2 mousePos = InputsManager::GetMousePosition();
		
		std::string layer = SELECTION_LAYER;

		SceneGameplay* sc = SceneManager::Get()->GetActiveScene<SceneGameplay>();
		SceneWorld* scWorld = dynamic_cast<SceneWorld*>(sc);
		if (scWorld)
		{
			scWorld->p_UIEntityInfo->ClearEntitiesShown();
			if (scWorld->p_UIHand->HasCardSelected())
				layer = GROUND_LAYER;
		}

		Hit hit = RayCast::CastRay(mousePos, layer);

		if (hit.m_CollidingID == -1)
			return;
			
		LivingEntity* entity = sc->p_EntityManager->GetLivingEntity(hit.m_CollidingID);
		if (entity == nullptr)
			return;
		if(scWorld != nullptr)
		{
			if (entity->GetTeam() == TEAM_ARNO)
			{
				Monkey* monk = dynamic_cast<Monkey*>(entity);
				if (monk == nullptr)
				{
					scWorld->p_UIEntityInfo->ShowEntityHP(entity);
					return;
				}
				Group* group = monk->GetGroup();
				if(group == nullptr)
				{
					scWorld->p_UIEntityInfo->ShowEntityHP(entity);
					return;
				}
				monk->GetGroup()->ShowHP();
				return;
			}
			else
			{
				entity->Click();
				return;
			}
		}	
	}
}