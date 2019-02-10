#include "Globals.h"
#include "Assets.h"
#include "Main.h"

#include <fstream>
#include <iostream>
#include <string>

#pragma comment (lib, "ws2_32.lib")

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)
#define GetFont(Name) ASSETS.GetFont(#Name)
#define GetSound(Name) ASSETS.GetSound(#Name)
#define GetMusic(Name) ASSETS.GetMusic(#Name)

int main()
{
	Init();
	
	// Main game loop
	while (!WindowShouldClose())
	{
		Update();
		Draw();
	}

	Unload();

	CloseAudioDevice();

	closesocket(Socket);
	WSACleanup();

	RCloseWindow();

    return 0;
}

// Function Definitions
//-----------------------------------------

void Init()
{
	// Window setup
	const unsigned short SCREEN_WIDTH = 700+PANEL_WIDTH; // From 700 to 1050 on X axis is reserved for player stats
	const unsigned short SCREEN_HEIGHT = 800;
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cosmic Hell");
	SetTargetFPS(120);

	InitAudioDevice();

	SetGameState(GAME);

	ASSETS.LoadSprites();
	ASSETS.LoadFonts();
	ASSETS.LoadSounds();
	ASSETS.LoadMusic();

	LoadHighscore();

	//bConnected = ConnectToServer();

	InitGame();
}

