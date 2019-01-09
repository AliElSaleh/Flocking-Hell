#pragma once

#include "BulletPatternGenerator.h"

struct LinearPattern : BulletPatternGenerator
{
	LinearPattern();
	virtual ~LinearPattern() = default;

	void Init() override;
	void Update() override;
	void Draw() override;
};
