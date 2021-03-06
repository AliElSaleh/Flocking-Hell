#include "EnemyManager.h"
#include "Demon.h"
#include "Alien.h"
#include "Flock.h"
#include "ArchDemon.h"
#include "Spacecraft.h"
#include "Warship.h"
#include "Player.h"

EnemyManager::EnemyManager()
{
	Init();
}

EnemyManager::~EnemyManager() = default;

void EnemyManager::AddEnemy(Enemy *NewEnemyType)
{
	Enemies.emplace_back(NewEnemyType);
}

void EnemyManager::Update()
{
	// Make all enemies inactive if not in the first element of vector
	if (!Enemies.empty())
		if (!bIsEnemyDead)
			for (unsigned short i = 1; i < Enemies.size(); i++)
				Enemies[i]->bActive = false;

	// Make all flock enemies inactive if not in the first element of vector
	if (!FlockOfEnemies.empty())
		for (unsigned short i = 1; i < FlockOfEnemies.size(); i++)
			for (unsigned short j = 0; j < FlockOfEnemies[i]->Boids.size(); j++)
				FlockOfEnemies[i]->Boids[j]->bActive = false;

	// Remove flock from vector when boids are empty
	if (!FlockOfEnemies.empty())
		if (FlockOfEnemies[0]->Boids.empty())
			RemoveFlock(0);

	// Remove enemy from vector on death
	if (!Enemies.empty())
		if (Enemies[0]->bIsDead)
		{
			if (Enemies[0]->FinalBullets->empty())
			{
				Enemies[0]->Player->BossKilled++;
				Enemies[0]->Player->bChangeMusic = true;

				RemoveEnemy(0);

				if (!FlockOfEnemies.empty())
				{
					if (FlockOfEnemies[0]->Boids.empty())
					{
						RemoveFlock(0);

						// Initialise the next flock in the vector
						for (unsigned short i = 0; i < FlockOfEnemies[0]->Boids.size(); i++)
						{
							FlockOfEnemies[0]->Boids[i]->Init();
							FlockOfEnemies[0]->Boids[i]->bActive = true;
						}
					}
				}

				bIsEnemyDead = true;

				if (!Enemies.empty())
				{
					Enemies[0]->Init();
					Enemies[0]->bActive = true;
				}

				bIsEnemyDead = false;
			}
		}

	// Remove flock enemy from vector on death
	if (!FlockOfEnemies.empty())
		if (!FlockOfEnemies[0]->Boids.empty())
			for (unsigned short i = 0; i < FlockOfEnemies[0]->Boids.size(); i++)
				if (FlockOfEnemies[0]->Boids[i]->bIsDead)
					if (FlockOfEnemies[0]->Boids[i]->FinalBullets->empty())
					{
						FlockOfEnemies[0]->Boids[i]->Player->EnemiesKilled++;
						RemoveEnemyFromFlock(i);
					}

	// Update the currently active flock
	if (!FlockOfEnemies.empty())
		if (!FlockOfEnemies[0]->Boids.empty())
			FlockOfEnemies[0]->Update();

	// Update the currently active enemy
	if (!Enemies.empty())
		Enemies[0]->Update();

	// Are all enemies dead
	if (FlockOfEnemies.empty() && Enemies.empty())
		bAllEnemiesDefeated = true;
}

void EnemyManager::Draw()
{
	if (!Enemies.empty())
		if (!bIsEnemyDead)
			Enemies[0]->Draw();

	// Draw the currently active flock
	if (!FlockOfEnemies.empty())
		if (!FlockOfEnemies[0]->Boids.empty())
			FlockOfEnemies[0]->Draw();

	// Draw the currently active enemy
	if (!Enemies.empty())
		Enemies[0]->Draw();
}

void EnemyManager::Reset()
{
	Enemies.clear();
	FlockOfEnemies.clear();
	bAllEnemiesDefeated = false;
}

void EnemyManager::Init()
{
	Enemies.reserve(3);
	FlockOfEnemies.reserve(3);

	FlockOfEnemies.emplace_back(reinterpret_cast<Flock<Enemy>*>(new Flock<ArchDemon>(25)));
	FlockOfEnemies.emplace_back(reinterpret_cast<Flock<Enemy>*>(new Flock<RocketShip>(40)));
	FlockOfEnemies.emplace_back(reinterpret_cast<Flock<Enemy>*>(new Flock<Spacecraft>(30)));

	Enemies.emplace_back(new Demon());
	Enemies.emplace_back(new Alien());
	Enemies.emplace_back(new Warship());

	bIsEnemyDead = false;
}

void EnemyManager::RemoveEnemy(const unsigned short Where)
{
	if (!Enemies.empty())
		Enemies.erase(Enemies.begin()+Where);
}

void EnemyManager::RemoveFlock(const unsigned short Where)
{
	if (!FlockOfEnemies.empty())
		FlockOfEnemies.erase(FlockOfEnemies.begin()+Where);
}

void EnemyManager::RemoveEnemyFromFlock(const unsigned short Where)
{
	if (!FlockOfEnemies[0]->Boids.empty())
		FlockOfEnemies[0]->Boids.erase(FlockOfEnemies[0]->Boids.begin()+Where);
}