void InitGame()
{
	switch (GameState)
	{
		case SPLASH:
			SplashLogo.Init();
		break;

		case MENU:
			MenuMusic = GetMusic(Menu);
			GameMusic = GetMusic(DemonStage);
			PlayMusicStream(MenuMusic);
			SetMusicVolume(MenuMusic, 0.4f);
			
			// Initialise sprites
			Background = GetAsset(MainMenu);
			MenuTitle = GetAsset(CosmicHellTitle);

			TitleLocation = {287.0f, 206.0f};

			// Initialise planets
			for (int i = 0; i < 5; ++i)
			{
				BigPlanet[i].Init();
				BigPlanet[i].Sprite = GetAsset(Planet);
			}

			BigPlanet[0].Location = {52.0f, 68.0f};
			BigPlanet[1].Location = {231.0f, 204.0f};
			BigPlanet[2].Location = {584.0f, 99.0f};
			BigPlanet[3].Location = {432.0f, 558.0f};
			BigPlanet[4].Location = {876.0f, 272.0f};

			for (int i = 0; i < 2; ++i)
			{
				RingPlanet[i].Init();
				RingPlanet[i].Sprite = GetAsset(RingPlanet);
			}

			RingPlanet[0].Location = {250.0f, 458.0f};
			RingPlanet[1].Location = {800.0f, 85.0f};

			FramesCounter = 0;
			MenuFramesCounter = 0;
			FadeAlpha = 1.0f;
			MusicFade = 0.5f;

			bMusicFaded = false;
			bBegan = false;	
			SplashLogo.bFinished = true;
		break;

		case OPTIONS:
			// Initialise sprites
			MenuMusic = GetMusic(Menu);
			GameMusic = GetMusic(DemonStage);
			PlayMusicStream(MenuMusic);
			SetMusicVolume(MenuMusic, 0.4f);
			Background = GetAsset(OptionsMenu);
			Selector = GetAsset(Selector);

			TitleLocation = {287.0f, 106.0f};

			SelectorLocation = StartLocation;

			Index = 0;

			FramesCounter = 0;
			MenuFramesCounter = 0;
			FadeAlpha = 0.0f;
			MusicFade = 0.5f;

			bMusicFaded = false;
			bBegan = false;	
			SplashLogo.bFinished = true;
		break;

		case GAME:
			MenuMusic = GetMusic(Menu);
			GameMusic = GetMusic(DemonStage);
			PlayMusicStream(GameMusic);
			SetMusicVolume(GameMusic, 0.5f);

			MusicFade = 0.5f;

			// Initialise Player
			Player.Init();
			Player.GameState = &GameState;

			SendData();
			
			// Initialise enemy manager system
			for (int i = 0; i < EnemyManager.Enemies.size(); i++)
			{
				EnemyManager.Enemies[i]->Player = &Player;
				EnemyManager.Enemies[i]->Init();
			}

			for (int i = 0; i < EnemyManager.FlockOfEnemies.size(); i++)
			{
				EnemyManager.FlockOfEnemies[i]->Player = &Player;				
				EnemyManager.FlockOfEnemies[i]->Init();
			}

			// Initialise stars
			for (int i = 0; i < MAX_SMALL_STARS; i++)
				SmallStars[i].Init();

			for (int i = 0; i < MAX_CLOSE_STARS/2; i++)
			{
				CloseStars[i].Init();
				CloseStars[i].Speed = 300.0f;
			}
			
			for (int i = 30; i < MAX_CLOSE_STARS; i++)
			{
				CloseStars[i].Init();
				CloseStars[i].Speed = 200.0f;
			}

			for (int i = 0; i < MAX_FAR_STARS; i++)
			{
				FarStars[i].Init();
				FarStars[i].Speed = 10.0f;
			}

			// Initialise rplanets
			for (int i = 0; i < 5; ++i)
			{
				BigPlanet[i].Init();
				BigPlanet[i].Sprite = GetAsset(Planet);
			}

			BigPlanet[0].Location = {52.0f, 68.0f};
			BigPlanet[1].Location = {231.0f, 204.0f};
			BigPlanet[2].Location = {584.0f, 99.0f};
			BigPlanet[3].Location = {432.0f, 558.0f};
			BigPlanet[4].Location = {876.0f, 272.0f};

			for (int i = 0; i < 2; ++i)
			{
				RingPlanet[i].Init();
				RingPlanet[i].Sprite = GetAsset(RingPlanet);
				RingPlanet[i].Speed = 40.0f;
			}

			RingPlanet[0].Location = {250.0f, 458.0f};
			RingPlanet[1].Location = {800.0f, 85.0f};

			// Initialise sprites
			Panel = GetAsset(Panel);

			FramesCounter = 0;
			FadeAlpha = 1.0f;

			bMusicFaded = false;
			bDebug = false;
		break;

		case DEATH:
			FramesCounter = 7199;
		break;

		case CREDITS:
			Credits.Init();
			Credits.AddName("Ali El Saleh", Category::PROGRAMMING_ART);
			Credits.AddName("Carpenter Brut", Category::MUSIC);
		break;

		default:
		break;
	}
}

void SetGameState(const State State)
{
	GameState = State;
}

void Update()
{
	SetWindowTitle(FormatText("Cosmic Hell | FPS: %02i", GetFPS()));

	switch (GameState)
	{
		case SPLASH:
			UpdateSplashScreen();
		break;

		case MENU:
			UpdateMenuScreen();
		break;

		case OPTIONS:
			UpdateOptionsScreen();
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

		case LEADERBOARD:
			UpdateLeaderboardScreen();
		break;

		case STATS:
			UpdateStatsScreen();
		break;

		case CREDITS:
			UpdateCreditsScreen();
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

		case OPTIONS:
			DrawOptionsScreen();
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

		case LEADERBOARD:
			DrawLeaderboardScreen();
		break;

		case STATS:
			DrawStatsScreen();
		break;

		case CREDITS:
			DrawCreditsScreen();
		break;

		default:
		break;
	}

	EndDrawing();
}

void UpdateSplashScreen()
{
	SetExitKey(0);

	SplashLogo.Update();

	if (SplashLogo.bFinished)
	{
	    FramesCounter++;

		if (((FramesCounter/240)%2) == 1)
		{
			FramesCounter = 0;
			SetGameState(MENU);
			InitGame();
		}
	}
}

