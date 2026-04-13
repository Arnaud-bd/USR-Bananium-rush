#include "pch.h"

void ThreadAutoSwitchScene(void* data)
{
	if (ECS::Get().IsStarted() == false)
		return;
	Sleep(500);
	SceneManager::Get()->GoOnScene(1);
	((Thread*)data)->Stop();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,PSTR cmdLine, int showCmd)
{
	//DEBUG_MEMORY_START_RECORD // <-- may cause a very slow program exit
	DEBUG_ALLOC_CONSOLE

	ECSSettings settings;
	settings.m_WinHeight = 1080;
	settings.m_WinWidth = 1920;
	settings.m_ShowFPSInWindowName = false;
	settings.m_WinName = L"USSR : Bananium Rush";
	ECS& ecs = ECS::Create();

	GameManager* gm = ecs.CreatePermanentScript<GameManager>();

	SceneMenu* MenuScene = SceneManager::Get()->CreateScene<SceneMenu>();

	SceneWorld* MainScene = SceneManager::Get()->CreateScene<SceneWorld>();

	Scene* EndGameScene = SceneManager::Get()->CreateScene<Scene>();

	SceneManager::Get()->GoOnScene(0);

	// Until the DeckManager is init in SceneEmpty, 
	// we wait a small time in SceneEmpty before switching to the SceneWorld
	// The DeckManager setup need to be moved in an active scene (or in GameManager::OnStart)
	Thread* thread = ThreadManager::CreateThread(ThreadAutoSwitchScene);
	thread->SetData(thread);
	thread->DefineAsInfiniteLoop();
	thread->TryStart();

	ecs.Run(hInstance, settings);

	//DEBUG_MEMORY_END_RECORD
}

