#include "pch.h"

UIManager::UIManager()
{
}

void UIManager::Update()
{
	float dt = ECS::GetDeltaTime();
	XMINT2 mousePos = InputsManager::GetMousePosition();

	for (int i = 0; i < m_HUDs.size(); i++)
		m_HUDs[i]->Update(dt, mousePos);
	for (int i = 0; i < m_Buttons.size(); i++)
		m_Buttons[i]->Update(dt, mousePos);

	m_IsDeletingHUD = true;
	int lastIndex = m_HUDsToRemove.size() - 1;
	while(lastIndex != -1)
	{
		for (int i = lastIndex; i >= 0; i--)
		{
			delete(m_HUDsToRemove[i]);
			m_HUDsToRemove.erase(m_HUDsToRemove.begin() + i);
		}
		
		for (HUD* hud : m_HUDsToRemoveTmp)
			m_HUDsToRemove.push_back(hud);
		m_HUDsToRemoveTmp.clear();

		lastIndex = m_HUDsToRemove.size() - 1;
	}
	m_IsDeletingHUD = false;
}

void UIManager::RemoveButton(Button* _Button)
{
	m_Buttons.erase(std::remove(m_Buttons.begin(), m_Buttons.end(), _Button), m_Buttons.end());
	if(m_IsDeletingHUD)
		m_HUDsToRemoveTmp.push_back(_Button);
	else
		m_HUDsToRemove.push_back(_Button);
}
void UIManager::RemoveHUD(HUD* _hud)
{
	m_HUDs.erase(std::remove(m_HUDs.begin(), m_HUDs.end(), _hud), m_HUDs.end());
	if (m_IsDeletingHUD)
		m_HUDsToRemoveTmp.push_back(_hud);
	else
		m_HUDsToRemove.push_back(_hud);
}