void UpdateMenuScreen()
{	
	SetExitKey(0);

	UpdateMusicStream(MenuMusic);
	StopMusicStream(GameMusic);

	MenuFramesCounter++;

	// Any key press functionality
	for (int i = KEY_A; i < KEY_Z; i++)
		if (IsKeyPressed(i))
		{
			RPlaySound(GetSound(MenuSelect));
			
			if (!bBegan)
				bBegan = true;

			FramesCounter = 0;
			FadeAlpha -= 0.0f;
		}

	if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
	{
		if (!bBegan)
			bBegan = true;

		FramesCounter = 0;
		FadeAlpha -= 0.0f;
	}

	if (bBegan)
	{
		GameState = OPTIONS;
		FramesCounter = 0;
		FadeAlpha = 0.0f;
		bBegan = false;
		InitGame();
	}

	// Fade in from splash screen
	else
	{
		FramesCounter++;
	
		FadeAlpha -= 0.01f;
	
		if (FadeAlpha <= 0)
			FadeAlpha = 0.0f;
	}

	// Move planets in the background
	for (int i = 0; i < 5; ++i)
		BigPlanet[i].Update();

	for (int i = 0; i < 2; ++i)
		RingPlanet[i].Update();
}

void UpdateOptionsScreen()
{
	SetExitKey(0);

	UpdateMusicStream(MenuMusic);
	StopMusicStream(GameMusic);

	// Move through menu items using mouse
	if (GetMousePosition().x > 359.0f && GetMousePosition().y > 388.0f && 
		GetMousePosition().x < 685.0f && GetMousePosition().y < 690.0f)
	{
		if (!bSoundPlayed[0])
		{
			bSoundPlayed[0] = true;
			RPlaySound(GetSound(Switch));
		}

		Index = 0;
		SelectorLocation = StartLocation;
	}
	else
		bSoundPlayed[0] = false;

	if (GetMousePosition().x > 359.0f && GetMousePosition().y > 448.0f && 
		GetMousePosition().x < 685.0f && GetMousePosition().y < 690.0f)
	{
		if (!bSoundPlayed[1])
		{
			bSoundPlayed[1] = true;
			RPlaySound(GetSound(Switch));
		}

		Index = 1;
		SelectorLocation.y = StartLocation.y + 60;
	}
	else
		bSoundPlayed[1] = false;

	if (GetMousePosition().x > 359.0f && GetMousePosition().y > 508.0f &&
		GetMousePosition().x < 685.0f && GetMousePosition().y < 690.0f)
	{
		if (!bSoundPlayed[2])
		{
			bSoundPlayed[2] = true;
			RPlaySound(GetSound(Switch));
		}

		Index = 2;
		SelectorLocation.y = StartLocation.y + 120;
	}
	else
		bSoundPlayed[2] = false;

	if (GetMousePosition().x > 359.0f && GetMousePosition().y > 568.0f && 
		GetMousePosition().x < 685.0f && GetMousePosition().y < 690.0f)
	{
		if (!bSoundPlayed[3])
		{
			bSoundPlayed[3] = true;
			RPlaySound(GetSound(Switch));
		}

		Index = 3;
		SelectorLocation.y = StartLocation.y + 180;
	}
	else
		bSoundPlayed[3] = false;

	if (GetMousePosition().x > 359.0f && GetMousePosition().y > 628.0f && 
		GetMousePosition().x < 685.0f && GetMousePosition().y < 690.0f)
	{
		if (!bSoundPlayed[4])
		{
			bSoundPlayed[4] = true;
			RPlaySound(GetSound(Switch));
		}

		Index = 4;
		SelectorLocation = EndLocation;
	}
	else
		bSoundPlayed[4] = false;

	// Move up through the menu items using keyboard
	if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
	{
		RPlaySound(GetSound(Switch));
		
		Index--;
		SelectorLocation.y -= JumpAmount;

		if (Index < 0)
		{
			Index = 4;
			SelectorLocation = EndLocation;
		}
	}

	// Move down through the menu items using keyboard
	if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
	{
		RPlaySound(GetSound(Switch));

		Index++;
		SelectorLocation.y += JumpAmount;

		if (Index > 4)
		{
			Index = 0;
			SelectorLocation = StartLocation;
		}
	}

	// Handle user input - keyboard or mouse
	if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || 
		IsMouseButtonReleased(0) && GetMousePosition().x > 359.0f && GetMousePosition().x < 685.0f && 
									GetMousePosition().y > 388.0f && GetMousePosition().y < 690.0f)
		switch (Index)
		{
		case 0: // Start Game
			RPlaySound(GetSound(Select));
			if (!bBegan)
				bBegan = true;
			break;

		case 1: // Leaderboards
			RPlaySound(GetSound(Select));
			GameState = LEADERBOARD;
			InitGame();
			break;

		case 2: // Stats
			RPlaySound(GetSound(Select));
			GameState = STATS;
			InitGame();
			break;

		case 3: // Credits
			RPlaySound(GetSound(Select));
			GameState = CREDITS;
			InitGame();
			break;

		case 4: // Quit
			Unload();

			CloseAudioDevice();

			closesocket(Socket);
			WSACleanup();

			RCloseWindow();

			exit(0);
		default:
			break;
		}

	// Fade out to black on key press and transition into the game state
	if (bBegan)
	{	
		FramesCounter++;
		
		FadeAlpha += 0.01f;

		MusicFade -= 0.004f;
		SetMusicVolume(MenuMusic, MusicFade);

		if (MusicFade < 0.0f)
			MusicFade = 0.0f;
		
		if (FadeAlpha >= 1.0f)
			FadeAlpha = 1.0f;
		
		if (FramesCounter/310%2)
		{
			FramesCounter = 0;

			SetGameState(GAME);
			EnemyManager.Reset();
			EnemyManager.Init();
			InitGame();
		}
	}
	else
	{
		FramesCounter++;
	
		FadeAlpha -= 0.01f;
	
		if (FadeAlpha <= 0)
			FadeAlpha = 0.0f;
	}

	// Move planets in the background
	for (int i = 0; i < 5; ++i)
		BigPlanet[i].Update();

	for (int i = 0; i < 2; ++i)
		RingPlanet[i].Update();
}

