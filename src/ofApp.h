#pragma once

#include "ofMain.h"

//	Project headers
#include "POP.h"
#include "SceneSettings.h"
#include "tNode.h"

//	Addons
//#include "ofxKinectForWindows2.h"
#include "ofxOpenCv.h"
#include "ofxSpout2Receiver.h"

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

	ofPlanePrimitive plane;

	//	Rain
	POP pop;
	ofFbo popBuffer;

	//	Images
	ofImage paperImg;

	//	Spout
	ofxSpout2::Receiver spoutIn;
	ofTexture spoutTex;
	bool bDrawSpout = false;

	//	Interaction data
	ofShader kShader;
	string kShaderPath;
	ofFbo kBuffer;
	bool bDrawK = false;

	/////////////////////////////////////////////////////////////
	//	Kinect
	//ofxKFW2::Device kinect;

	////	Open CV
	//ofxCvGrayscaleImage grayImage; // grayscale depth image
	//ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	//ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	//ofxCvGrayscaleImage kDataImg;

	//int nearThreshold;
	//int farThreshold;
};
