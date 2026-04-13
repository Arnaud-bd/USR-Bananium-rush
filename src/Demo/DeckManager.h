#pragma once

#define HAND_SIZE 4
#define HAND_COMPLETE_SIZE 5

class UIHand;
class UIDeck;

enum class CardSet
{
	DECK,
	POOL,
	HAND
};

class DeckManager
{
	std::vector< std::vector<int>* > MapCardSet;

	inline static DeckManager* p_Instance;

	CardManager* m_CardManager; // To avoid multiple Get()
	UIHand* p_UIHand;
	UIDeck* p_UIDeck;

	// DECKS STORAGE
	Deck* m_Deck;

	// CARDS STOCKAGE
	std::vector<int> m_HandCards;	// Set of cards player that can play
	std::vector<int> m_PoolCards;	// Set of cards that can be pick
	int m_NextCard;					// Next Card to go in hand

	// UTILS
	int RandomPickIn(CardSet _cardset);
	void BuildHand();
	void PlaceCardInUIHand(int _cardID, int _indexInHand);

	void AddCardInUIDeck(int _cardID);

public:
	// - Getter  -
	static DeckManager* Get() { return p_Instance; }

	// - Init  -
	DeckManager();
	void SetUIHand(UIHand* _pUIHand);

	// - Modifier  -
	void AddCardTo(CardSet _cardset, int _cardID);
	void RemoveCardTo(CardSet _cardset, int _cardID);

	// - Playable - 
	void InitRound();
	void PlayedCard(int _cardID, int _CardIndexInHand);

	CardManager* GetCardManager() { return m_CardManager; };
};

