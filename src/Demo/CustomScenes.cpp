#include "pch.h"

// -- STRUCT GAMEPLAY --
void SceneGameplay::OnInit()
{
	ECS& ecs = ECS::Get();

	p_GM = GameManager::Get();
	p_UIManager = new UIManager();
	p_EntityManager = new EntityManager();
}

void SceneGameplay::OnUpdate()
{
	p_UIManager->Update();
	p_EntityManager->Update(ECS::Get().GetDeltaTime());
}

// -------------------------------------------

// -- SCENE MENU --
void SceneMenu::OnInit()
{
    ECS& ecs = ECS::Get();

    m_TextID = ecs.CreateEntity();

    SpriteComponent* SC = ecs.AddComponent<SpriteComponent>(m_TextID);
    SC->SetText(L"PRESS SPACE FOR PLAY");

    TransformComponent* TC = ecs.AddComponent<TransformComponent>(m_TextID);

    auto windowSize = Device::Get()->GetWindow()->Size();
    int X = (int)(windowSize.x * 0.3f);
    int Y = (int)(windowSize.y * 0.5f);

    TC->SetWorldPosition(ScreenToWorld({ X, Y }, windowSize));
}
void SceneMenu::OnUpdate()
{
	if (InputsManager::GetKeyDown(Keyboard::SPACE))
	{
		GameManager::Get()->m_State = GameState::PLAY;
	}
}

// -- SCENE WORLD --
void SceneWorld::SpawnOre(float finalX, float finalZ, OreType type, XMFLOAT4 color)
{
	Deposit* deposit = FactoryManager::Get()->CreateEntity<Deposit>(EntityFacIndex::DEPOSIT);

	float y = deposit->GetTransformComponent()->GetScale().y * 0.5f;

	deposit->SetPosition(finalX, y, finalZ);
	deposit->GetMesh()->SetColor(color);
	deposit->SetType(type);

	m_Deposit.push_back(deposit);
}

