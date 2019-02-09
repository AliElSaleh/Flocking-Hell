#pragma once
#include "Enemy.h"
#include "HealthPickup.h"

#include <vector>
#include "Flock.h"

struct EnemyManager
{
	EnemyManager();
	~EnemyManager();

	void Init();
	void AddEnemy(Enemy* NewEnemyType);
	void Update();
	void Draw();

	std::vector<Enemy*> Enemies{};
	std::vector<Flock<Enemy>*> FlockOfEnemies{};

	void Reset();
private:
	//std::vector<HealthPickup*> Heart{};

	void RemoveEnemy(unsigned short Where);
	void RemoveFlock(unsigned short Where);
	void RemoveEnemyFromFlock(unsigned short Where);

	bool bWasPreviousEnemyDead{};
	bool bIsEnemyDead{};
};

