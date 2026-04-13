#pragma once

class Button : public HUD
{
protected:
	std::function<void(Button*)> m_OnHover = nullptr;
	std::function<void(Button*)> m_OnClick = nullptr;

	Button() = default;
	
	virtual void Init(std::string _TextureName, std::function<void(HUD*)> _OnUpdate = nullptr, std::function<void(Button*)> _OnHover = nullptr, std::function<void(Button*)> _OnClick = nullptr);

	virtual void Update(float _dt, XMINT2 _MousePos);

public:

	virtual bool IsHovered() { return p_Sprite->IsHovered(); }
	virtual bool IsClicked() { return p_Sprite->IsClicked(); }

	virtual void SetOnHover(std::function<void(Button*)> _OnHover) { m_OnHover = _OnHover; }
	virtual void SetOnClick(std::function<void(Button*)> _Onclick) { m_OnClick = _Onclick; }

	friend class UIManager;
};

