#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//	OF Settings
	ofSetWindowShape(1920, 1200);
	ofSetFullscreen(true);
	ofSetFrameRate(60);
	ofSetBackgroundAuto(false);
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	ofHideCursor();

	//	Window settings
    width   = ofGetWidth();
    height  = ofGetHeight();

	//	Geometry
	plane.set(width, height);
	plane.mapTexCoords(0, 0, width, height);
    
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
	pop.setup(512 * 1024);
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

	//	Spout
	spoutIn.setup();

	//	Interaction data
	kShaderPath = "shaders/interaction";
	kShader.load(kShaderPath);

	kBuffer.allocate(width, height, GL_RGBA);
	kBuffer.begin();
	ofClear(0);
	kBuffer.end();
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

	//	Update Spout
	spoutIn.updateTexture();

	//	Update interaction data
	kBuffer.begin();
	ofClear(0);
	kShader.begin();
	kShader.setUniform2f("res", width, height);
	kShader.setUniformTexture("kTex", spoutIn.getTexture(), 0);
	ofPushMatrix();
	ofTranslate(width / 2, height / 2);
	plane.draw();
	ofPopMatrix();
	kShader.end();
	kBuffer.end();

	//	Update POP
	pop.updateKinectData(kBuffer.getTexture());
	pop.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //  Draw scene
	ofBackgroundGradient(ofColor(255, 255, 255, 255), ofColor(200, 200, 200, 255));
	ofPushStyle();
	ofSetColor(255, 255, 255, 64);
	paperImg.draw(0, 0, width, height);
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

	//	POP
	ofSetColor(255);
	popBuffer.begin();
	ofBackgroundGradient(ofColor(37, 27, 52, 1), ofColor(8, 4, 13, 1));
	pop.draw();
	popBuffer.end();
	popBuffer.draw(0, 0);

	//	Draw tree over
	treeBuffer.draw(0, 0);

	//	Kinect Data
	if (bDrawK)
		kBuffer.draw(0, 0, width, height);
	if (bDrawSpout)
		spoutIn.getTexture().draw(0, height, width, -height);
}

//--------------------------------------------------------------
void ofApp::createNewTree(){
    
    if(scene == NULL) scene = new SceneSettings;
    
    tree = new tNode(NULL, scene, width/2 + ofRandom(-0.5 * width, 0.5 * width), height, ofRandom(0.8 * PI, 1.2 * PI), height / 20 * ofRandom(0.5, 1.25));
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
	//	Interaction shader
	kShader.unload();
	kShader.load(kShaderPath);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key)
	{
	case 'f':
		ofSetFullscreen(true);
		ofHideCursor();
		break;
	case 'g':
		ofSetFullscreen(false);
		ofShowCursor();
		break;
	case 'k':
		bDrawK = !bDrawK;
		break;
	case 'r':
		reloadShaders();
		pop.loadShaders();
		break;
	case 's':
		bDrawSpout = !bDrawSpout;
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
