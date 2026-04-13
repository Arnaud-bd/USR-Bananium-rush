#pragma once

class UIManager
{
	std::vector<HUD*> m_HUDs;
	std::vector<Button*> m_Buttons;

	bool m_IsDeletingHUD = false;
	std::vector<HUD*> m_HUDsToRemove;
	std::vector<HUD*> m_HUDsToRemoveTmp;



public:
	UIManager();
	void Update();

	template <typename T>
	inline T* CreateHUD(std::string _TextureName, std::function<void(HUD*)> _OnUpdate = nullptr);

	template <typename T>
	inline T* CreateButton(std::string _TextureName, std::function<void(HUD*)> _OnUpdate = nullptr);

	void AddButton(Button* _Button) { m_Buttons.push_back(_Button); };

	void RemoveButton(Button* _Button);
	void RemoveHUD(HUD* _hud);

};

#include "UIManager.inl"
