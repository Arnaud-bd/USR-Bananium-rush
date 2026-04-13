#include "pch.h"

// -- UICard --

void UICard::Update(float _dt, XMINT2 _MousePos)
{
	Button::Update(_dt, _MousePos);

	if (p_Sprite->IsActive() == false)
		return;

	HandleHoverEffect();

	if (m_IsActivated == false) 
		return;

	if (IsClicked())
	{
		m_IsFollowing = !m_IsFollowing;
		int index = m_IsFollowing;
		SetScaleEffect(m_ScalesFollowing[index]);

		float screenPosY = GetScreenPosition().y;

		bool inHandZone = screenPosY > m_HandLimitY;
		if (index == 0 && inHandZone == false)
		{
			OnPose();
			return;
		}
		

		if (m_IsFollowing)
			p_UIHand->SetCardSelected(this);
		else
			p_UIHand->ResetCardSelected();
	}

	if (m_IsFollowing)
	{
		SetScreenPosition(_MousePos.x, _MousePos.y);
	}
}

void UICard::InitTextsWithTransform()
{
	if (p_Sprite->HaveSprite() == false)
		return;

	XMFLOAT4X4 worldTrs = p_Transform->GetWorldMatrix();
	float globalScale = p_Transform->GetScale().x;
	XMFLOAT3 worldPos = { worldTrs._41, worldTrs._42, worldTrs._43 };
	XMINT2 screenPos = WorldToScreen(worldPos, Device::GetWindow()->Size());

	XMFLOAT2 scale = p_Sprite->GetSprScale();
	XMINT2 size = p_Sprite->GetSize();
	XMFLOAT2 scaledSize = XMFLOAT2(scale.x / size.x, scale.y / size.y);

	XMFLOAT2 offsetName = XMFLOAT2(UICARD_NAME_POS_X * scaledSize.x, UICARD_NAME_POS_Y * scaledSize.y);
	XMFLOAT2 offsetCost = XMFLOAT2(UICARD_COST_POS_X * scaledSize.x, UICARD_COST_POS_Y * scaledSize.y);
	XMFLOAT2 offsetTier = XMFLOAT2(UICARD_TIER_POS_X * scaledSize.x, UICARD_TIER_POS_Y * scaledSize.y);

	p_Name->GetTransform()->SetLocalPosition({ offsetName.x, offsetName.y, 1.0f });
	p_Cost->GetTransform()->SetLocalPosition({ offsetCost.x, offsetCost.y, 1.0f });
	p_Tier->GetTransform()->SetLocalPosition({ offsetTier.x, offsetTier.y, 1.0f });

	p_Name->SetScale(globalScale * UICARD_STATS_SCALE);
	p_Cost->SetScale(globalScale * UICARD_STATS_SCALE);
	p_Tier->SetScale(globalScale * UICARD_STATS_SCALE);
}

void UICard::OnPose()
{
	DeckManager* dm = DeckManager::Get();

	Player* player = SceneManager::Get()->GetActiveScene<SceneWorld>()->p_Player;

	XMINT3 cost = dm->GetCardManager()->GetCard(m_CardID)->GetCost();

	if (player->HaveEnoughRessource(cost) == false)
	{
		p_UIHand->ResetCardSelected();
		return;
	}

	Hit hit;
	Card* card = CardManager::Get()->GetCard(m_CardID);

	if (card->GetTypeOfCard() == CardType::BUILDING)
	{
		hit = RayCast::CastRay(InputsManager::GetMousePosition(), GROUND_LAYER);
	}
	else
	{
		hit = RayCast::CastRay(InputsManager::GetMousePosition(), PLACEMENT_COLLIDERS_LAYER);
	}
	
	hit.m_Point.y = 0.0f;

	if (hit.m_CollidingID == -1)
	{
		SceneManager::Get()->GetActiveScene<SceneWorld>()->p_UIHand->ResetCardSelected();
		return;
	}

	int cardIndexInHand = p_UIHand->GetCardIDInHand(this);

	dm->PlayedCard(m_CardID, cardIndexInHand);
	player->SubstractToInventory(cost);

	p_UIHand->ResetCardSelected();

	if (m_OnPose)
		m_OnPose(this, hit.m_Point);
}

/// PUBLIC

UICard::~UICard()
{
	p_UIHand->ClearDeletedCard();
	p_UIManager->RemoveHUD(p_Name);
	p_UIManager->RemoveHUD(p_Cost);
	p_UIManager->RemoveHUD(p_Tier);
}

