#pragma once

class CardManager
{
	inline static CardManager* p_Instance = nullptr;

	std::vector<Card*> m_SetOfCards; // Set of All cards in Game
public:
	CardManager();

	void LoadCard();

	// - Add -
	void AddCard(Card* _card) { m_SetOfCards.push_back(_card); };
	// - Getter -
	static CardManager* Get() { return p_Instance; }

	int GetSizeOfSet() { return m_SetOfCards.size(); };

	Card* GetCard(int _cardID) { return m_SetOfCards.at(_cardID); };
};
