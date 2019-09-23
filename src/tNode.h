#pragma once

#include "ofMain.h"

#include "SceneSettings.h"

class tNode {
public:
  
	tNode(tNode* parent, SceneSettings* scene, float x, float y, float angleOffset, float length);
    
    void update();
    void draw();
	void clean();
	void drawLeafBuffer();
    
    ///////////////////////////////////////////////////////////
    float x;
    float y;
    float angle;
    float angleOffset;
    float length;
	float width;
    float growth = 0;
    float windForce = 0;
    float blastForce = 0;

	int depth = 0;

	float leafScale = 1;

	float prevAngle;
	float targetAngle;

	tNode* branchA = nullptr;
	tNode* branchB = nullptr;
	tNode* parent = nullptr;

	bool split = false;
    
    SceneSettings* scene;

	int colorIndex;

	ofFbo leafBuffer;
};