void UICard::Init(std::string _TextureName, std::function<void(HUD*)> m_OnUpdate)
{
	Button::Init(_TextureName, m_OnUpdate);

	m_ScalesFollowing[0] = 1.0f;
	m_ScalesFollowing[1] = UICARD_FOLLOWING_SCALE;

	m_ScalesHover[0] = UICARD_HOVER_SCALE;
	m_ScalesHover[1] = 1.0f;

	p_Name = p_UIManager->CreateHUD<HUD>("");
	p_Cost = p_UIManager->CreateHUD<HUD>("");
	p_Tier = p_UIManager->CreateHUD<HUD>("");

	ECS& ecs = ECS::Get();

	ecs.GetEntity(m_Entity)->AddChild(p_Name->GetEntity());
	ecs.GetEntity(m_Entity)->AddChild(p_Cost->GetEntity());
	ecs.GetEntity(m_Entity)->AddChild(p_Tier->GetEntity());

	SetZIndex(CARD_ZINDEX);

	InitTextsWithTransform();
}

void UICard::SetScaleEffect(float _Scale)
{
	HUD::SetScale(_Scale);
}

void UICard::HandleHoverEffect()
{
	// Is hover but not hover previous frame (and not following mouse)
	if (IsHovered() && m_IsHoverLastFrame == false && m_IsFollowing == false)
	{
		if (m_IsActivated == false)
		{
			int index = IsHovered() == false;
			HUD::SetScreenPosition(SavedPos.first, SavedPos.second - 100);
			m_IsHoverLastFrame = true;
		}
		else 
		{
			int index = IsHovered() == false;
			SetScaleEffect(m_ScalesHover[index]);
			SetZIndex(CARD_ZINDEX + 10);
			m_IsHoverLastFrame = true;
		}
	}
	// Not hover and not hover previous frame (and not following mouse)
	else if (IsHovered() == false && m_IsHoverLastFrame && m_IsFollowing == false)
	{
		if (m_IsActivated == false)
		{
			HUD::SetScreenPosition(SavedPos.first, SavedPos.second);
		}
		else
		{
			SetScaleEffect(m_ScalesHover[1]);
			SetZIndex(CARD_ZINDEX);
		}
	}

	if (IsHovered() == false)
	{
		m_IsHoverLastFrame = false;
		return;
	}
}

void UICard::SetScreenPosition(int _x, int _y)
{
	XMFLOAT3 worldPos = ScreenToWorld({ _x, _y }, Device::GetWindow()->Size());

	SavedPos.first = _x;
	SavedPos.second = _y;

	p_Transform->SetWorldPosition(worldPos);
}

void UICard::SetZIndex(int _z)
{
	HUD::SetZIndex(_z);
	p_Name->SetZIndex(_z + 1);
	p_Cost->SetZIndex(_z + 1);
	p_Tier->SetZIndex(_z + 1);
}

void UICard::SetScale(float _Scale)
{
	HUD::SetScale(_Scale);
	m_ScalesFollowing[0] = 1.0f * _Scale;
	m_ScalesFollowing[1] = UICARD_FOLLOWING_SCALE * _Scale;
	m_ScalesHover[0] = UICARD_HOVER_SCALE * _Scale;
	m_ScalesHover[1] = 1.0f * _Scale;
}

void UICard::SetActive(bool _IsActive)
{
	p_Sprite->SetActive(_IsActive);
	p_Name->SetActive(_IsActive);
	p_Cost->SetActive(_IsActive);
	p_Tier->SetActive(_IsActive);
}

void UICard::SetCardCost(XMINT3 _Cost)
{
	std::wstring costText = std::to_wstring(_Cost.x) + L"B " + std::to_wstring(_Cost.y) + L"R " + std::to_wstring(_Cost.z) + L"S";
	p_Cost->SetText(costText);
}

void UICard::SetCardTier(int _Tier)
{
	std::wstring tierText = L"T";
	switch (_Tier)
	{
	case 1:
		tierText += L"I";
		break;
	case 2:
		tierText += L"II";
		break;
	case 3:
		tierText += L"III";
		break;
	default:
		D::Cout("UICard::SetCardTier : Tier " + std::to_string(_Tier) + " is not supported, tier text set to X");
		tierText += L"X";
		break;
	}
	p_Tier->SetText(tierText);
}

void UICard::SetText(std::wstring _Text)
{
	D::Cout("UICard::SetText : Cannot set text of card, use SetCardName/SetCardCost...");
}

void UICard::Hide()
{
	HUD::Hide();
	p_Name->Hide();
	p_Cost->Hide();
	p_Tier->Hide();
}

void UICard::Show()
{
	HUD::Show();
	p_Name->Show();
	p_Cost->Show();
	p_Tier->Show();
}
// -- UICardDeck --

