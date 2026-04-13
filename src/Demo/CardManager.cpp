#include "pch.h"

CardManager::CardManager()
{
	p_Instance = this;
}

void CardManager::LoadCard()
{
	//JSON
	std::ifstream f("../../res/Cards/Cards.json");
	json data = json::parse(f);

	FactoryManager* facManager = FactoryManager::Get();

	for (int i = 0; i < data["Cards"].size(); i++)
	{
		std::string _name = data["Cards"][i]["Name"];
		int cardID = i;
		int tier = data["Cards"][i]["Tier"];
		int entityIndex = data["Cards"][i]["EntityIndex"];

		CardType type = data["Cards"][i]["Type"];

		CardRole role = data["Cards"][i]["Role"];

		XMINT3 cost;
		cost.x = data["Cards"][i]["Cost"][0];
		cost.y = data["Cards"][i]["Cost"][1];
		cost.z = data["Cards"][i]["Cost"][2];

		std::string matName = data["Cards"][i]["MatName"];

		bool lingering = data["Cards"][i]["Lingering"];
		
		Card* newCard = facManager->CreateCard(i);
		newCard->Init(_name, cardID, tier, entityIndex, type, role, cost, matName, lingering);
		m_SetOfCards.push_back(newCard);
	}
}