void SceneWorld::OnInit()
{
	SceneGameplay::OnInit();

	ECS& ecs = ECS::Get();

	GameManager::Get()->GetLevelManager()->PlayLevel(0);

	// - UI HAND - 
	p_UIHand = new UIHand();
	p_UIHand->Init(p_UIManager);

	// - UI Life - 
	p_UIEntityInfo = p_UIManager->CreateButton<UILivingEntityInfos>("UILifeTex");

	// - PLAYER BEHAVIOR - 
	p_Player = new Player();

	// - EFFECTS - 
	p_BloodManager = ecs.CreateScript<BloodManager>();

	// - WAVE INFO -
	m_UIWaveInfo = ecs.CreateEntity();
	ecs.AddComponent<SpriteComponent>(m_UIWaveInfo);

	TransformComponent* waveInfoTrs = ecs.AddComponent<TransformComponent>(m_UIWaveInfo);
	waveInfoTrs->SetWorldPosition(ScreenToWorld({ 1600 ,0 }, Device::Get()->GetWindow()->Size()));
	waveInfoTrs->SetScale(0.5f);

	// - UI INVENTORY - 
	m_InventoryUI_ID = ecs.CreateEntity();
	ecs.AddComponent<SpriteComponent>(m_InventoryUI_ID);

	TransformComponent* inventoryTrs = ecs.AddComponent<TransformComponent>(m_InventoryUI_ID);
	inventoryTrs->SetWorldPosition(ScreenToWorld({ 0 ,0 }, Device::Get()->GetWindow()->Size()));
	inventoryTrs->SetScale(0.5f);

	// - UI NEXUS ENEMY -
	m_NexusEnemyUI_ID = ecs.CreateEntity();
	ecs.AddComponent<SpriteComponent>(m_NexusEnemyUI_ID);

	TransformComponent* nexusTrs = ecs.AddComponent<TransformComponent>(m_NexusEnemyUI_ID);
	nexusTrs->SetWorldPosition(ScreenToWorld({ 0 ,50 }, Device::Get()->GetWindow()->Size()));
	nexusTrs->SetScale(0.5f);

	// - DECK - 
	DeckManager* deckManager = DeckManager::Get();

	// - DECKBUILDING - 
	deckManager->AddCardTo(CardSet::DECK, 0);
	deckManager->AddCardTo(CardSet::DECK, 0);

	deckManager->AddCardTo(CardSet::DECK, 1);
	deckManager->AddCardTo(CardSet::DECK, 1);

	deckManager->AddCardTo(CardSet::DECK, 2);
	deckManager->AddCardTo(CardSet::DECK, 2);

	deckManager->AddCardTo(CardSet::DECK, 3);
	deckManager->AddCardTo(CardSet::DECK, 3);
	deckManager->AddCardTo(CardSet::DECK, 3);
	deckManager->AddCardTo(CardSet::DECK, 3);

	deckManager->AddCardTo(CardSet::DECK, 4);
	deckManager->AddCardTo(CardSet::DECK, 4);

	deckManager->AddCardTo(CardSet::DECK, 5);
	deckManager->AddCardTo(CardSet::DECK, 5);

	deckManager->AddCardTo(CardSet::DECK, 6);
	deckManager->AddCardTo(CardSet::DECK, 6);

	deckManager->SetUIHand(p_UIHand);
	deckManager->InitRound();

	// - GROUND - 
	
	m_GroundID = ecs.CreateEntity();
	TransformComponent* groundTransf = ecs.AddComponent<TransformComponent>(m_GroundID);
	MeshComponent* groundMesh = ecs.AddComponent<MeshComponent>(m_GroundID);

	ColliderComponent* groundCollider = ecs.AddComponent<ColliderComponent>(m_GroundID);
	AddToLayer(groundCollider, GROUND_LAYER);
	ColliderAABB* boxCollider1 = groundCollider->SetType<ColliderAABB>();

	groundMesh->SetGeometry(GeometryManager::Get()->GetGeometry(GEO_TYPE::PLANE));
	groundMesh->SetMaterial(MATERIAL_MOON_NAME);

	groundMesh->SetColor({ 0.992, 0.22, 1, 1.0f });
	groundTransf->SetScale(m_WorldSize.first, 0.f, m_WorldSize.second);

	// - SIDE - 

	int SideOneID = ecs.CreateEntity();	

	TransformComponent* SideOneTransf = ecs.AddComponent<TransformComponent>(SideOneID);
	SideOneTransf->SetScale(m_WorldSize.first, 0.f, m_WorldSize.second);
	SideOneTransf->RotateWorld(0, -Maths::Pi * 0.5f, 0);
	SideOneTransf->SetWorldPosition({ 0, -m_WorldSize.first * 0.5f, -m_WorldSize.second * 0.5f });

	MeshComponent* SideOneMesh = ecs.AddComponent<MeshComponent>(SideOneID);
	SideOneMesh->SetGeometry(GeometryManager::Get()->GetGeometry(GEO_TYPE::PLANE));
	SideOneMesh->SetColor({ 0.388f, 0.388f, 0.388f, 1.0f });
	SideOneMesh->SetMaterial(MATERIAL_ROCKSIDE_NAME);

	int SideTwoID = ecs.CreateEntity();

	TransformComponent* SideTwoTransf = ecs.AddComponent<TransformComponent>(SideTwoID);
	SideTwoTransf->SetScale(m_WorldSize.first, 0.f, m_WorldSize.second);
	SideTwoTransf->RotateWorld(0, -Maths::Pi * 0.5f, -Maths::Pi * 0.5f);
	SideTwoTransf->SetWorldPosition({ m_WorldSize.first * 0.5f, -m_WorldSize.second * 0.5f, 0 });

	MeshComponent* SideTwoMesh = ecs.AddComponent<MeshComponent>(SideTwoID);
	SideTwoMesh->SetGeometry(GeometryManager::Get()->GetGeometry(GEO_TYPE::PLANE));
	SideTwoMesh->SetColor({ 0.388f, 0.388f, 0.388f, 1.0f });
	SideTwoMesh->SetMaterial(MATERIAL_ROCKSIDE_NAME);

	// - NEXUS - 
	m_Allie_Nexus = FactoryManager::Get()->CreateEntity<Nexus>(EntityFacIndex::NEXUS); //NEXUS ALLY
	m_Allie_Nexus->SetTeam(TEAM_ZENTIL);
	m_Allie_Nexus->AddEnemy(TEAM_ARNO);
	m_Allie_Nexus->GetRangeColliderComponent()->AddToLayer(PLACEMENT_COLLIDERS_LAYER);
	m_Allie_Nexus->GetMesh()->SetGeometry(GeometryManager::Get()->GetGeometry(GEO_TYPE::CYLINDER));
	m_Allie_Nexus->SetBaseColor({ 0.f, 0.f, 1.f, 1.f });
	m_Allie_Nexus->GetTransformComponent()->SetScale(2, 5, 2);
	m_Allie_Nexus->SetPosition(-m_WorldSize.first * 0.5f + 10, m_Allie_Nexus->GetTransformComponent()->GetScale().y * 0.5f, -m_WorldSize.second * 0.5f + 10);


	m_Enemy_Nexus = FactoryManager::Get()->CreateEntity<Nexus>(EntityFacIndex::NEXUS); //NEXUS ENEMI
	m_Enemy_Nexus->SetTeam(TEAM_ARNO);
	m_Enemy_Nexus->AddEnemy(TEAM_ZENTIL);
	m_Enemy_Nexus->GetMesh()->SetGeometry(GeometryManager::Get()->GetGeometry(GEO_TYPE::CYLINDER));
	m_Enemy_Nexus->SetBaseColor({ 1.f, 0.f, 0.f, 1.f });
	m_Enemy_Nexus->GetTransformComponent()->SetScale(2, 5, 2);
	m_Enemy_Nexus->SetPosition(m_WorldSize.first * 0.5f - 10, m_Enemy_Nexus->GetTransformComponent()->GetScale().y * 0.5f, m_WorldSize.second * 0.5f - 10);

	// - ORE -
	SpawnOre(75, 75, OreType::BANANIUM, { 1, 0.988, 0.212 ,1.f });
	SpawnOre(25, 25, OreType::BANANIUM, { 1, 0.988, 0.212 ,1.f });
	SpawnOre(-25, -25, OreType::BANANIUM, { 1, 0.988, 0.212 ,1.f });
	SpawnOre(-75, -75, OreType::BANANIUM, { 1, 0.988, 0.212 ,1.f });

	SpawnOre(82, 78, OreType::IRON, { 0.55f, 0.55f, 0.55f, 1.f });
	SpawnOre(28, 33, OreType::IRON, { 0.55f, 0.55f, 0.55f, 1.f });
	SpawnOre(-22, -18, OreType::IRON, { 0.55f, 0.55f, 0.55f, 1.f });
	SpawnOre(-78, -83, OreType::IRON, { 0.55f, 0.55f, 0.55f, 1.f });

	SpawnOre(88, 72, OreType::IRON, { 0.55f, 0.55f, 0.55f, 1.f });
	SpawnOre(22, 38, OreType::IRON, { 0.55f, 0.55f, 0.55f, 1.f });
	SpawnOre(-18, -28, OreType::IRON, { 0.55f, 0.55f, 0.55f, 1.f });
	SpawnOre(-72, -88, OreType::IRON, { 0.55f, 0.55f, 0.55f, 1.f });

	SpawnOre(68, 82, OreType::ROCK, { 1.f, 1.f, 1.f, 1.f });
	SpawnOre(18, 35, OreType::ROCK, { 1.f, 1.f, 1.f, 1.f });
	SpawnOre(-35, -22, OreType::ROCK, { 1.f, 1.f, 1.f, 1.f });
	SpawnOre(-85, -68, OreType::ROCK, { 1.f, 1.f, 1.f, 1.f });

	SpawnOre(60, 90, OreType::ROCK, { 1.f, 1.f, 1.f, 1.f });
	SpawnOre(30, 10, OreType::ROCK, { 1.f, 1.f, 1.f, 1.f });
	SpawnOre(-40, -30, OreType::ROCK, { 1.f, 1.f, 1.f, 1.f });
	SpawnOre(-90, -60, OreType::ROCK, { 1.f, 1.f, 1.f, 1.f });

	//Cinematic Camera
	int camId = ecs.CreateEntity();

	m_CinematicCameraTrs = ecs.AddComponent<TransformComponent>(camId);
	m_CinematicCameraTrs->RotateWorld(-Maths::Pi / 4.f, Maths::Pi / 4.f, 0.0f);
	m_CinematicCameraTrs->SetWorldPosition({ 150.f, 50.f, 75.f });

	m_CinematicCamera = ecs.AddComponent<CameraComponent>(camId);
	m_CinematicCamera->SetMainCamera(true);

	p_UIHand->HideUIHand();
}

