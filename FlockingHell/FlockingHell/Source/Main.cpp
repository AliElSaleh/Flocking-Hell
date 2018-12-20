#include "Main.h"

int main()
{
	Init();
	
	// Main game loop
	while (!WindowShouldClose())
	{
		Update();
		Draw();
	}

	UnloadTextures();

	CloseWindow();

    return 0;
}

// Function Definitions
//-----------------------------------------

void Init()
{
	// Window setup
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flocking Hell");
	SetTargetFPS(120);

	SetGameState(GAME);

	InitGame();
}

void InitGame()
{
	// Initialise Player
	Player.Init();
	Player.GameState = &GameState;

	RedBullet = LoadTexture("Sprites/RedBullet.png");

	Bullets.reserve(WAVE_ONE_BULLETS + WAVE_TWO_BULLETS);

	// Initialise enemy bullets
	for (int i = 0; i < WAVE_ONE_BULLETS; i++)
	{
		Bullet[i].Player = &Player;
		Bullet[i].Texture = RedBullet;
		Bullet[i].InitArray(i);
		Bullets.push_back(Bullet[i]);
	}

	BulletWave.push_back(Bullets);
	Bullets.erase(Bullets.begin(), Bullets.end());

	for (int i = 0; i < WAVE_TWO_BULLETS; i++)
	{
		Bullet[i].Player = &Player;
		Bullet[i].Texture = RedBullet;
		Bullet[i].InitArray(i);
		Bullet[i].InitWave(2);
		Bullets.push_back(Bullet[i]);
	}

	BulletWave.push_back(Bullets);

	// Initialise stars
	for (int i= 0; i <MAX_SMALL_STARS; i++)
		SmallStars[i].Init();

	bShowDebugHit = true;
	bDebug = false;
	Player.bDebug = true;
}

void SetGameState(const State State)
{
	GameState = State;
}

void Update()
{
	switch (GameState)
	{
		case SPLASH:
			UpdateSplashScreen();
		break;

		case MENU:
			UpdateMenuScreen();
		break;

		case GAME:
			UpdateGame();
		break;

		case PAUSE:
			UpdatePauseScreen();
		break;

		case DEATH:
			UpdateDeathScreen();
		break;

		case WIN:
			UpdateWinScreen();
		break;

		case LOSE:
			UpdateLoseScreen();
		break;

		case STATS:
			UpdateStatsScreen();
		break;

		case HIGHSCORE:
			UpdateHighscoreScreen();
		break;

		default:
		break;
	}
}

void Draw()
{
	BeginDrawing();

	switch (GameState)
	{
		case SPLASH:
			DrawSplashScreen();
		break;

		case MENU:
			DrawMenuScreen();
		break;

		case GAME:
			DrawGame();
		break;

		case PAUSE:
			DrawPauseScreen();
		break;

		case DEATH:
			DrawDeathScreen();
		break;

		case WIN:
			DrawWinScreen();
		break;

		case LOSE:
			DrawLoseScreen();
		break;

		case STATS:
			DrawStatsScreen();
		break;

		case HIGHSCORE:
			DrawHighscoreScreen();
		break;

		default:
		break;
	}

	EndDrawing();
}

void UpdateSplashScreen()
{
	FramesCounter++;

	if (((FramesCounter/360)%2) == 1)
		SetGameState(MENU);
}

void UpdateMenuScreen()
{	
	if (IsKeyPressed(KEY_ENTER))
	{
		ResetGame();
		SetGameState(GAME);
	}

	SetExitKey(KEY_ESCAPE);
}

void UpdateGame()
{
	FramesCounter++;
	SetExitKey(KEY_ESCAPE);
	HideCursor();

	// Player
	Player.Update();

	// Enemy Bullets
	if (!BulletWave[0][0].IsOutsideWindow())
		for (int i = 0; i < WAVE_ONE_BULLETS; i++)
			BulletWave[0][i].Update();

	if (BulletWave[0][0].IsLocationYGreaterThan(500))
		for (int i = 0; i < WAVE_TWO_BULLETS; i++)
			BulletWave[1][i].Update();

	// Loop bullet waves (currently 2 arrays of bullets)
	if (BulletWave[0][0].IsOutsideWindow() && BulletWave[1][0].IsOutsideWindow())
	{
		for (int i = 0; i < WAVE_ONE_BULLETS; i++)
			BulletWave[0][i].ResetBullet();

		for (int i = 0; i < WAVE_TWO_BULLETS; i++)
			BulletWave[1][i].ResetBullet();
	}


	// Stars
	for (int i = 0; i < MAX_SMALL_STARS; i++)
		SmallStars[i].Update();


	if (IsKeyPressed(KEY_ESCAPE))
		SetGameState(PAUSE);
}

