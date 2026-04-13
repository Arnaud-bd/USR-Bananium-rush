#include "pch.h"

void Blood::ShowBlood()
{
	for (int i = 0; i < 3; i++)
	{
		ECS& ecs = ECS::Get();
		TransformComponent* trs = ecs.GetComponent<TransformComponent>(m_Entities[i]);
		trs->SetScale(m_ScaleFactor);
		MeshComponent* mesh = ecs.GetComponent<MeshComponent>(m_Entities[i]);
		XMFLOAT4 color = mesh->Get3DObject()->GetColor();
		color.w = 0.5f;
		mesh->SetColor(color);
	}
}

void Blood::HandleSpawn()
{
	float increase = m_Timer * (1.0f / BLOOD_SPAWN_TIME);
	float scale = increase * m_ScaleFactor;
	float alpha = increase / 2.0f;
	for (int i = 0; i < 3; i++)
	{
		ECS& ecs = ECS::Get();
		TransformComponent* trs = ecs.GetComponent<TransformComponent>(m_Entities[i]);
		trs->SetScale(scale);
		MeshComponent* mesh = ecs.GetComponent<MeshComponent>(m_Entities[i]);
		XMFLOAT4 color = mesh->Get3DObject()->GetColor();
		color.w = alpha;
		mesh->SetColor(color);
	}
}

void Blood::HandleDispawn()
{
	float decrease = (m_Duration - m_Timer) / BLOOD_DISPAWN_TIME;
	float scale = decrease * m_ScaleFactor;
	float alpha = decrease / 2.0f;
	for (int i = 0; i < 3; i++)
	{
		ECS& ecs = ECS::Get();
		TransformComponent* trs = ecs.GetComponent<TransformComponent>(m_Entities[i]);
		trs->SetScale(scale);
		MeshComponent* mesh = ecs.GetComponent<MeshComponent>(m_Entities[i]);
		XMFLOAT4 color = mesh->Get3DObject()->GetColor();
		color.w = alpha;
		mesh->SetColor(color);
	}
}

Blood::Blood()
{
	ECS& ecs = ECS::Get();

	m_Entity = ecs.CreateEntity();
	p_Transform = ecs.AddComponent<TransformComponent>(m_Entity);
	p_Emitter = ecs.AddComponent<EmitterComponent>(m_Entity);
	EmitterSettings settings = EmitterSettings();
	settings.m_EmitterShape = EMITER_SHAPE::CONE;
	settings.m_EmitterMaxPart = 5;
	settings.m_PartDir = { 0, m_ScaleFactor * 0.5f, 0 };
	settings.m_PartGravity = { 0, m_ScaleFactor * -1.5f, 0 };
	settings.m_PartGeoType = GEO_TYPE::SPHERE;
	settings.m_PartStartColor = { 1, 0, 0, 1 };
	settings.m_PartEndColor = { 0.5f, 0, 0, 1 };
	settings.m_PartSize = m_ScaleFactor * 3.0f;
	settings.m_PartSpeed = m_ScaleFactor * 3.0f;
	settings.m_PartLifetime = m_Duration;
	p_Emitter->SetSettings(settings);
	p_Emitter->SetActive(false);

	for(int i = 0; i < 3; i++)
	{
		int nEntity = ecs.CreateEntity();
		m_Entities[i] = nEntity;
		TransformComponent* nTrs = ecs.AddComponent<TransformComponent>(nEntity);

		MeshComponent* nMesh = ecs.AddComponent<MeshComponent>(nEntity);
		nMesh->SetGeometry(GeometryManager::GetGeometry(GEO_TYPE::PLANE));
		nMesh->SetMaterial(MATERIAL_BLOOD_NAME);
		nMesh->SetActive(false);
		nMesh->Get3DObject()->SetActive(false);

		p_Transform->AddChild(nTrs);
	}
}

void Blood::Update(float _dt)
{
	if (m_IsActive == false)
		return;
	m_Timer += _dt;

	if(m_Timer <= BLOOD_SPAWN_TIME)
	{
		HandleSpawn();
		return;
	}
	if(m_HasSpawned == false)
	{
		ShowBlood();
		m_HasSpawned = true;
	}
	if (m_Timer >= (m_Duration - BLOOD_DISPAWN_TIME) && m_Timer < m_Duration)
	{
		HandleDispawn();
		return;
	}

	if (m_Timer < m_Duration)
		return;

	m_Timer = 0.0f;
	m_IsActive = false;
	p_Emitter->SetActive(false);
	p_Emitter->HideAllParticules();
	for (int i = 0; i < 3; i++)
	{
		ECS& ecs = ECS::Get();
		MeshComponent* nMesh = ecs.GetComponent<MeshComponent>(m_Entities[i]);
		nMesh->SetActive(false);
		nMesh->Get3DObject()->SetActive(false);
	}
}
void Blood::SpawnBlood(XMFLOAT3 _position, float _duration, float _scale)
{
	for (int i = 0; i < 3; i++)
	{
		ECS& ecs = ECS::Get();
		TransformComponent* trs = ecs.GetComponent<TransformComponent>(m_Entities[i]);
		float yaw = rand() % 360;
		float x = ((rand() % (int)(_scale * 10)) - (_scale * 5.0f)) / 20.0f;
		float z = ((rand() % (int)(_scale * 10)) - (_scale * 5.0f)) / 20.0f;
		float scale = ((rand() % (int)(_scale * 10)) / 20.0f) + 1.0f;
		yaw = XMConvertToRadians(yaw);
		trs->RotateWorld(yaw, 0.0f, 0.0f);
		trs->TranslateWorld({ x, 0, z });
		trs->SetScale(scale);
		//TODO : translate a bit with rand
		MeshComponent* mesh = ecs.GetComponent<MeshComponent>(m_Entities[i]);
		mesh->SetActive(true);
		mesh->Get3DObject()->SetActive(true);
		mesh->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	}
	p_Transform->SetWorldPosition(_position);
	p_Emitter->SetActive(true);
	EmitterSettings settings = p_Emitter->GetSettings();
	settings.m_PartDir = { 0, m_ScaleFactor * 0.5f, 0 };
	settings.m_PartGravity = { 0, m_ScaleFactor * -1.5f, 0 };
	settings.m_PartSize = m_ScaleFactor * 3.0f;
	settings.m_PartSpeed = m_ScaleFactor * 3.0f;
	settings.m_PartLifetime = m_Duration;
	p_Emitter->SetSettings(settings);
	m_IsActive = true;
	m_Duration = _duration;
	m_ScaleFactor = _scale;
}

void BloodManager::OnStart()
{
	if (TextureManager::GetTextureIndex(TEXTURE_BLOOD_NAME) != -1)
		return;

	TextureManager::LoadTexture(TEXTURE_BLOOD_NAME, L"../../res/Textures/Blood.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_BLOOD_NAME, TEXTURE_BLOOD_NAME);
}

void BloodManager::CreateBlood(XMFLOAT3 _position, float _scale)
{
	for (Blood* blood : m_Bloods)
	{
		if (blood->m_IsActive)
			continue;

		blood->SpawnBlood(_position, m_BloodDuration, _scale);
		return;
	}

	//If no one is available, create a new one
	Blood* blood = new Blood();
	blood->SpawnBlood(_position, m_BloodDuration, _scale);
	m_Bloods.push_back(blood);
}

void BloodManager::OnUpdate()
{
	float dt = ECS::GetDeltaTime();

	for (Blood* blood : m_Bloods)
		blood->Update(dt);
}


