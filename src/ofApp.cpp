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
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //  Draw scene
	ofBackgroundGradient(ofColor(255, 255, 255, 255), ofColor(200, 200, 200, 255));
	ofPushStyle();
	ofSetColor(255, 255, 255, 64);
	paperImg.draw(0, 0);
	ofPopStyle();

	//	Draw tree
	treeBuffer.begin();
	ofBackgroundGradient(ofColor(255, 255, 255, 1), ofColor(255, 255, 255, 1));
    tree->draw();
	treeBuffer.end();

	//	Draw forest
	forestBuffer.begin();
	if(ofGetFrameNum() % 30 == 29)  ofBackgroundGradient(ofColor(0, 54, 104, 1), ofColor(0, 8, 13, 1));
	forestBuffer.end();

	forestBuffer.draw(0, 0);
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
	scene->leaf_colors.addColor(ofColor::fromHex(0xFDF0D5));
	scene->leaf_colors.addColor(ofColor::fromHex(0xC6D8D3));
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
