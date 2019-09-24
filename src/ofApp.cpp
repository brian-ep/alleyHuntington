#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//	OF Settings
	ofSetWindowShape(1920, 1200);
	ofSetFrameRate(60);
	ofSetBackgroundAuto(false);
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	ofHideCursor();

	//	Window settings
    width   = ofGetWidth();
    height  = ofGetHeight();
    
	//	Scene
    scene = new SceneSettings;
	scene->minX = 0.1 * width;
	scene->maxX = 0.9 * width;
	scene->screenW = (int)width;
	scene->screenH = (int)height;

	//	Buffers
	treeBuffer.allocate(width, height, GL_RGBA16F_ARB, 8);
	treeBuffer.begin();
	ofBackgroundGradient(ofColor(0, 54, 104), ofColor(0, 8, 13));
	treeBuffer.end();

	forestBuffer.allocate(width, height, GL_RGBA16F_ARB, 8);
	forestBuffer.begin();
	ofClear(0);
	forestBuffer.end();

	//	Images
	paperImg.load("images/paper.jpg");

	//	Create assets
	createColorPools();
	createNewTree();

	//	POP
	pop.setup(512 * 512);
	pop.updateColorPool(scene->leaf_colors);

	ofFbo::Settings s;
	s.internalformat = GL_RGBA32F_ARB;
	s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	s.minFilter = GL_NEAREST;
	s.maxFilter = GL_NEAREST;
	s.wrapModeHorizontal = GL_CLAMP;
	s.wrapModeVertical = GL_CLAMP;
	s.width = width;
	s.height = height;
	s.numColorbuffers = 1;
	popBuffer.allocate(s);

	popBuffer.begin();
	ofClear(0);
	popBuffer.end();

	//	CV
	grayImage.allocate(512, 424);
	grayThreshNear.allocate(512, 424);
	grayThreshFar.allocate(512, 424);
	kDataImg.allocate(width, height);

	nearThreshold = 5;
	farThreshold = 0;

	//	Kinect
	kinect.open();
	kinect.initDepthSource();
}

//--------------------------------------------------------------
void ofApp::update(){

	//	Window title
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	//	Timeline
	if (ofGetFrameNum() % 1800 == 1799) newTree();

	//	Update scene
	scene->windAngle += 0.001 * ofNoise(1.0);

	//	Update tree
	tree->windForce = sin(scene->windAngle) * 0.02;
	tree->update();

	///////////////////////////////////////////////////////////
	//	Kinect
	kinect.update();

	//	CV
	if (kinect.isFrameNew())
	{
		grayImage.setFromPixels(kinect.getDepthSource()->getPixels());

		grayThreshNear = grayImage;
		grayThreshFar = grayImage;
		grayThreshNear.threshold(nearThreshold, true);
		grayThreshFar.threshold(farThreshold);
		cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);

		grayImage.flagImageChanged();

		kDataImg.scaleIntoMe(grayImage, CV_INTER_LINEAR);
		kDataImg.updateTexture();

		//	Update POP interaction data
		pop.updateKinectData(kDataImg.getTexture());
	}

	//	Update POP
	pop.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //  Draw scene
	ofBackgroundGradient(ofColor(255, 255, 255, 255), ofColor(200, 200, 200, 255));
	ofPushStyle();
	ofSetColor(255, 255, 255, 64);
	paperImg.draw(0, 0);
	ofPopStyle();

	//	Draw tree to buffer
	treeBuffer.begin();
	ofBackgroundGradient(ofColor(255, 255, 255, 2), ofColor(255, 255, 255, 2));
    tree->draw();
	treeBuffer.end();

	//	Draw forest
	forestBuffer.begin();
	if(ofGetFrameNum() % 30 == 29)  ofBackgroundGradient(ofColor(0, 54, 104, 1), ofColor(0, 8, 13, 1)); 
	forestBuffer.end();
	forestBuffer.draw(0, 0);

	//	Kinect data
	//kDataImg.draw(0, 0);

	//	Rain
	ofSetColor(255);
	popBuffer.begin();
	ofBackgroundGradient(ofColor(37, 27, 52, 8), ofColor(8, 4, 13, 8));
	pop.draw();
	popBuffer.end();
	popBuffer.draw(0, 0);

	//	Draw tree over
	treeBuffer.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::createNewTree(){
    
    if(scene == NULL) scene = new SceneSettings;
    
    tree = new tNode(NULL, scene, width/2 + ofRandom(-0.5 * width, 0.5 * width), height, ofRandom(0.8 * PI, 1.2 * PI), height / 20 * ofRandom(0.4, 1.25));
}

//--------------------------------------------------------------
void ofApp::createColorPools() {
	//	Branch colors
	scene->branch_colors.clear();
	scene->branch_colors.addColor(ofColor::fromHex(0x0C0F0A));
	scene->branch_colors.addColor(ofColor::fromHex(0x0C0F0A));
	scene->branch_colors.addColor(ofColor::fromHex(0x0C0F0A));
	scene->branch_colors.addColor(ofColor::fromHex(0x0C0F0A));
	scene->branch_colors.addColor(ofColor::fromHex(0x1C2826));
	scene->branch_colors.addColor(ofColor::fromHex(0x1C2826));
	scene->branch_colors.addColor(ofColor::fromHex(0x1C2826));
	scene->branch_colors.addColor(ofColor::fromHex(0x403D39));
	scene->branch_colors.addColor(ofColor::fromHex(0x485665));
	scene->branch_colors.addColor(ofColor::fromHex(0x353A47));

	//	Leaf colors
	scene->leaf_colors.clear();
	scene->leaf_colors.addColor(ofColor::fromHex(0xAD001A));
	scene->leaf_colors.addColor(ofColor::fromHex(0xAD001A));
	scene->leaf_colors.addColor(ofColor::fromHex(0xD64550));
	scene->leaf_colors.addColor(ofColor::fromHex(0xEA9E8D));
	scene->leaf_colors.addColor(ofColor::fromHex(0xC6D8D3));
	scene->leaf_colors.addColor(ofColor::fromHex(0xFDF0D5));

}

//--------------------------------------------------------------
void ofApp::newTree(){
	forestBuffer.begin();
	tree->draw();
	forestBuffer.end();

	tree->clean();
	delete tree;
	createNewTree();
}

//--------------------------------------------------------------
void ofApp::reloadShaders() {

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key)
	{
	case 'r':
		reloadShaders();
		pop.loadShaders();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	newTree();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
