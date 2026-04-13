#include "pch.h"

void Button::Init(std::string _TextureName, std::function<void(HUD*)> _OnUpdate, std::function<void(Button*)> _OnHover, std::function<void(Button*)> _OnClick)
{
	HUD::Init(_TextureName, _OnUpdate);
	m_OnHover = _OnHover;
	m_OnClick = _OnClick;
}

void Button::Update(float _dt, XMINT2 _MousePos)
{
	if (p_Sprite->IsActive() == false)
		return;

	HUD::Update(_dt, _MousePos);

	if (p_Sprite->IsHovered() && m_OnHover)
		m_OnHover(this);
	if (p_Sprite->IsClicked() && m_OnClick)
		m_OnClick(this);
}

///////////////
/// PUBLIC
///////////////
