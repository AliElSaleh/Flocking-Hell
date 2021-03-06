#pragma once

#include "Enemy.h"
#include "LinearPattern.h"

struct Spacecraft : Enemy
{
	Spacecraft();

	void Init() override;
	void Update() override;
	void Draw() override;

	void Flock(std::vector<Enemy*> *Boids) override;

protected:
	void ApplyBehaviours(std::vector<Enemy*> *Enemies) override;

	void IncreasePlayerScore() override;

private:
	struct LinearPattern LinearBullet{};

	float Rotation{0.0f};

	void UpdateBullet() override;
	void DrawBullet() override;
};