void UpdateGame()
{
	FramesCounter++;
	SetExitKey(0);
	HideCursor();
	
	UpdateMusicStream(GameMusic);
	StopMusicStream(MenuMusic);

	if (GetMousePosition().x > GetScreenWidth()-PANEL_WIDTH)
		RShowCursor();

	FadeAlpha -= 0.03f;

	if (FadeAlpha <= 0.0f)
		FadeAlpha = 0.0f;

	// Stars
	for (int i = 0; i < MAX_FAR_STARS; i++)
		FarStars[i].Update();

	for (int i = 0; i < MAX_SMALL_STARS; i++)
		SmallStars[i].Update();

	// Planets
	for (int i = 0; i < 5; i++)
		BigPlanet[i].UpdateY();

	for (int i = 0; i < 5; i++)
		RingPlanet[i].UpdateY();

	// Stars
	for (int i = 0; i < MAX_CLOSE_STARS; i++)
		CloseStars[i].Update();

	// Player
	Player.Update();
	switch (Player.BossKilled)
	{
	case 1:
		if (Player.bChangeMusic)
		{
			// Fade current music
			if (!bMusicFaded)
			{
				MusicFade -= 0.004f;
				SetMusicVolume(GameMusic, MusicFade);

				if (MusicFade < 0.0f)
				{
					MusicFade = 0.0f;
					bMusicFaded = true;
				}
			}
			else
			{
				// Change music when fade is complete
				GameMusic = GetMusic(AlienStage);
				PlayMusicStream(GameMusic);
				Player.bChangeMusic = false;
				MusicFade = 0.5f;
				bMusicFaded = false;
			}
		}
		break;

	case 2:
		if (Player.bChangeMusic)
		{
			// Fade current music
			if (!bMusicFaded)
			{
				MusicFade -= 0.004f;
				SetMusicVolume(GameMusic, MusicFade);

				if (MusicFade < 0.0f)
				{
					MusicFade = 0.0f;
					bMusicFaded = true;
				}
			}
			else
			{
				// Change music when fade is complete
				GameMusic = GetMusic(WarshipStage);
				PlayMusicStream(GameMusic);
				Player.bChangeMusic = false;
				bMusicFaded = false;
			}
		}
		break;

	default:
		break;
	}

	if (Player.bIsDead)
		InitGame();

	// Enemy Manager system
	EnemyManager.Update();

	if (IsKeyPressed(KEY_ESCAPE))
		SetGameState(PAUSE);
}

