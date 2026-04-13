#pragma once

enum class CardType
{
	BUILDING,
	TACTICAL,
	UNIT,
	COUNT
};

enum class CardRole
{
	CIVIL,
	MILITARY,
	COUNT
};

class Card
{
protected:

	std::string m_Name = "NoName";
	std::string m_MatName = "NoName";

	// 1:BANANIUM 2:CONCRET 3:STEEL
	XMINT3 m_Cost = { -1, -1, -1 };

	int m_Tier = -1;
	int m_CardID = -1;
	int m_Experience = -1;

	int m_EntityIndex = -1;

	CardType m_Type = CardType::COUNT;

	CardRole m_Role = CardRole::COUNT;

	bool m_Lingering = false;

public:

	// - Builder -
	Card() = default;
	void Init(std::string _name, int _cardID, int _tier, int _entityIndex, CardType _type, CardRole _role, XMINT3 _cost, std::string _matName, bool _lingering);

	// - Virtual -
	virtual void OnPlacement(XMFLOAT3 _worldpos) {};
	virtual void OnUpdate(float _dt) {};
	virtual void OnEnd() {};

	// - Getter -
	std::string GetName() { return m_Name; };

	XMINT3 GetCost() { return m_Cost; };

	CardType GetTypeOfCard() { return m_Type; };
	CardRole GetRoleOfCard() { return m_Role; };

	int GetID() { return m_CardID; };
	int GetTier() { return m_Tier; };
	int GetExperience() { return m_Experience; };

	bool GetLingering() { return m_Lingering; };

	std::string GetMaterialName() { return m_MatName; };
};