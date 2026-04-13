#pragma once

class Deck
{
	std::vector<int> m_DeckCards;	// Set of player cards 
	int m_Minion = -1;				// Card allways in player hand 
public:

	// - Getter -
	std::vector<int>* GetDeck() { return &m_DeckCards; };
	int GetMinionsIndex() { return m_Minion; };
};
