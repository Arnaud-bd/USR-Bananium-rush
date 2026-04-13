#include "pch.h"

// - Init  -
DeckManager::DeckManager()
{
	p_Instance = this;

	// SETUP CARD FOR ALL GAME
	m_CardManager = new CardManager();
	m_CardManager->LoadCard();

	// SETUP MAP CARD SET
	m_Deck = new Deck();
	std::vector<int>* deck = m_Deck->GetDeck();
	MapCardSet.push_back(deck); // DECK

	m_PoolCards.clear();
	MapCardSet.push_back(&m_PoolCards); // POOL

	m_HandCards.clear();
	MapCardSet.push_back(&m_HandCards); // HAND
}

void DeckManager::SetUIHand(UIHand* _pUIHand)
{
	p_UIHand = _pUIHand;
}

// - Modifier  -
void DeckManager::AddCardTo(CardSet _cardset, int _cardID)
{
	MapCardSet[(int)_cardset]->push_back(_cardID);
}

void DeckManager::RemoveCardTo(CardSet _cardset, int _cardID)
{
	auto cardsList = MapCardSet[(int)_cardset];

	for (int i = 0 ; i < cardsList->size(); ++i)
	{
		if (cardsList->at(i) == _cardID)
		{
			cardsList->erase(cardsList->begin() + i);
			return;
		}
	}
}

int  DeckManager::RandomPickIn(CardSet _cardset)
{
	auto cardsList = MapCardSet[(int)_cardset];
	int lastIndex = cardsList->size() - 1;
	int rdmIndex = rand() % lastIndex;
	int cardID = cardsList->at(rdmIndex);
	return cardID;
}

void DeckManager::PlaceCardInUIHand(int _cardID, int _indexInHand)
{
	Card* card = m_CardManager->GetCard(_cardID);
	p_UIHand->SetCardInHand(card, _indexInHand);
}

// - Playable - 
void DeckManager::BuildHand()
{
	for (int i = 1; i < HAND_COMPLETE_SIZE; ++i)
	{
		int ID = RandomPickIn(CardSet::POOL);
		AddCardTo(CardSet::HAND, ID);
		RemoveCardTo(CardSet::POOL, ID);

		//Place new card in UIHand
		PlaceCardInUIHand(ID, i);
	}
	m_NextCard = RandomPickIn(CardSet::POOL);
	PlaceCardInUIHand(m_NextCard, 0);
}


void DeckManager::InitRound()
{
	std::vector<int>* deck = m_Deck->GetDeck();

	m_PoolCards = *deck;

	m_HandCards.clear();

	BuildHand();
}

void DeckManager::PlayedCard(int _cardID, int _CardIndexInHand)
{
	Card* _Card = m_CardManager->GetCard(_cardID);
	if (_Card->GetLingering())
	{
		// HERE IMPLEMENTE WAITING IN HAND 
	}
	else 
	{
		RemoveCardTo(CardSet::HAND, _cardID);		//remove played card
		AddCardTo(CardSet::POOL, _cardID);			//Add removed card
		AddCardTo(CardSet::HAND, m_NextCard);		//Add new card 

		//Place new card in UIHand
		PlaceCardInUIHand(m_NextCard, _CardIndexInHand);

		//Place next card in UIHand
		m_NextCard = RandomPickIn(CardSet::POOL);
		RemoveCardTo(CardSet::POOL, m_NextCard);
		PlaceCardInUIHand(m_NextCard, 0);
	}
}


