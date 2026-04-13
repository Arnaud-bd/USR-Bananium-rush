#pragma once
class UIDeck
{
	std::vector<UICard*> m_UICards;
	UIManager* p_UIManager = nullptr;

public:
	void Init(UIManager* _pUIManager);
	void PlacementScoller();

	UICardDeck* CreateCard(Card* _Card);
};

