#include "pch.h"

UIStats::UIStats()
{
	p_String = new UIString();
}

void UIStats::SetPosition(float x, float y)
{
	p_String->SetPosition(x, y);
}
void UIStats::SetText(std::wstring text)
{
	m_Text = text;
	m_IsDirty = true;
}

void UIStats::Update(float dt)
{
	if (m_IsDirty == false)
		return;
	m_Timer += dt;
	if (m_Timer < 0.25f)
		return;

	m_Timer = 0.0f;
	p_String->SetString(m_Text);
	m_IsDirty = false;
}
