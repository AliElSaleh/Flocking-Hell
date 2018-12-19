#pragma once

#include "raylib.h"
#include "Player.h"
#include "Bullet.h"
#include "Star.h"
#include "State.h"
#include <vector>


// Variable Declarations
//-----------------------------------------
// Structs
struct Player Player;
struct Bullet Bullet[WAVE_ONE_BULLETS + WAVE_TWO_BULLETS];
struct Star SmallStars[MAX_SMALL_STARS];

// vectors
std::vector<struct Bullet> Bullets;
std::vector<std::vector<struct Bullet>> BulletWave;

// enums
enum State GameState;

// ints
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 800;
int FramesCounter = 0;

// bools
bool bShowDebugHit = false;
bool bDebug = false;


// Function Declarations
//-----------------------------------------
void Init(); // Initialise application
void InitGame(); // Create instances of objects and initialise their member variables
void SetGameState(State state); // Sets the next frame's current game state to the specified state

void Update(); // Update logic depending on the current game state
void Draw(); // Render graphics depending on the current game state

void UpdateSplashScreen();
void UpdateMenuScreen();
void UpdateGame(); // Game Update loop
void UpdatePauseScreen();
void UpdateDeathScreen();
void UpdateWinScreen();
void UpdateLoseScreen();
void UpdateStatsScreen();
void UpdateHighscoreScreen();

void DrawSplashScreen();
void DrawGame(); // Game Render loop
void DrawGameUI();
void DrawMenuScreen();
void DrawPauseScreen();
void DrawDeathScreen();
void DrawWinScreen();
void DrawLoseScreen();
void DrawStatsScreen();
void DrawHighscoreScreen();
void DrawDebugInfo();

void ResetGame();
void UnloadTextures(); // Unload all textures that have been loaded since InitWindow()