#pragma once

// - Custom Levels - 

class LevelIntro : public Level
{
public:
	LevelIntro();
};


/*
Weak enemies with a large cooldown between waves
- 5 waves
- Cutter and Kamrade
- 60 seconds between waves
*/
class LevelEasy : public Level
{
public:
	LevelEasy();
};
/*
Hard enemies with an large cooldown between waves
- 10 waves
- Cutter, Kamrade ...
- 60 seconds between waves
*/
class LevelHard : public Level
{
public:
	LevelHard();
};