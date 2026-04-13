#include "pch.h"

void HUD::Init(std::string _TextureName, std::function<void(HUD*)> _OnUpdate)
{
	ECS& ecs = ECS::Get();

	m_Entity = ecs.CreateEntity();
	p_Transform = ecs.AddComponent<TransformComponent>(m_Entity);
	p_Sprite = ecs.AddComponent<SpriteComponent>(m_Entity);

	p_Sprite->SetTexture(_TextureName);

	m_OnUpdate = _OnUpdate;
}

void HUD::Update(float _dt, XMINT2 _MousePos)
{
	if(p_Sprite->IsActive() && m_OnUpdate)
		m_OnUpdate(this);
}

///////////////
/// PUBLIC
///////////////

HUD::~HUD()
{
	ECS& ecs = ECS::Get();
	ecs.RemoveEntity(m_Entity);
	m_Entity = -1;
}

void HUD::SetActive(bool _IsActive)
{
	if(p_Sprite)
		p_Sprite->SetActive(_IsActive);
}

void HUD::SetTexture(std::string _TextureName)
{
	p_Sprite->SetTexture(_TextureName);
}

void HUD::SetText(std::wstring _Text)
{
	if (p_Sprite)
		p_Sprite->SetText(_Text);
}

void HUD::SetScreenPosition(int _x, int _y)
{
	XMFLOAT3 worldPos = ScreenToWorld({ _x, _y }, Device::GetWindow()->Size()); 
	p_Transform->SetWorldPosition(worldPos);
}

void HUD::SetZIndex(int _z)
{
	p_Sprite->SetZIndex(_z);
}

void HUD::SetScale(float _scale)
{
	p_Transform->SetScale(_scale);
}

XMINT2 HUD::GetSize()
{
	if(p_Sprite == nullptr)
		return { 0, 0 };
	return p_Sprite->GetSize();
}

XMINT2 HUD::GetScreenPosition()
{
	XMFLOAT3 worldPos = p_Transform->GetWorldPosition();
	return WorldToScreen(worldPos, Device::GetWindow()->Size());
}
