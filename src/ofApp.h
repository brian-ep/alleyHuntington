#pragma once

#include "ofMain.h"

//	Project headers
#include "POP.h"
#include "tNode.h"
#include "SceneSettings.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void createNewTree();
	void createColorPools();
	void newTree();

	void reloadShaders();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	
    //  App data
    float width, height;
    
    // Tree
	tNode* tree;
    
	//	Scene
    SceneSettings* scene;

	//	Drawing
	ofFbo treeBuffer;
	ofFbo forestBuffer;
	bool bAddTreeToForest = false;

	//	Rain
	POP pop;
	ofFbo popBuffer;

	//	Images
	ofImage paperImg;
};