void UpdatePauseScreen()
{
	DisableCursor();

	if (IsKeyPressed(KEY_Q))
	{
		SetGameState(OPTIONS);
		EnemyManager.Reset();
		EnemyManager.Init();
		EnableCursor();
		InitGame();
	}
	
	if (IsKeyPressed(KEY_ESCAPE))
		SetGameState(GAME);
}

void UpdateDeathScreen()
{
	SetExitKey(0);

	StopMusicStream(GameMusic);

	if (IsKeyPressed(KEY_ENTER))
	{
		SetGameState(GAME);
		EnemyManager.Reset();
		EnemyManager.Init();
		InitGame();
	}

	if (IsKeyPressed(KEY_ESCAPE))
	{
		SetGameState(OPTIONS);
		InitGame();
	}

	TotalScore = Player.Score + Player.GrazingScore;

	if (ScoreCounter < TotalScore)
		ScoreCounter += 2 * 10;
	else
		ScoreCounter = TotalScore;

	// Count the score first then try to connect to server
	if (ScoreCounter == TotalScore)
	{
		FramesCounter++;

		if (!bConnected)
		{
			// Try reconnecting to the server every minute if we are not connected
			if (FramesCounter/ReconnectDelay%2)
			{
				FramesCounter = 0;
				bConnected = ConnectToServer();
			}
		}
	}

	// Skip score counting by pressing any key
	for (int i = KEY_A; i < KEY_Z; i++)
		if (IsKeyPressed(i))
			ScoreCounter = TotalScore;

	// Highscore check
	if (Highscore < TotalScore)
	{
		Highscore = TotalScore;

		SaveHighscore();
		
		if (bConnected)
			SendData();
	}
}

void UpdateWinScreen()
{
	FramesCounter++;
	SetExitKey(0);

	if (IsKeyPressed(KEY_ENTER))
	{
		SetGameState(CREDITS);
		EnemyManager.Reset();
		InitGame();
	}

	if (IsKeyPressed(KEY_ESCAPE))
	{
		SetGameState(OPTIONS);
		EnemyManager.Reset();
		EnemyManager.Init();
		InitGame();
	}

	if (!bConnected)
	{
		// Try reconnecting to the server every minute if we are not connected
		if (FramesCounter/ReconnectDelay%2)
		{
			FramesCounter = 0;
			bConnected = ConnectToServer();
		}
	}
}

void UpdateLeaderboardScreen()
{
	FramesCounter++;
	SetExitKey(0);

	UpdateMusicStream(MenuMusic);
	StopMusicStream(GameMusic);

	if (IsKeyPressed(KEY_ESCAPE))
	{
		GameState = OPTIONS;
		InitGame();
	}

	if (!bConnected)
	{
		if (!bEntered)
		{
			bEntered = true;
			//bConnected = ConnectToServer();
		}

		// Try reconnecting to the server every minute if we are not connected
		if (FramesCounter/ReconnectDelay%2)
		{
			FramesCounter = 0;
			bConnected = ConnectToServer();
		}
	}
}

