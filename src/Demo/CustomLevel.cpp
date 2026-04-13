#include "pch.h"

// - Custom Levels - 

LevelIntro::LevelIntro() : Level(0.0f)
{
	Wave* w1 = new Wave();

	w1->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::SMALL));

	AddWave(w1);
}

// - Easy Level -

LevelEasy::LevelEasy() : Level(60.0f) // 60.0f
{
	Wave* w1 = new Wave();
	Wave* w2 = new Wave();
	Wave* w3 = new Wave();
	Wave* w4 = new Wave();
	Wave* w5 = new Wave();

	w1->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::SMALL));

	w2->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::SMALL));
	w2->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::SMALL));

	w3->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));

	w4->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::MEDIUM));
	w4->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));

	w5->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));
	w5->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));
	w5->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));

	AddWave(w1);
	AddWave(w2);
	AddWave(w3);
	AddWave(w4);
	AddWave(w5);
}

// - Extremely Hard Level -

LevelHard::LevelHard() : Level(60.0f)
{
	Wave* w1 = new Wave();
	Wave* w2 = new Wave();
	Wave* w3 = new Wave();
	Wave* w4 = new Wave();
	Wave* w5 = new Wave();
	Wave* w6 = new Wave();
	Wave* w7 = new Wave();
	Wave* w8 = new Wave();
	Wave* w9 = new Wave();
	Wave* w10 = new Wave();

	w1->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));

	w2->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::SMALL));
	w2->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::SMALL));
	w2->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::SMALL));

	w3->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::SMALL));
	w3->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::MEDIUM));
	w3->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));

	w4->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::SMALL));
	w4->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::MEDIUM));
	w4->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));
	w4->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));

	w5->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::MEDIUM));
	w5->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::SMALL));
	w5->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::MEDIUM));
	w5->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));

	w6->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));
	w6->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));
	w6->AddHorde(HordeStorage::GetHorde(HordeType::BODYBUILDER, HordeSize::MEDIUM));
	w6->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));
	w6->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));

	w7->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));
	w7->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::MEDIUM));
	w7->AddHorde(HordeStorage::GetHorde(HordeType::SOLDIER, HordeSize::SMALL));
	w7->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::MEDIUM));
	w7->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::SMALL));

	w8->AddHorde(HordeStorage::GetHorde(HordeType::SOLDIER, HordeSize::SMALL));
	w8->AddHorde(HordeStorage::GetHorde(HordeType::SOLDIER, HordeSize::SMALL));
	w8->AddHorde(HordeStorage::GetHorde(HordeType::SOLDIER, HordeSize::MEDIUM));
	w8->AddHorde(HordeStorage::GetHorde(HordeType::BODYBUILDER, HordeSize::SMALL));
	w8->AddHorde(HordeStorage::GetHorde(HordeType::BODYBUILDER, HordeSize::SMALL));
	w8->AddHorde(HordeStorage::GetHorde(HordeType::BODYBUILDER, HordeSize::MEDIUM));

	w9->AddHorde(HordeStorage::GetHorde(HordeType::SOLDIER, HordeSize::MEDIUM));
	w9->AddHorde(HordeStorage::GetHorde(HordeType::SOLDIER, HordeSize::MEDIUM));
	w9->AddHorde(HordeStorage::GetHorde(HordeType::BODYBUILDER, HordeSize::MEDIUM));
	w9->AddHorde(HordeStorage::GetHorde(HordeType::BODYBUILDER, HordeSize::MEDIUM));
	w9->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::MEDIUM));
	w9->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::MEDIUM));

	w10->AddHorde(HordeStorage::GetHorde(HordeType::BODYBUILDER, HordeSize::LARGE));
	w10->AddHorde(HordeStorage::GetHorde(HordeType::BODYBUILDER, HordeSize::LARGE));
	w10->AddHorde(HordeStorage::GetHorde(HordeType::SOLDIER, HordeSize::LARGE));
	w10->AddHorde(HordeStorage::GetHorde(HordeType::SOLDIER, HordeSize::LARGE));
	w10->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::LARGE));
	w10->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::LARGE));
	w10->AddHorde(HordeStorage::GetHorde(HordeType::KAMRADE, HordeSize::LARGE));
	w10->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::LARGE));
	w10->AddHorde(HordeStorage::GetHorde(HordeType::CUTTER, HordeSize::LARGE));

	AddWave(w1);
	AddWave(w2);
	AddWave(w3);
	AddWave(w4);
	AddWave(w5);
	AddWave(w6);
	AddWave(w7);
	AddWave(w8);
	AddWave(w9);
	AddWave(w10);
}
