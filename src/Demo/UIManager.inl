#include "UIManager.h"
#pragma once

	template<typename T>
	inline T* UIManager::CreateHUD(std::string _TextureName, std::function<void(HUD*)> _OnUpdate)
	{
		T* hud = new T();
		hud->p_UIManager = this;
		hud->Init(_TextureName, _OnUpdate);
		m_HUDs.push_back(hud);
		return hud;
	}

	template<typename T>
	inline T* UIManager::CreateButton(std::string _TextureName, std::function<void(HUD*)> _OnUpdate)
	{
		T* button = new T();
		button->p_UIManager = this;
		button->Init(_TextureName, _OnUpdate);
		m_Buttons.push_back(button);
		return button;
	}
