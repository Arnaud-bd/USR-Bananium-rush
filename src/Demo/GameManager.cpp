#include "pch.h"

void GameManager::OnStart()
{
	p_Instance = this;

	ECS& ecs = ECS::Get();

	LoadingPhase();

	p_DeckManager = new DeckManager();
	p_LevelManager = new LevelManager();
	p_LevelManager->CreateLevel<LevelIntro>();
	p_LevelManager->CreateLevel<LevelEasy>();
	p_LevelManager->CreateLevel<LevelHard>();


	/*p_UIFPS = new UIStats();
	p_UIFPS->SetText(L"FPS 0");
	p_UIFPS->SetPosition(0.0f, 0.0f);*/

	m_State = GameState::MENU;
}

void GameManager::LoadingPhase()
{	
	//DEBUG
	TextureManager::LoadTexture("CardTemplateTex", L"../../res/Cards/CardTemplate.dds");
	MaterialManager::CreateMaterial(SHADER_TEXTURE_A_NAME, "Template", "CardTemplateTex");

	TextureManager::LoadTexture(TEXTURE_DEBUGCUBE_NAME, L"../../res/Textures/DEBUG_UV.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_DEBUGCUBE_NAME, TEXTURE_DEBUGCUBE_NAME);

	//UI
	TextureManager::LoadTexture("UILifeTex", L"../../res/UI/UILife.dds");
	MaterialManager::CreateMaterial(SHADER_TEXTURE_A_NAME, "UILifeMat", "UILifeTex");

	//CARD
	TextureManager::LoadTexture(TEXTURE_CARDBARACK_NAME, L"../../res/Textures/Card_Barack.dds");
	MaterialManager::CreateMaterial(SHADER_TEXTURE_A_NAME, MATERIAL_CARDBARACK_NAME, TEXTURE_CARDBARACK_NAME);

	TextureManager::LoadTexture(TEXTURE_CARDCUTTER_NAME, L"../../res/Textures/Card_Cutter.dds");
	MaterialManager::CreateMaterial(SHADER_TEXTURE_A_NAME, MATERIAL_CARDCUTTER_NAME, TEXTURE_CARDCUTTER_NAME);

	TextureManager::LoadTexture(TEXTURE_CARDHEALING_NAME, L"../../res/Textures/Card_Healing.dds");
	MaterialManager::CreateMaterial(SHADER_TEXTURE_A_NAME, MATERIAL_CARDHEALING_NAME, TEXTURE_CARDHEALING_NAME);

	TextureManager::LoadTexture(TEXTURE_CARDKAMRAD_NAME, L"../../res/Textures/Card_Kamrad.dds");
	MaterialManager::CreateMaterial(SHADER_TEXTURE_A_NAME, MATERIAL_CARDKAMRAD_NAME, TEXTURE_CARDKAMRAD_NAME);

	TextureManager::LoadTexture(TEXTURE_CARDMINIONS_NAME, L"../../res/Textures/Card_Minions.dds");
	MaterialManager::CreateMaterial(SHADER_TEXTURE_A_NAME, MATERIAL_CARDMINIONS_NAME, TEXTURE_CARDMINIONS_NAME);

	TextureManager::LoadTexture(TEXTURE_CARDBODYBUILDER_NAME, L"../../res/Textures/Card_BodyBuilder.dds");
	MaterialManager::CreateMaterial(SHADER_TEXTURE_A_NAME, MATERIAL_CARDBODYBUILDER_NAME, TEXTURE_CARDBODYBUILDER_NAME);

	TextureManager::LoadTexture(TEXTURE_CARDSOLDIER_NAME, L"../../res/Textures/Card_Soldier.dds");
	MaterialManager::CreateMaterial(SHADER_TEXTURE_A_NAME, MATERIAL_CARDSOLDIER_NAME, TEXTURE_CARDSOLDIER_NAME);

	//MAP
	TextureManager::LoadTexture(TEXTURE_MOON_NAME, L"../../res/Textures/Ground.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_NAME, MATERIAL_MOON_NAME, TEXTURE_MOON_NAME);

	TextureManager::LoadTexture(TEXTURE_ROCKSIDE_NAME, L"../../res/Textures/RockSide.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_ROCKSIDE_NAME, TEXTURE_ROCKSIDE_NAME);

	//MONKEY ALLY
	TextureManager::LoadTexture(TEXTURE_CUTTERALLY_NAME, L"../../res/Textures/Cutter_Ally.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_CUTTERALLY_NAME, TEXTURE_CUTTERALLY_NAME);

	TextureManager::LoadTexture(TEXTURE_KAMRADALLY_NAME, L"../../res/Textures/Kamrad_Ally.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_KAMRADALLY_NAME, TEXTURE_KAMRADALLY_NAME);

	TextureManager::LoadTexture(TEXTURE_MINIONSALLY_NAME, L"../../res/Textures/Minions_Ally.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_MINIONSALLY_NAME, TEXTURE_MINIONSALLY_NAME);

	TextureManager::LoadTexture(TEXTURE_BODYBUILDERALLY_NAME, L"../../res/Textures/BodyBuilder_Ally.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_BODYBUILDERALLY_NAME, TEXTURE_BODYBUILDERALLY_NAME);

	TextureManager::LoadTexture(TEXTURE_SOLDIERALLY_NAME, L"../../res/Textures/Soldier_Ally.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_SOLDIERSALLY_NAME, TEXTURE_SOLDIERALLY_NAME);

	//MONKEY ENEMY
	TextureManager::LoadTexture(TEXTURE_CUTTERENEMY_NAME, L"../../res/Textures/Cutter_Enemy.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_CUTTERENEMY_NAME, TEXTURE_CUTTERENEMY_NAME);

	TextureManager::LoadTexture(TEXTURE_KAMRADENEMY_NAME, L"../../res/Textures/Kamrad_Enemy.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_KAMRADENEMY_NAME, TEXTURE_KAMRADENEMY_NAME);

	TextureManager::LoadTexture(TEXTURE_MINIONSENEMY_NAME, L"../../res/Textures/Minions_Enemy.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_MINIONSENEMY_NAME, TEXTURE_MINIONSENEMY_NAME);

	TextureManager::LoadTexture(TEXTURE_BODYBUILDERENEMY_NAME, L"../../res/Textures/BodyBuilder_Enemy.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_BODYBUILDERENEMY_NAME, TEXTURE_BODYBUILDERENEMY_NAME);

	TextureManager::LoadTexture(TEXTURE_SOLDIERENEMY_NAME, L"../../res/Textures/Soldier_Enemy.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_SOLDIERENEMY_NAME, TEXTURE_SOLDIERENEMY_NAME);

	//BUILDING
	TextureManager::LoadTexture(TEXTURE_BARACKBUILD_NAME, L"../../res/Textures/Barack.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_BARACKBUILD_NAME, TEXTURE_BARACKBUILD_NAME);

	TextureManager::LoadTexture(TEXTURE_BARACKHEALBUILD_NAME, L"../../res/Textures/HealingBarack.dds");
	MaterialManager::CreateMaterial(SHADER_COLOR_TEXTURE_A_NAME, MATERIAL_BARACKHEALBUILD_NAME, TEXTURE_BARACKHEALBUILD_NAME);

}

void GameManager::OnUpdate()
{
	switch (m_State)
	{
		case(GameState::MENU):
		{
			SceneManager::Get()->GoOnScene(0);
			break;
		}
		case(GameState::PLAY):
		{
			SceneManager::Get()->GoOnScene(1);
			break;
		}
		case(GameState::LOSE):
		{
			SceneManager::Get()->GoOnScene(2);
			break;
		}
		case(GameState::WIN):
		{
			SceneManager::Get()->GoOnScene(2);
			break;
		}
		default:
			break;
	}

	//p_UIFPS->Update(ECS::Get().GetDeltaTime());
	//p_UIFPS->SetText(L"FPS " + std::to_wstring(ECS::GetFPS()));
}
