#pragma once

#include "BulletPatternGenerator.h"

#define Stringify(name) #name

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct CirclePattern : BulletPatternGenerator
{
	CirclePattern();

	void Init() override;
	void Update() override;
	void Draw() override;

	void Delay(float Seconds) override;

	void AddDebugSwitchPatternCode() override;
	void DrawDebugInfo() override;
};

