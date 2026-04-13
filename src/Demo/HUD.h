#pragma once

class UIManager;

class HUD
{
protected:

	std::function<void(HUD*)> m_OnUpdate = nullptr;

	TransformComponent* p_Transform = nullptr;
	SpriteComponent* p_Sprite = nullptr;

	UIManager* p_UIManager = nullptr;

	int m_Entity = -1;

	HUD() = default;

	virtual void Init(std::string _TextureName, std::function<void(HUD*)> _OnUpdate = nullptr);

	virtual void Update(float _dt, XMINT2 _MousePos);

public:

	virtual ~HUD();

	virtual XMINT2 GetSize();
	virtual XMINT2 GetScreenPosition();
	virtual TransformComponent* GetTransform() { return p_Transform; }
	virtual SpriteComponent* GetSprite() { return p_Sprite; }
	virtual int GetEntity() { return m_Entity; }
	std::wstring GetText() { return p_Sprite->GetTextWstr(); }

	virtual void SetActive(bool _IsActive);
	virtual void SetTexture(std::string _TextureName);
	virtual void SetText(std::wstring _Text);
	virtual void SetText(std::string _Text) { SetText(std::wstring(_Text.begin(), _Text.end())); }
	virtual void SetScreenPosition(int _x, int _y);
	virtual void SetZIndex(int _z);
	virtual void SetScale(float _scale);
	virtual void SetOnUpdate(std::function<void(HUD*)> _OnUpdate) { m_OnUpdate = _OnUpdate; }
	void SetUIManager(UIManager* _UIManager) { p_UIManager = _UIManager; }

	virtual void Hide() { p_Sprite->Hide(); }
	virtual void Show() { p_Sprite->Show(); }

	friend class UIManager;
};

