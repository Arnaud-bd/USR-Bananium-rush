#include "pch.h"

void Card::Init(std::string _name, int _cardID, int _tier, int _entityIndex, CardType _type, CardRole _role, XMINT3 _cost, std::string _matName, bool _lingering)
{
	m_Name = _name;
	m_Tier = _tier;
	m_EntityIndex = _entityIndex;
	m_Type = _type;
	m_Role = _role;
	m_Cost = _cost;
	m_CardID = _cardID;
	m_MatName = _matName;
	m_Lingering = _lingering;
}
