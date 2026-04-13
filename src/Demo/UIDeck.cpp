#include "pch.h"

void UIDeck::Init(UIManager* _pUIManager)
{
	p_UIManager = _pUIManager;
	int width = Device::GetWindow()->Width();
	int height = Device::GetWindow()->Height();

	CardManager* CM = DeckManager::Get()->GetCardManager()->Get();

	for (int Test = 0; Test < 5; ++Test)
	{
		for (int i = 0; i < CM->GetSizeOfSet(); ++i)
		{
			m_UICards.push_back(CreateCard(CM->GetCard(i)));
			m_UICards[Test + i]->SetScale(0.5f);
		}
	}
	PlacementScoller();
}

UICardDeck* UIDeck::CreateCard(Card* _Card)
{
	UICardDeck* card = nullptr;
	Material* mat = MaterialManager::GetMaterial(_Card->GetMaterialName());
	if (mat == nullptr)
	{
		D::Cout("UIHand::SetCardInHand : No material named " + _Card->GetMaterialName() + " exist, card set with default texture");
		card = p_UIManager->CreateButton<UICardDeck>(DEFAULT_TEXTURE_NAME);
	}
	else
	{
		int textureIndex = mat->TextureIndex;
		if (textureIndex == -1)
		{
			D::Cout("UIHand::SetCardInHand : Material " + _Card->GetMaterialName() + " has no texture, card set with default texture");
			card = p_UIManager->CreateButton<UICardDeck>(DEFAULT_TEXTURE_NAME);
		}
		else
		{
			Texture* texture = TextureManager::GetTexture(textureIndex);
			card = p_UIManager->CreateButton<UICardDeck>(texture->m_Name);
		}
	}

	std::string cardNameStr = _Card->GetName();
	std::wstring cardNameWstr = std::wstring(cardNameStr.begin(), cardNameStr.end());

	card->SetCardName(cardNameWstr);
	card->SetCardCost(_Card->GetCost());
	card->SetCardTier(_Card->GetTier());
	card->SetCardID(_Card->GetID());

	std::function<void(Button*, XMFLOAT3)> AddCard = nullptr;
	card->SetOnPose(AddCard);

	card->p_UIDeck = this;

	card->SetScale(1.0f);

	return card;
}

void UIDeck::PlacementScoller()
{
	for (int i = 0; i < m_UICards.size(); ++i)
		m_UICards[i]->SetScreenPosition((m_UICards[i]->GetSize().y * m_UICards[i]->GetTransform()->GetScale().y + 10) * i, 810);
}