void SceneWorld::OnUpdate()
{
	float dt = ECS::Get().GetDeltaTime();
	SceneGameplay::OnUpdate();
	GameManager::Get()->GetLevelManager()->Update(dt);

	if(m_Cinematic)
	{

		m_CinematicCameraTrs->TranslateWorld({-Maths::Pi * m_CinematicSpeed * ECS::GetDeltaTime(), 0, -Maths::Pi * m_CinematicSpeed * ECS::GetDeltaTime() });

		m_CinematicSpeed += ECS::GetDeltaTime();

		if(m_CinematicCameraTrs->GetWorldPosition().x <= -75.0f || InputsManager::GetKeyDown(Keyboard::NUMPAD8))
		{
			m_Cinematic = false;
			CameraComponent* cam = p_Player->GetCamera();
			cam->SetMainCamera(true);
			int playerCamID = cam->GetID();
			ECS::Get().GetComponent<TransformComponent>(playerCamID)->SetWorldPosition(m_CinematicCameraTrs->GetWorldPosition());

			p_UIHand->ShowUIHand();

			GameManager::Get()->GetLevelManager()->PlayLevel(1); 	//Easy Mode
			//GameManager::Get()->GetLevelManager()->PlayLevel(2);	//Hard Mode
		}

		return;
	}

	p_Player->Update();

	if (m_Allie_Nexus->TimerNexus())
		p_Player->AddToInventory({ m_Allie_Nexus->GetProduction() , 0, 0 });
	
	if (m_Allie_Nexus->IsDead())
		GameManager::Get()->m_State = GameState::LOSE;
	if (m_Enemy_Nexus->IsDead())
		GameManager::Get()->m_State = GameState::WIN;

	UpdateUI();
}

