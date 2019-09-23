#pragma once

#include "ofMain.h"
#include "ColorPool.h"

struct SceneSettings {
	int screenW;
	int screenH;

    float windAngle = 0;
	float minX;
	float maxX;

	ColorPool branch_colors;
	ColorPool leaf_colors;
};
