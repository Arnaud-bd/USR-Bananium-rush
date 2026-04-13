#pragma once

#define SPEED_SLOW 2.5f
#define SPEED_NORMAL 5.0f
#define SPEED_FAST 7.5f

class CardKamrade : public Card
{
public:
	void OnPlacement(XMFLOAT3 _worldPos) override;
};

class CardCutter : public Card
{
public:
	void OnPlacement(XMFLOAT3 _worldPos) override;
};

class CardBarrack : public Card
{
public:
	void OnPlacement(XMFLOAT3 _worldPos) override;
};

class CardMinion : public Card
{
public:
	void OnPlacement(XMFLOAT3 _worldPos) override;
};

class CardHealTower: public Card
{
public:
	void OnPlacement(XMFLOAT3 _worldPos) override;
};

class CardBodybuilder : public Card
{
public:
	void OnPlacement(XMFLOAT3 _worldPos) override;
};

class CardSoldier : public Card
{
public:
	void OnPlacement(XMFLOAT3 _worldPos) override;
};