void UpdateStatsScreen()
{
	SetExitKey(0);

	UpdateMusicStream(MenuMusic);
	StopMusicStream(GameMusic);

	if (IsKeyPressed(KEY_ESCAPE))
	{
		GameState = OPTIONS;
		InitGame();
	}
}

void UpdateCreditsScreen()
{
	SetExitKey(0);

	StopMusicStream(GameMusic);
	StopMusicStream(MenuMusic);

	if (IsKeyPressed(KEY_ESCAPE))
	{
		GameState = OPTIONS;
		InitGame();
	}

	Credits.Update();

	if (Credits.bFinished)
	{
		GameState = OPTIONS;
		InitGame();
	}
}

void DrawSplashScreen()
{
	ClearBackground(WHITE);

	SplashLogo.Draw();
}

void DrawMenuScreen()
{
	// Space background
	DrawTexture(Background, 0, 0, WHITE);

	// Planets
	for (int i = 0; i < 5; i++)
		BigPlanet[i].Draw();

	for (int i = 0; i < 2; i++)
		RingPlanet[i].Draw();

	// Title
	DrawTexture(MenuTitle, int(TitleLocation.x), int(TitleLocation.y), WHITE);

	// PRESS ANY KEY
	 if ((MenuFramesCounter/60)%2)
		 DrawTexture(GetAsset(PressAnyKey), 353, 665, WHITE);

	// Fade screen out on key press
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, FadeAlpha));
}

void DrawOptionsScreen()
{
	// Background
	DrawTexture(Background, 0, 0, WHITE);

	// Planets
	for (int i = 0; i < 5; i++)
		BigPlanet[i].Draw();

	for (int i = 0; i < 2; i++)
		RingPlanet[i].Draw();

	// Title
	DrawTexture(MenuTitle, int(TitleLocation.x), int(TitleLocation.y), WHITE);

	// Selector
	DrawTexture(Selector, SelectorLocation.x, SelectorLocation.y, WHITE);

	// Menu items
	// Start game
	DrawTexture(GetAsset(StartGame), 393.0f, 407.0f, WHITE);

	// Leaderboards
	DrawTexture(GetAsset(Leaderboard), 371.0f, 466.8f, WHITE);

	// Stats
	DrawTexture(GetAsset(Stats), 457.0f, 527.8f, WHITE);

	// Credits
	DrawTexture(GetAsset(Credits), 438.0f, 588.0f, WHITE);

	// Quit
	DrawTexture(GetAsset(Quit), 480.0f, 645.8f, WHITE);

	// Fade screen out on key press
	if (bBegan)
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, FadeAlpha));
}

void DrawGame()
{
	// Space background
	DrawTexture(GetAsset(GameBackground), 0, 0, GRAY);

	// Stars
	for (int i = 0; i < MAX_SMALL_STARS; i++)
		SmallStars[i].Draw();

	for (int i = 0; i < MAX_CLOSE_STARS; i++)
		CloseStars[i].Draw();
	
	for (int i = 0; i < MAX_FAR_STARS; i++)
		FarStars[i].Draw();

	// Planets
	for (int i = 0; i < 5; i++)
		BigPlanet[i].Draw();

	for (int i = 0; i < 2; i++)
		RingPlanet[i].Draw();

	// Player
	Player.Draw();

	// Enemy manager system
	EnemyManager.Draw();

	Player.DrawBullets();

	// Stats panel
	DrawTexture(Panel, GetScreenWidth()-PANEL_WIDTH, 0, WHITE);
 	DrawTexture(GetAsset(Divider), GetScreenWidth()-PANEL_WIDTH-5, 0, WHITE); // Divider
	DrawFPS(GetScreenWidth()-80, GetScreenHeight() - 25);
	DrawGameUI();

	// Fade screen in when entering this state
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, FadeAlpha));
}

