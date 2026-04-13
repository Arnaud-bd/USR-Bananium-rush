#include "pch.h"

void UIHand::Init(UIManager* _pUIManager)
{
	p_UIManager = _pUIManager;
	TextureManager::LoadTexture(HAND_TEXTURE_NAME, HAND_TEXTURE_PATH);
	p_UIHand = p_UIManager->CreateHUD<HUD>(HAND_TEXTURE_NAME);
	int width = Device::GetWindow()->Width();
	int height = Device::GetWindow()->Height();
	p_UIHand->SetScreenPosition(width / 2.0f, height);
	p_UIHand->SetZIndex(UI_ZINDEX - 1);

	//Init vector
	for (int i = 0; i < HAND_COMPLETE_SIZE; i++)
		m_CardsInHand.push_back(nullptr);
}

int UIHand::GetCardIDInHand(UICard* _UICard)
{
	for(int i = 0; i < HAND_COMPLETE_SIZE; i++)
	{
		UICard* uiCard = m_CardsInHand[i];
		if (uiCard == nullptr)
			continue;
		
		if(_UICard == uiCard)
			return i;
	}

	D::Cout(L"UIHand::GetCardIDInHand : No card " + _UICard->GetName() + L" in hand, index return -1");
	return -1;
}

float UIHand::GetLimitY()
{
	XMINT2 size = p_UIHand->GetSize();
	XMINT2 pos = p_UIHand->GetScreenPosition();

	return pos.y - size.y/2.0f;
}

void UIHand::SetUICardInHand(UICard* _Card, int _IndexInHand)
{
	XMINT2 uiHandSize = p_UIHand->GetSize();
	XMINT2 uiHandPos = p_UIHand->GetScreenPosition();

	float left = uiHandPos.x - uiHandSize.x / 2.0f + m_CardPlacementStart.x;
	float top = uiHandPos.y - uiHandSize.y / 2.0f + m_CardPlacementStart.y;
	float spacingX = (uiHandSize.x / HAND_COMPLETE_SIZE) + m_CardSpacing.x;

	float posX = left + spacingX * _IndexInHand;
	float posY = top + m_CardSpacing.y;

	if (_IndexInHand == 0)
		posY += 50;
	 
	_Card->SetScreenPosition(posX, posY);
	_Card->SetHandLimitY(GetLimitY());
	if (_IndexInHand == 0)
		_Card->SetActiveInHand(false);

	if (m_CardsInHand[_IndexInHand] == _Card)
		return;
	if (m_CardsInHand[_IndexInHand] != nullptr)
	{
		p_UIManager->RemoveButton(m_CardsInHand[_IndexInHand]);
		m_CardsToClear.push_back(m_CardsInHand[_IndexInHand]);
	}
		
	m_CardsInHand[_IndexInHand] = _Card;
}

// To Move on Utils
void OnPoseLogic(Button* _Button, XMFLOAT3 _pos)
{
	UICard* cardUI = (UICard*)_Button;
	int cardID = cardUI->GetCardID();
	Card* card = CardManager::Get()->GetCard(cardID);

	card->OnPlacement(_pos);
}

UICard* UIHand::CreateCard(Card* _Card)
{
	UICard* card = nullptr;
	Material* mat = MaterialManager::GetMaterial(_Card->GetMaterialName());
	if (mat == nullptr)
	{
		D::Cout("UIHand::SetCardInHand : No material named " + _Card->GetMaterialName() + " exist, card set with default texture");
		card = p_UIManager->CreateButton<UICard>(DEFAULT_TEXTURE_NAME);
	}
	else
	{
		int textureIndex = mat->TextureIndex;
		if (textureIndex == -1)
		{
			D::Cout("UIHand::SetCardInHand : Material " + _Card->GetMaterialName() + " has no texture, card set with default texture");
			card = p_UIManager->CreateButton<UICard>(DEFAULT_TEXTURE_NAME);
		}
		else
		{
			Texture* texture = TextureManager::GetTexture(textureIndex);
			card = p_UIManager->CreateButton<UICard>(texture->m_Name);
		}
	}

	std::string cardNameStr = _Card->GetName();
	std::wstring cardNameWstr = std::wstring(cardNameStr.begin(), cardNameStr.end());
	card->SetCardName(cardNameWstr);
	card->SetCardCost(_Card->GetCost());
	card->SetCardTier(_Card->GetTier());
	card->SetCardID(_Card->GetID());
	std::function<void(Button*, XMFLOAT3)> onPose = OnPoseLogic;
	card->SetOnPose(onPose);
	card->p_UIHand = this;
	card->SetScale(1.0f);

	return card;
}

void UIHand::SetCardInHand(Card* _Card, int _IndexInHand)
{
	SetUICardInHand(CreateCard(_Card), _IndexInHand);
}

void UIHand::ShowAllAlliesRanges()
{
	std::vector<LivingEntity*> barracks = SceneManager::Get()->GetActiveScene<SceneWorld>()->p_EntityManager->GetEntitiesByType(EntityMapIndex::BARRACK);
	for (auto b : barracks)
	{
		b->ShowRange();
	}
}

void UIHand::HideAllAlliesRanges()
{
	std::vector<LivingEntity*> barracks = SceneManager::Get()->GetActiveScene<SceneWorld>()->p_EntityManager->GetEntitiesByType(EntityMapIndex::BARRACK);
	for (auto b : barracks)
	{
		b->HideRange();
	}
}

void UIHand::HideUIHand()
{
	for(auto card : m_CardsInHand)
	{
		if (card == nullptr)
			continue;
		card->Hide();
	}

	p_UIHand->Hide();
}

void UIHand::ShowUIHand()
{
	for (auto card : m_CardsInHand)
	{
		if (card == nullptr)
			continue;
		card->Show();
	}

	p_UIHand->Show();
}

void UIHand::SetCardSelected(UICard* _Card)
{
	m_HasCardSelected = true;
	
	Card* card = CardManager::Get()->GetCard(_Card->GetCardID());
	if (card->GetTypeOfCard() == CardType::UNIT)
		ShowAllAlliesRanges();

	for (int i = 1; i < HAND_COMPLETE_SIZE; i++)
	{
		UICard* cardUI = m_CardsInHand[i];
		if (cardUI == _Card)
			continue;
		cardUI->SetActiveInHand(false);
		cardUI->ResetScaleEffect();
		cardUI->SetZIndex(CARD_ZINDEX - 1);
	}
	_Card->SetActiveInHand(true);
}

void UIHand::ResetCardSelected()
{
	m_HasCardSelected = false;

	HideAllAlliesRanges();

	for (int i = 1; i < HAND_COMPLETE_SIZE; i++)
	{
		UICard* card = m_CardsInHand[i];
		card->SetActiveInHand(true);
		card->SetZIndex(CARD_ZINDEX);
	}
	AlignCardsInHand();
}

void UIHand::ClearDeletedCard()
{
	for(int j = 0; j < m_CardsToClear.size(); j++)
	{
		UICard* cardToClear = m_CardsToClear[j];

		for (int i = 1; i < HAND_COMPLETE_SIZE; i++)
		{
			UICard* card = m_CardsInHand[i];
			if (card != cardToClear)
				continue;
			m_CardsInHand.erase(m_CardsInHand.begin() + i);
		}
		m_CardsToClear.erase(m_CardsToClear.begin() + j);
		j--;
	}
}

void UIHand::AlignCardsInHand()
{
	for (int i = 0; i < HAND_COMPLETE_SIZE; i++)
	{
		SetUICardInHand(m_CardsInHand[i], i);
	}		
}
