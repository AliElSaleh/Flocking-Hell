#include "raylib.h"
#include "Player.h"

// Variable Declarations
//-----------------------------------------
struct Player Player;
//-----------------------------------------

// Function Declarations
//-----------------------------------------
void Init();
void Update();
void Draw();
void MovePlayer();
void CheckWindowCollision();
//-----------------------------------------

int main()
{
	Init();

	// Main game loop
	while (!WindowShouldClose())
	{
		Update();
		Draw();
	}

	CloseWindow();

    return 0;
}

// Function Definitions
//-----------------------------------------
/// Initialise application
void Init()
{
	const int ScreenWidth = 700;
	const int ScreenHeight = 800;

	InitWindow(ScreenWidth, ScreenHeight, "Flocking Hell");
	SetTargetFPS(144);

	Player.Location.x = (float)ScreenWidth / 2;
	Player.Location.y = (float)ScreenHeight - 100;
	Player.Size = 20.0f;
}

/// Update game logic
void Update()
{
	MovePlayer();
	CheckWindowCollision();
}

/// Render objects
void Draw()
{
	BeginDrawing();

	ClearBackground(LIGHTBLACK);
	
	DrawFPS(10, 10);
	DrawCircleGradient((int)Player.Location.x, (int)Player.Location.y, Player.Size, RED, BLUE);

	EndDrawing();
}

void MovePlayer()
{
	if (IsKeyDown(KEY_A))
		Player.Location.x -= 2;
	
	if (IsKeyDown(KEY_D))
		Player.Location.x += 2;
	
	if (IsKeyDown(KEY_W))
		Player.Location.y -= 2;
	
	if (IsKeyDown(KEY_S))
		Player.Location.y += 2;
}

void CheckWindowCollision()
{
	if (Player.Location.x + Player.Size > GetScreenWidth())
		Player.Location.x = GetScreenWidth() - Player.Size;
	
	if (Player.Location.y + (int)Player.Size > GetScreenHeight())
		Player.Location.y = GetScreenHeight() - Player.Size;

	if (Player.Location.x - Player.Size < 0)
		Player.Location.x = Player.Size;
	
	if (Player.Location.y - Player.Size < 0)
		Player.Location.y = Player.Size;
}
//-----------------------------------------