void UpdatePauseScreen()
{
	if (IsKeyPressed(KEY_ESCAPE))
		SetGameState(GAME);
}

void UpdateDeathScreen()
{
	SetExitKey(0);

	if (IsKeyPressed(KEY_ENTER))
	{
		ResetGame();
		SetGameState(GAME);
	}

	if (IsKeyPressed(KEY_ESCAPE))
	{
		ResetGame();
		SetGameState(MENU);
	}
}

void UpdateWinScreen()
{
}

void UpdateLoseScreen()
{
}

void UpdateStatsScreen()
{
}

void UpdateHighscoreScreen()
{
}

void DrawSplashScreen()
{
	ClearBackground(WHITE);

	DrawText("Splash Screen!!", SCREEN_WIDTH/2 - MeasureText("Splash Screen!!", 20), SCREEN_HEIGHT/2 - MeasureText("Splash Screen!!", 5), 40, RED);
}

void DrawGame()
{
	ClearBackground(LIGHTBLACK);
	
	DrawGameUI();

	// Stars
	for (int i = 0; i < MAX_SMALL_STARS; i++)
		SmallStars[i].Draw();

	// Player sprite
	Player.Draw();

	// Enemy bullets
	if (!BulletWave[0][0].IsOutsideWindow())
		for (int i = 0; i < WAVE_ONE_BULLETS; i++)
			BulletWave[0][i].Draw();
	
	if (BulletWave[0][0].IsLocationYGreaterThan(500))
		for (int i = 0; i < WAVE_TWO_BULLETS; i++)
			BulletWave[1][i].Draw();
		

	if (bDebug)
		DrawDebugInfo();
}

void DrawGameUI()
{
	DrawText(FormatText("Health: %02i", Player.Health), 10, 10, 20, GREEN);
}

void DrawMenuScreen()
{
	ClearBackground(WHITE);

	DrawText("Press [Enter] to play", SCREEN_WIDTH/2 - MeasureText("Press [Enter] to play", 10), SCREEN_HEIGHT/2 - 30, 20, RED);
	DrawText("Press [ESC] to quit", SCREEN_WIDTH/2 - MeasureText("Press [Enter] to quit", 10), SCREEN_HEIGHT/2, 20, RED);
}

void DrawPauseScreen()
{
	ClearBackground(WHITE);

	DrawText("Game Paused", SCREEN_WIDTH/2 - MeasureText("Game Paused", 10), SCREEN_HEIGHT/2 - 30, 20, RED);
}

void DrawDeathScreen()
{
	ClearBackground(WHITE);

	DrawText("YOU FAILED", SCREEN_WIDTH/2 - MeasureText("YOU FAILED", 39), SCREEN_HEIGHT/2 - MeasureText("YOU FAILED", 20), 80, RED);

	DrawText("Press [Enter] to try again", SCREEN_WIDTH/2 - MeasureText("Press [Enter] to try again", 10), SCREEN_HEIGHT/2 - 30, 20, RED);
	DrawText("Press [ESC] to enter main menu", SCREEN_WIDTH/2 - MeasureText("Press [ESC] to enter main menu", 10), SCREEN_HEIGHT/2, 20, RED);
}

void DrawWinScreen()
{
}

void DrawLoseScreen()
{
}

void DrawStatsScreen()
{
}

void DrawHighscoreScreen()
{
}

void DrawDebugInfo()
{
	// DEBUG enemy bullet hit with player
	for (int i = 0; i < WAVE_ONE_BULLETS; i++)
	{
		if (!Bullet[i].bActive && Bullet[i].bIsHit && bShowDebugHit)
		{	
			DrawText("Hit", 10, 80, 20, RED);
			Bullet[i].bIsHit = false;
		}
	}

	// DEBUG frames counter
	DrawText(FormatText("%i", FramesCounter), 10, 50, 20, ORANGE);
}

void ResetGame()
{
	if (!BulletWave.empty())
		BulletWave.erase(BulletWave.begin(), BulletWave.end());

	if (!Bullets.empty())
		Bullets.erase(Bullets.begin(), Bullets.end());
	
	InitGame();
}

void UnloadTextures()
{
	UnloadTexture(Player.PlayerSprite);
	UnloadTexture(Player.BulletSprite);
}