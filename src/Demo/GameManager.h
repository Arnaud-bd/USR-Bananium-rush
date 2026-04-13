enum class GameState
{
	PLAY,
	WIN,
	LOSE,
	MENU,
	COUNT
};

DECLARE_SCRIPT(GameManager)

private:

	inline static GameManager* p_Instance = nullptr;

	DeckManager* p_DeckManager = nullptr; // To avoid multiple Get()
	LevelManager* p_LevelManager = nullptr;
	UIStats* p_UIFPS = nullptr;

public:
	GameState m_State;

	static GameManager* Get() { return p_Instance; }
	LevelManager* GetLevelManager() { return p_LevelManager; }

	void LoadingPhase();

	virtual void OnStart() override;
	virtual void OnUpdate() override;

END_SCRIPT