void DrawGameUI()
{
	unsigned short Spacing  = 0;

	for (int i = 0; i < Player.Heart.size(); i++)
	{
		DrawTexture(Player.Heart[i], GetScreenWidth()-PANEL_WIDTH + 140 + Spacing, 436, WHITE);
		Spacing += HeartSpriteSpacing;
	}

	Spacing = 0;
	
	for (int i = 0; i < Player.Bomb.size(); i++)
	{
		DrawTexture(Player.Bomb[i], GetScreenWidth()-PANEL_WIDTH + 154 + Spacing, 487, WHITE);
		Spacing += BombSpriteSpacing;
	}

	Spacing = 0;

	unsigned int OutputScore = Highscore;

	for (int i = 0; i < 3; i++)
	{
		if (Highscore > 999999)
		{
			Highscore = 999999;
			OutputScore = Highscore;
		}

		RDrawTextEx(GetFont(VCR), FormatText("%06i", OutputScore), {float(GetScreenWidth()-PANEL_WIDTH+211), 223.0f + Spacing}, 31.0f, 0.0f, WHITE);

		// Grazing
		if (i > 0)
		{
			OutputScore = Player.GrazingScore;
			Spacing = GrazingScorePosition;
		}
		// Score
		else
		{
			OutputScore = Player.Score;
			Spacing = ScorePosition;
		}
	}
}

void DrawPauseScreen()
{
	DrawGame();

	DrawRectangle(0, 0, GetScreenWidth()-PANEL_WIDTH, GetScreenHeight(), Fade(BLACK, 0.7f));

	DrawTexture(GetAsset(PauseState), 0, 0, WHITE);

	// Stats panel
	DrawTexture(Panel, GetScreenWidth()-PANEL_WIDTH, 0, WHITE);
 	DrawTexture(GetAsset(Divider), GetScreenWidth()-PANEL_WIDTH-5, 0, WHITE); // Divider
	DrawFPS(GetScreenWidth()-80, GetScreenHeight() - 25);
	DrawGameUI();
}

void DrawDeathScreen()
{
	DrawTexture(GetAsset(DeathState), 0, 0, WHITE);

	// Highscore
	RDrawTextEx(GetFont(VCR), FormatText("%06i", Highscore), {284.0f, 223.0f}, 31.0f, 0.0f, WHITE);

	// Score
	RDrawTextEx(GetFont(VCR), FormatText("%06i", Player.Score), {284.0f, 325.0f}, 31.0f, 0.0f, WHITE);

	// Grazing
	RDrawTextEx(GetFont(VCR), FormatText("%06i", Player.GrazingScore), {284.0f, 430.0f}, 31.0f, 0.0f, WHITE);

	// Total
	RDrawTextEx(GetFont(VCR), FormatText("%06i", ScoreCounter), {266.0f, 580.0f}, 42.0f, 0.0f, WHITE);

	// Stats panel
	DrawTexture(Panel, GetScreenWidth()-PANEL_WIDTH, 0, WHITE);
 	DrawTexture(GetAsset(Divider), GetScreenWidth()-PANEL_WIDTH-5, 0, WHITE); // Divider
	DrawFPS(GetScreenWidth()-80, GetScreenHeight() - 25);
	DrawGameUI();
}

