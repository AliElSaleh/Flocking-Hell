#include "CirclePattern.h"
#include "Assets.h"
#include "Enemy.h"
#include "Player.h"

const char* CirclePatternNames[]
{
	Stringify(CIRCLE),
	Stringify(CIRCLE HOLE),
	Stringify(CIRCLE HOLE LOCK ON)
};

CirclePattern::CirclePattern()
{
	CirclePattern::Init();
}

void CirclePattern::Init()
{
	BulletSprite = GetAsset(RedBullet);
	DummySprite = GetAsset(Alien);

	// Default values
	Bullet.clear();
	Angles.clear();
	Points.clear();

	FramesCounter = 0;
	ShootRate = 0;
	bDelayed = false;
	bRelease = false;
	bIsInProgress = false;

	switch (CurrentPattern)
	{
		case CIRCLE:
			CreateCirclePattern(false, 100, 300.0f, 1.0f);
		break;

		case CIRCLE_HOLE:
			CreateCirclePattern(true, 100, 300.0f, 1.0f);
		break;

		case CIRCLE_HOLE_LOCK_ON:
			CreateCirclePattern(true, 100, 300.0f, 1.0f);

			if (bDebug)
				AddDebugInitCode();
		break;

		default:
		break;
	}

	for (unsigned short i = 0; i < Bullet.size(); i++)
		Bullet[i].Init();
}

void CirclePattern::Update()
{
	if (bDebug)
		AddDebugSwitchPatternCode();

	switch (CurrentPattern)
	{
		case CIRCLE:
			UpdateCircleBullet(false);
		break;

		case CIRCLE_HOLE:
			UpdateCircleBullet(false);
		break;

		case CIRCLE_HOLE_LOCK_ON:
			UpdateCircleBullet(true);
			AddDebugUpdateCode();
		break;

		default:
		break;
	}

	// Player's bomb mechanic
	if (IsKeyPressed(KEY_B))
		if (Enemy->Player->BombsLeft >= 0 && Enemy->Player->bWasBombUsed)
			DestroyActiveBullets();
}

void CirclePattern::Draw()
{
	if (bDebug)
	{
		RDrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);
		
		switch (CurrentPattern)
		{
			case CIRCLE:
				DrawDebugInfo();
			break;

			case CIRCLE_HOLE:
				DrawDebugInfo();
			break;

			case CIRCLE_HOLE_LOCK_ON:
				DrawDummy();
				DrawDebugInfo();
			break;

			default:
			break;
		}
		
		RDrawText(CirclePatternNames[CurrentPattern-33], 10, 60, 20, WHITE);
		RDrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}

	// Bullets
	if (!Bullet.empty())
		for (int i = 0; i < Bullet.size(); i++)
			Bullet[i].Draw();
}

void CirclePattern::Delay(const float Seconds)
{
	FramesCounter++;

	if (!bDelayed)
	{
		if (Seconds == 0)
			bDelayed = true;
		else
		{
			if (((FramesCounter/(120*Seconds))/2) == 1)
			{
				StartShotRoutine();
				CheckBulletOutsideWindow();
				FramesCounter = 0;
				bDelayed = true;
			
				//SetSoundVolume(GetSound(Fire), 2.0f);
				//RPlaySound(GetSound(Fire));
			}	
		}
	}
	else
	{

		StartShotRoutine();
		CheckBulletOutsideWindow();

		for (unsigned short i = 0; i < Bullet.size(); i++)
			Bullet[i].Update();
	}
}

void CirclePattern::AddDebugSwitchPatternCode()
{
	if (!Bullet.empty() && bRelease)
		bIsInProgress = true;
	else
	{
		bIsInProgress = false;
		bDelayed= false;
	}

	// Switch bullet types with arrow keys when not firing/in progress
	if (!bIsInProgress)
	{
		if (IsKeyPressed(KEY_LEFT))
		{
			if (static_cast<Pattern>(int(CurrentPattern-1)) < CIRCLE)
				SetBulletPattern(static_cast<Pattern>(CIRCLE_HOLE_LOCK_ON));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));

			Init();
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > CIRCLE_HOLE_LOCK_ON)
				SetBulletPattern(static_cast<Pattern>(CIRCLE));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern+1)));

			Init();
		}
	}
}

void CirclePattern::DrawDebugInfo()
{
	DrawDebugPoints(unsigned short(Bullet.size()));
	RDrawText(FormatText("Ways: %0i", unsigned short(Bullet.size())), 10, 120, 18, WHITE);
}
