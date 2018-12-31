#pragma once
#include <raylib.h>

#define WAVE_ONE_BULLETS unsigned short(20)
#define WAVE_TWO_BULLETS unsigned short(30)

struct Player;

struct Bullet
{
	void Init();
	void InitArray(int i);
	void InitWave(int Wave);
	void Update();
	void Draw() const;

	void CheckCollisionWithPlayer();

	bool IsOutsideWindow() const;
	bool IsLocationYGreaterThan(float Y) const; // Is this bullet greater than (Y) on the Y axis

	void ResetBullet();
	void Clear();

	Vector2 Location{};
	Vector2 CollisionOffset{};
	Vector2 Center{};

	Texture2D Sprite{};

	struct Player* Player{};

	float Radius{};
	float Speed{};

	unsigned short FramesCounter{};
	unsigned short Damage{};
	unsigned short Spacing{};

	bool bIsHit{};
	bool bActive{};

private:

	bool bDebug{};
};