void DrawWinScreen()
{
	DrawTexture(GetAsset(WinState), 0, 0, WHITE);

	// Highscore
	RDrawTextEx(GetFont(VCR), FormatText("%06i", Highscore), {284.0f, 223.0f}, 31.0f, 0.0f, WHITE);

	// Score
	RDrawTextEx(GetFont(VCR), FormatText("%06i", Player.Score), {284.0f, 325.0f}, 31.0f, 0.0f, WHITE);

	// Grazing
	RDrawTextEx(GetFont(VCR), FormatText("%06i", Player.GrazingScore), {284.0f, 430.0f}, 31.0f, 0.0f, WHITE);

	// Total
	RDrawTextEx(GetFont(VCR), FormatText("%06i", ScoreCounter), {266.0f, 580.0f}, 42.0f, 0.0f, WHITE);

	// Stats panel
	DrawTexture(Panel, GetScreenWidth()-PANEL_WIDTH, 0, WHITE);
 	DrawTexture(GetAsset(Divider), GetScreenWidth()-PANEL_WIDTH-5, 0, WHITE); // Divider
	DrawFPS(GetScreenWidth()-80, GetScreenHeight() - 25);
	DrawGameUI();
}

void DrawLeaderboardScreen()
{
	DrawTexture(GetAsset(LeaderboardScreen), 0, 0, WHITE);
}

void DrawStatsScreen()
{
	DrawTexture(GetAsset(StatsScreen), 0, 0, WHITE);
}

void DrawCreditsScreen()
{
	ClearBackground(BLACK);

	Credits.Draw();
}

void LoadHighscore()
{
	std::ifstream LoadHighscore;
	LoadHighscore.open("Highscore.score", std::ios::in | std::ios::binary);

	if (LoadHighscore.fail())
	{
		std::cerr << "\nERROR: No highscore file found.\n";
		std::cout << "INFO: Creating new file...\n";

		LoadHighscore.open("Highscore.score", std::ios::in | std::ios::binary);

		std::cout << "INFO: File created successfully.\n";
	}

	LoadHighscore.seekg(0, std::ios::end);
	const int Size = int(LoadHighscore.tellg());
	LoadHighscore.seekg(0, std::ios::beg);

	while (LoadHighscore.tellg() < Size)
		LoadHighscore.read(reinterpret_cast<char*>(&Highscore), sizeof(Highscore));
	
	LoadHighscore.close();
}

void SaveHighscore()
{
	system("attrib +h +s -r Highscore.score");

	std::ofstream SaveHighscore;
	SaveHighscore.open("Highscore.score", std::ios::out | std::ios::binary | std::ios::app);

	if (SaveHighscore.fail())
	{
		std::cerr << "ERROR: Unable to save highscore.\n";
		exit(EXIT_FAILURE);
	}

	SaveHighscore.write(reinterpret_cast<char*>(&Highscore), sizeof(Highscore));
	SaveHighscore.close();

	system("attrib +h +s +r Highscore.score");
}

bool ConnectToServer()
{
	std::string IP = "199.192.24.86"; // Address of server
	const int Port = 54000; // Listening port number on server

	// Initialise Win socket
	Version = MAKEWORD(2, 2);
	const int WSResult = WSAStartup(Version, &Data);
	if (WSResult != 0)
	{
		std::cerr << "Can't start Win-sock, Error# " << WSResult << ".\n";
		exit(EXIT_FAILURE);
	}

	// Create socket
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Socket == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Error# " << WSAGetLastError() << "\n";
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	// Fill in hint structure
	Hint.sin_family = AF_INET;
	Hint.sin_port = htons(Port); // htons "Host to network short"
	inet_pton(AF_INET, IP.c_str(), &Hint.sin_addr);

	// Connect to server
	const int ConnectionResult = connect(Socket, reinterpret_cast<sockaddr*>(&Hint), sizeof(Hint));
	if (ConnectionResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Error# " << WSAGetLastError() << "\n";
		closesocket(Socket);
		WSACleanup();
		return false;
	}

	return true;
}

void SendData()
{
	DataToSend = std::to_string(Highscore);

	send(Socket, Player.Name.c_str(), int(Player.Name.length()), 0);
	send(Socket, DataToSend.c_str(), int(DataToSend.length()) + 1, 0);
}

void Unload()
{
	ASSETS.UnloadSprites();
	ASSETS.UnloadFonts();
	ASSETS.UnloadSounds();
	ASSETS.UnloadMusic();
}
