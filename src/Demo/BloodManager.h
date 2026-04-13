#pragma once

#define MATERIAL_BLOOD_NAME "BloodMat"
#define TEXTURE_BLOOD_NAME "BloodTex"

#define BLOOD_SPAWN_TIME 0.25f
#define BLOOD_DISPAWN_TIME 15.f

struct Blood
{
	TransformComponent* p_Transform = nullptr;
	EmitterComponent* p_Emitter = nullptr;

	float m_Duration = 20.0f;
	float m_Timer = 0.0f;
	float m_ScaleFactor = 1.0f;

	int m_Entities[3] = { -1, -1, -1 };
	int m_Entity = -1;

	bool m_IsActive = false;
	bool m_HasSpawned = false;

	void ShowBlood();
	void HandleSpawn();
	void HandleDispawn();

	Blood();

	void Update(float _dt);
	void SpawnBlood(XMFLOAT3 _position, float _duration, float _scale);
};

DECLARE_SCRIPT(BloodManager)

std::vector<Blood*> m_Bloods;

float m_BloodDuration = 20.0f;

public :

	virtual void OnStart() override;
	virtual void OnUpdate() override;

	void SetBloodDuration(float _duration) { m_BloodDuration = _duration; }
	void CreateBlood(XMFLOAT3 _position, float _scale = 2.0f);

END_SCRIPT