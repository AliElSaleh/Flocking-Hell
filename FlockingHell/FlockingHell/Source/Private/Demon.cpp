#include "Demon.h"
#include "Player.h"

void Demon::Init()
{
	Location = {300.0f, 100.0f};
	HitboxOffset = {50.0f, 105.0f};
	Hitbox = {Location.x + HitboxOffset.x, Location.y + HitboxOffset.y, float(Sprite.width)/11, float(Sprite.height)/4};
	SpriteBox = {Location.x, Location.y, float(Sprite.width), float(Sprite.height)};
	Health = 500;
	Speed = 120.0f;
	Damage = GetRandomValue(1, 3);
	bActive = true;
	bIsDead = false;
	bDebug = false;

	SetDestLocation({float(GetRandomValue(0 + Sprite.width, GetScreenWidth() - Sprite.width)), float(GetRandomValue(0 + Sprite.height/2, 150))});
}

void Demon::Update()
{
	if (bActive && !bIsDead)
	{
		if (!IsLowHealth())
			MoveToLocation(Destination);
		else
			MoveToLocation({float(GetRandomValue(0, GetScreenWidth())), float(GetRandomValue(0, 150))});

		SpriteBox.x = Location.x;
		SpriteBox.y = Location.y;

		Hitbox.x = Location.x + HitboxOffset.x;
		Hitbox.y = Location.y + HitboxOffset.y;
	}

	CheckCollisionWithPlayer();
	CheckCollisionWithPlayerBullets();
	CheckHealth();

	IsAtLocation(Destination);
}

void Demon::Draw() const
{
	// Draw the demon sprite
	if (bActive && !bIsDead)
		DrawTexture(Sprite, int(Location.x), int(Location.y), WHITE); // Demon sprite

	// Draw debug information
	if (bDebug && bActive && !bIsDead)
	{
		DrawRectangle(int(SpriteBox.x), int(SpriteBox.y), int(SpriteBox.width), int(SpriteBox.height), WHITE); // A rectangle that its width/height is the same as the sprite's width/height
		DrawRectangle(int(Hitbox.x), int(Hitbox.y), int(Hitbox.width), int(Hitbox.height), WHITE); // Hitbox
		DrawText(FormatText("Demon Health: %02i", Health), 10, 60, 20, RED); // Demon health
	}	
}

void Demon::CheckCollisionWithPlayer()
{
	if (CheckCollisionRecs(Player->Hitbox, SpriteBox))
		Player->Health -= Damage;
}

void Demon::CheckCollisionWithPlayerBullets()
{
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
		if (CheckCollisionCircleRec(Player->Bullet[i].Center, Player->Bullet[i].Radius, Hitbox))
			if (bActive && !bIsDead)
			{
				Player->ResetBullet(i);

				Health -= Player->BulletDamage;
			}
}

void Demon::CheckHealth()
{
	if (Health <= 0 && bActive && !bIsDead)
	{
		Health = 0;
		Player->EnemiesKilled++;
		bActive = false;
		bIsDead = true;
	}
}

bool Demon::IsLowHealth() const
{
	bool bLowHealth;

	Health <= 150 ? bLowHealth = true : bLowHealth = false;

	return bLowHealth;
}
