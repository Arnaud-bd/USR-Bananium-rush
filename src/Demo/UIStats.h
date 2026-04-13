#pragma once

class UIStats
{
protected:
	std::wstring m_Text;
	UIString* p_String = nullptr;
	float m_Timer = 0.0f;
	bool m_IsDirty = false;

public:
	UIStats();

	void SetPosition(float x, float y);
	void SetText(std::wstring text);

	void Update(float dt);
};

