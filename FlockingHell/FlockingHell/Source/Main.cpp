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
/// Initialise application
void Init()
{
	// Window setup
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flocking Hell");
	SetTargetFPS(120);

	GameState = GAME;

	InitGame();
}

void InitGame()
{
	Player.Init();
	Player.GameState = &GameState;

	// Bullet setup
	for (int i = 0; i < MAX_SMALL_BULLETS; i++)
	{
		Bullet[i].Player = &Player;
		Bullet[i].InitArray(i);
		BulletLocation[i] = Bullet[i].Location;
	}

	// Star setup
	for (int i= 0; i <MAX_SMALL_STARS; i++)
	{
		SmallStars[i].Location.x = float(GetRandomValue(0, SCREEN_WIDTH));
		SmallStars[i].Location.y = float(GetRandomValue(0, SCREEN_HEIGHT));
		SmallStars[i].Radius = 1.0f;
		SmallStars[i].Speed = 100.0f;
	}

	bShowDebugHit = true;
	bDebug = false;
}

/// Update game logic
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

/// Render objects
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

void UpdateStars()
{
	for (int i = 0; i < MAX_SMALL_STARS; i++)
		SmallStars[i].Location.y += SmallStars[i].Speed * GetFrameTime();
}

void CheckStarsOutsideWindow()
{
	for (int i = 0; i < MAX_SMALL_STARS; i++)
	{
		if (SmallStars[i].Location.y - SmallStars[i].Radius > GetScreenHeight())
			SmallStars[i].Location.y = SmallStars[i].Radius;
	}
}

void UpdateSplashScreen()
{
	FramesCounter++;

	if (((FramesCounter/360)%2) == 1)
		GameState = MENU;
}

void DrawSplashScreen()
{
	ClearBackground(WHITE);

	DrawText("Splash Screen!!", SCREEN_WIDTH/2 - MeasureText("Splash Screen!!", 20), SCREEN_HEIGHT/2 - MeasureText("Splash Screen!!", 5), 40, RED);
}

void UpdateGame()
{
	FramesCounter++;
	SetExitKey(KEY_ESCAPE);

	// Player
	Player.Update();


	// Bullet
	for (int i = 0; i < MAX_SMALL_BULLETS; i++)
		Bullet[i].Update();


	// Stars
	UpdateStars();
	CheckStarsOutsideWindow();


	if (IsKeyPressed(KEY_ESCAPE))
		GameState = PAUSE;
}

void DrawGame()
{
	ClearBackground(LIGHTBLACK);
	
	//------------------------------------------------------------------------------------------------------------------------------------
	DrawGameUI();

	DrawStars();

	for (int i = 0; i < MAX_SMALL_BULLETS; i++)
		Bullet[i].Draw();

	Player.Draw();

	if (bDebug)
		DrawDebugInfo();
	//------------------------------------------------------------------------------------------------------------------------------------
}

void UpdateMenuScreen()
{	
	if (IsKeyPressed(KEY_ENTER))
	{
		ResetGame();
		GameState = GAME;
	}

	SetExitKey(KEY_ESCAPE);
}

void DrawMenuScreen()
{
	ClearBackground(WHITE);

	DrawText("Press [Enter] to play", SCREEN_WIDTH/2 - MeasureText("Press [Enter] to play", 10), SCREEN_HEIGHT/2 - 30, 20, RED);
	DrawText("Press [ESC] to quit", SCREEN_WIDTH/2 - MeasureText("Press [Enter] to quit", 10), SCREEN_HEIGHT/2, 20, RED);
}

void UpdatePauseScreen()
{
	if (IsKeyPressed(KEY_ESCAPE))
		GameState = GAME;
}

void DrawPauseScreen()
{
	ClearBackground(WHITE);

	DrawText("Game Paused", SCREEN_WIDTH/2 - MeasureText("Game Paused", 10), SCREEN_HEIGHT/2 - 30, 20, RED);
}

void UpdateDeathScreen()
{
	SetExitKey(0);

	if (IsKeyPressed(KEY_ENTER))
	{
		ResetGame();
		GameState = GAME;
	}

	if (IsKeyPressed(KEY_ESCAPE))
	{
		ResetGame();
		GameState = MENU;
	}
}

void DrawDeathScreen()
{
	ClearBackground(WHITE);

	DrawText("YOU FAILED", SCREEN_WIDTH/2 - MeasureText("YOU FAILED", 39), SCREEN_HEIGHT/2 - MeasureText("YOU FAILED", 20), 80, RED);

	DrawText("Press [Enter] to try again", SCREEN_WIDTH/2 - MeasureText("Press [Enter] to try again", 10), SCREEN_HEIGHT/2 - 30, 20, RED);
	DrawText("Press [ESC] to enter main menu", SCREEN_WIDTH/2 - MeasureText("Press [ESC] to enter main menu", 10), SCREEN_HEIGHT/2, 20, RED);
}

void UpdateWinScreen()
{
}

void DrawWinScreen()
{
}

void UpdateLoseScreen()
{
}

void DrawLoseScreen()
{
}

void UpdateStatsScreen()
{
}

void DrawStatsScreen()
{
}

void UpdateHighscoreScreen()
{
}

void DrawHighscoreScreen()
{
}

void ResetGame()
{
	InitGame();
}

void DrawGameUI()
{
	//DrawFPS(10, 10);
	DrawText(FormatText("Health: %02i", Player.Health), 10, 10, 20, GREEN);
}

void DrawStars()
{
	// Small Stars
	for (int i = 0; i < MAX_SMALL_STARS; i++)
		DrawCircle(int(SmallStars[i].Location.x), int(SmallStars[i].Location.y), SmallStars[i].Radius, WHITE);
}

void DrawDebugInfo()
{
	// DEBUG enemy bullet hit with player
	for (int i = 0; i < MAX_SMALL_BULLETS; i++)
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

void UnloadTextures()
{
	UnloadTexture(Player.Texture);
}