void SceneWorld::UpdateUI()
{
	// - WAVE INFO -
	SpriteComponent* WaveInfo_SC = ECS::Get().GetComponent<SpriteComponent>(m_UIWaveInfo);

	Level* pLevel = GameManager::Get()->GetLevelManager()->GetActualLevel();
	int timer = (int)pLevel->GetWaveTimer();
	int duration = (int)pLevel->GetDurationBetweenWave();
	int actualWave = pLevel->GetActualWave();
	int waveCount = pLevel->GetWavesCount();
	int remaining = duration - timer;

	wstring WaveInfoString = L"WAVE " + 
		std::to_wstring(actualWave) + L"/" + std::to_wstring(waveCount)
		+ L" in " + std::to_wstring(remaining) + L"s";
	WaveInfo_SC->SetText(WaveInfoString);

	// - INVENTORY -
	SpriteComponent* InventorySC = ECS::Get().GetComponent<SpriteComponent>(m_InventoryUI_ID);
	SpriteComponent* NexusInfoSC = ECS::Get().GetComponent<SpriteComponent>(m_NexusEnemyUI_ID);

	wstring BananiumString = L" Bananium " + std::to_wstring(p_Player->GetInventory().x);
	wstring IronString = L" Rock " + std::to_wstring(p_Player->GetInventory().y);
	wstring RockString = L" Steel " + std::to_wstring(p_Player->GetInventory().z);

	wstring NexusString = L" Enemy Nexus Life " + std::to_wstring(m_Enemy_Nexus->GetHealth());

	InventorySC->SetText(BananiumString + L" " + IronString + L" " + RockString);
	NexusInfoSC->SetText(NexusString);
}

// -- SCENE END --

void SceneEnd::OnInit()
{
	ECS& ecs = ECS::Get();
	m_TextID = ecs.CreateEntity();
	TransformComponent* TC = ecs.AddComponent<TransformComponent>(m_TextID);

	auto windowSize = Device::Get()->GetWindow()->Size();
	int X = (int)(windowSize.x * 0.3f);
	int Y = (int)(windowSize.y * 0.5f);

	TC->SetWorldPosition(ScreenToWorld({ X, Y }, windowSize));

}

void SceneEnd::OnUpdate()
{
	if (m_TextSetup == true)
		return;

	ECS& ecs = ECS::Get();
	SpriteComponent* SC = ecs.AddComponent<SpriteComponent>(m_TextID);
	if (GameManager::Get()->m_State == GameState::WIN)
	{
		SC->SetText(L"GG YOU WIN");
	}
	if (GameManager::Get()->m_State == GameState::LOSE)
	{
		SC->SetText(L"BOOUUHHH YOU LOSE");
	}
}