void UICardDeck::Update(float _dt, XMINT2 _MousePos)
{
	Button::Update(_dt, _MousePos);

	if (p_Sprite->IsActive() == false)
		return;

	HandleHoverEffect();

	if (m_IsActivated == false)
		return;

	if (IsClicked())
	{
		m_IsFollowing = !m_IsFollowing;
		int index = m_IsFollowing;
		SetScaleEffect(m_ScalesFollowing[index]);

		float screenPosY = GetScreenPosition().y;

		bool inHandZone = screenPosY > m_HandLimitY;
		if (index == 0 && inHandZone == false)
		{
			OnPose();
			return;
		}


		//if (m_IsFollowing)
		//	p_UIHand->SetCardSelected(this);
		//else
		//	p_UIHand->ResetCardSelected();
	}

	if (m_IsFollowing)
	{
		SetScreenPosition(_MousePos.x, _MousePos.y);
	}
}

// -- UILivingEntityInfos --

void UILivingEntityInfos::Init(std::string _TextureName, std::function<void(HUD*)> _OnUpdate)
{
	Button::Init(_TextureName, _OnUpdate);

	p_Sprite->SetZIndex(UI_ZINDEX);
	SetScreenPosition(1800, 400);

	p_NameHUD = p_UIManager->CreateHUD<HUD>("");
	p_NameHUD->SetText(L"Name");
	p_NameHUD->SetZIndex(UI_ZINDEX);
	p_NameHUD->SetScale(0.35f);
	TransformComponent* trs = p_NameHUD->GetTransform();
	p_Transform->AddChild(trs);
	trs->SetLocalPosition({ -100, -350, 0 });

	Hide();
}

void UILivingEntityInfos::IncreaseLifeSprites()
{
	int toAdd = m_LivingEntities.size() - m_LifeSprites.size();
	for(int i = 0; i < toAdd; i++)
	{
		Button* pSprite = p_UIManager->CreateButton<Button>("");
		m_LifeSprites.push_back(pSprite);
		p_Transform->AddChild(pSprite->GetTransform());
		pSprite->SetScale(0.5f);
		pSprite->SetZIndex(UI_ZINDEX + 1);
	}
}

void UILivingEntityInfos::DecreaseLifeSprites()
{
	ECS& ecs = ECS::Get();
	int LivingEntitiesSize = m_LivingEntities.size();
	int toRemove = m_LifeSprites.size() - LivingEntitiesSize;
	for (int i = LivingEntitiesSize + toRemove - 1; i > LivingEntitiesSize - 1; i--)
	{
		p_UIManager->RemoveButton(m_LifeSprites[i]);
		m_LifeSprites.erase(m_LifeSprites.begin() + i);
	}
}

void UILivingEntityInfos::Update(float _dt, XMINT2 _MousePos)
{
	Button::Update(_dt, _MousePos);

	m_Timer += _dt;
	if(m_Timer < UI_GROUP_LIFE_UPDATE_TIMER)
		return;
	m_Timer = 0.0f;

	UpdateShownUI();
}

void UILivingEntityInfos::UpdateShownUI()
{
	XMINT2 winSize = Device::GetWindow()->Size();
	for (int i = 0; i < m_LivingEntities.size(); i++)
	{
		LivingEntity* LE = m_LivingEntities[i];
		int health = LE->GetHealth();
		int maxHealth = LE->GetMaxHealth();
		if (health < 0)
			health = 0;
		m_LifeSprites[i]->SetText(L"HP " + std::to_wstring(health) + L"/" + std::to_wstring(maxHealth));
		m_LifeSprites[i]->GetTransform()->SetLocalPosition(ScreenToWorld({ 850, 200 + 50 * i }, winSize));
	}
}

void UILivingEntityInfos::Hide()
{ 
	HUD::Hide();
	for(auto sprite : m_LifeSprites)
		sprite->Hide();
	p_NameHUD->Hide();
}
void UILivingEntityInfos::Show() 
{ 
	HUD::Show();
	for (auto sprite : m_LifeSprites)
		sprite->Show();
	p_NameHUD->Show();
}


void UILivingEntityInfos::ShowEntityHP(LivingEntity* _pLE)
{
	std::string name = _pLE->GetName();
	p_NameHUD->SetText(name);
	m_LivingEntities.push_back(_pLE);
	IncreaseLifeSprites();
	Show();
}

void UILivingEntityInfos::ClearEntitiesShown()
{
	m_LivingEntities.clear();
	DecreaseLifeSprites();
	Hide();
}

void UILivingEntityInfos::RemoveEntityShown(LivingEntity* _pLE)
{
	for(int i = 0; i < m_LivingEntities.size(); i++)
	{
		LivingEntity* LE = m_LivingEntities[i];
		if (LE != _pLE)
			continue;
		m_LivingEntities.erase(m_LivingEntities.begin() + i);
		DecreaseLifeSprites();
		return;
	}
}


