#pragma once

#define CARD_ZINDEX UI_ZINDEX + 10.0f

// Parameters for the UICard, change it to fit with your card design/wishes

#define UICARD_FOLLOWING_SCALE 0.25f
#define UICARD_HOVER_SCALE 1.25f
#define UICARD_STATS_SCALE 0.5f

#define UICARD_COST_OFFSET_Y 20.0f

#define UICARD_NAME_POS_X -100 
#define UICARD_NAME_POS_Y -200

#define UICARD_TIER_POS_X 50
#define UICARD_TIER_POS_Y -200

#define UICARD_COST_POS_X -100
#define UICARD_COST_POS_Y 0

class UIHand;
class UIDeck;
class DeckManager;

class UICard : public Button
{
protected:

	HUD* p_Name = nullptr;
	HUD* p_Cost = nullptr;
	HUD* p_Tier = nullptr;

	UIHand* p_UIHand = nullptr;

	std::function<void(Button*, XMFLOAT3)> m_OnPose = nullptr;
	float m_ScalesFollowing[2];
	float m_ScalesHover[2];

	float m_HandLimitY = 0.0f;

	int m_CardID = 0;

	bool m_IsFollowing = false;
	bool m_IsHoverLastFrame = false;
	bool m_IsActivated = true;

	virtual void Update(float _dt, XMINT2 _MousePos);
	virtual void InitTextsWithTransform();
	virtual void OnPose();

	void SetScaleEffect(float _Scale);
	void HandleHoverEffect();

	UICard() = default;
	virtual ~UICard();

	virtual void Init(std::string _TextureName, std::function<void(HUD*)> m_OnUpdate);

	std::pair<int, int> SavedPos;

public:

	void SetScreenPosition(int _x, int _y) override;

	std::wstring GetName() { return p_Name->GetSprite()->GetTextWstr(); }
	int GetCardID() { return m_CardID; }
	
	virtual void SetZIndex(int _z) override;
	virtual void SetScale(float _Scale) override;
	virtual void SetActive(bool _IsActive);
	virtual void ResetScaleEffect() { SetScaleEffect(m_ScalesFollowing[0]); }
	virtual void SetActiveInHand(bool _IsActivated) { m_IsActivated = _IsActivated; }
	virtual void SetHandLimitY(float _HandLimitY) { m_HandLimitY = _HandLimitY; }
	virtual void SetOnPose(std::function<void(Button*, XMFLOAT3)> _OnPose) { m_OnPose = _OnPose; }
	// Name displayed on the card
	void SetCardName(std::wstring _Name) { p_Name->SetText(_Name); }
	// Cost of the card in 1.Bananium, 2.Concrete, 3.Steel
	void SetCardCost(XMINT3 _Cost);
	void SetCardTier(int _Tier);
	void SetCardID(int _ID) { m_CardID = _ID; }

	//Functions that don't make sense for a card, logic has been removed 

	//virtual void SetRect(int _x, int _y, int _sizeX, int _sizeY) override;
	virtual void SetText(std::wstring _Text) override;

	virtual void Hide();
	virtual void Show();

	friend class UIManager;
	friend class UIHand;
};

class UICardDeck : public UICard
{
public:

	void Update(float _dt, XMINT2 _MousePos);

	UIDeck* p_UIDeck = nullptr;

	friend class UIDeck;
};


#define UI_GROUP_LIFE_UPDATE_TIMER 0.25f

class UILivingEntityInfos : public Button
{
	std::vector<Button*> m_LifeSprites;
	std::vector<LivingEntity*> m_LivingEntities;

	HUD* p_NameHUD = nullptr;

	float m_Timer = 0.0f;

	void Init(std::string _TextureName, std::function<void(HUD*)> _OnUpdate = nullptr) override;

	void IncreaseLifeSprites();
	void DecreaseLifeSprites();

	virtual void Update(float _dt, XMINT2 _MousePos);
	void UpdateShownUI();

public:
	UILivingEntityInfos() : Button() {};

	int GetLivingEntityCount() { return m_LivingEntities.size(); }

	void Show();
	void Hide();

	void ShowEntityHP(LivingEntity* _pLE);
	void ClearEntitiesShown();
	void RemoveEntityShown(LivingEntity* _pLE);

	friend class UIManager;
};