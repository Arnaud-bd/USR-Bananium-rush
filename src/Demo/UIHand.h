#pragma once

#define HAND_TEXTURE_PATH L"../../res/Textures/Hand.dds"
#define HAND_TEXTURE_NAME "HandTex"


class UIHand final
{
	XMINT2 m_CardSpacing = { 0, 10 };
	XMINT2 m_CardPlacementStart = { 200, 0 };
	
	std::vector<UICard*> m_CardsInHand = std::vector<UICard*>();
	std::vector<UICard*> m_CardsToClear = std::vector<UICard*>();

	HUD* p_UIHand = nullptr;
	UIManager* p_UIManager = nullptr;

	bool m_HasCardSelected = false;

	void Update() = delete;
	void SetUICardInHand(UICard* _Card, int _IndexInHand);
public:

	UIHand() = default;
	void Init(UIManager* _pUIManager);
	 
	int GetCardIDInHand(UICard* _UICard);
	float GetLimitY();

	bool HasCardSelected() { return m_HasCardSelected; }

	UICard* CreateCard(Card* _Card);

	// Set the UICard in the hand at the given index, if there is already a UICard it will be removed
	void SetCardInHand(Card* _Card, int _IndexInHand);
	void SetCardSpacing(XMINT2 _CardSpacing) { m_CardSpacing = _CardSpacing; }

	void ShowAllAlliesRanges(); //HERE Je trouve ça super moche d'y mettre ici 
	void HideAllAlliesRanges();

	void HideUIHand();
	void ShowUIHand();

	void SetCardSelected(UICard* _Card);
	void ResetCardSelected();
	void ClearDeletedCard();

	void AlignCardsInHand();

	friend class UIManager;
};

