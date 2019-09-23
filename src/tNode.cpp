#include "tNode.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tNode::tNode(tNode* parent, SceneSettings* scene, float x, float y, float angleOffset, float length){
    
    //  Set parent and position
    this->parent = parent;
    this->scene  = scene;
    this->x      = x;
    this->y      = y;
    
    //  Set parameters from parent
    if (parent != nullptr){
        this->angle       = parent->angle + angleOffset;
        this->angleOffset = angleOffset;

		depth = parent->depth + 1;
    }
    else {
        this->angle       = angleOffset;
        this->angleOffset = 0.0;
    }
    
    //  Set design parameters
    this->length = length;
	colorIndex = (int) ofRandom((float) scene->branch_colors.numColors());

	if (parent == nullptr)
		leafScale = ofMap(length, 24, 75, 0.2, 1.0);
	else
		leafScale = parent->leafScale;
    
    //  Set child position
    float xB = x + sin(angle) * length;
    float yB = y + cos(angle) * length;
   
	//  Check length & recursion depth before creating children
	if (length > 6 && depth < 40) {
		//  Create chance to split into two child branches
		float chance = ofMap(depth, 0, 40, 0.025, 0.25, true);
		if (depth > 3 && (parent->split == true || parent->parent->split == true)) chance = 0;
		else if (depth > 3 && depth < 9) chance = 0.9;
		if (ofRandom(1) < chance) split = true;

		if (split) {
			//	Branch A
			float newAngle = -0.2 - ofRandom(0.2) + (fmod(angle, PI) > HALF_PI ? -1 / length : 1 / length);
			branchA = new tNode(this, scene, xB, yB, newAngle, length * ofRandom(0.96, 1.0));
			
			//	Branch B
			newAngle = 0.2 + ofRandom(0.2) + (fmod(angle, PI) > HALF_PI ? -1 / length : 1 / length);
			branchB = new tNode(this, scene, xB, yB, newAngle, length * ofRandom(0.80, 0.95));
		}
		else {
			if (length + ofRandom(3 * length) > 20) {
				float direction = ofRandom(1.0) > 0.5 ? 1.0f : -1.0f;
				float newAngle = direction * ofRandom(0.4) + (fmod(angle, PI) > HALF_PI ? -1 / length : 1 / length);
				branchA = new tNode(this, scene, xB, yB, newAngle, length * ofRandom(0.96, 1.0));
			}
			else {
				drawLeafBuffer();
			}
		}

		//  If one branch is NULL make branch B NULL
		if (branchB != nullptr && branchA == nullptr) {
			branchA = branchB;
			branchB = nullptr;
		}
	}
	else {
		drawLeafBuffer();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tNode::update(){
    
	//	Apply wind forces
    if (parent != nullptr){
        x = parent->x + sin(parent->angle) * parent->length * parent->growth;
        y = parent->y + cos(parent->angle) * parent->length * parent->growth;
        
        windForce = parent->windForce * (1 + 1.0 / length);

		targetAngle = parent->angle + angleOffset + windForce;
		angle = 0.5 * prevAngle + 0.5 * targetAngle;
		prevAngle = angle;
        
		float newGrowth = growth + 0.025 * parent->growth;
        growth = min(newGrowth, 1.0f);
    }
    else {
        float newGrowth = growth + 0.025;
        growth = min(newGrowth, 1.0f);
    }
    
	//	Recurse
    if (branchA != nullptr){
        branchA->update();
        
        if (branchB != nullptr){
            branchB->update();
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tNode::draw(){
    
    if (branchA != nullptr){
        //  Set endpoint
        float xB = branchA->x;
        float yB = branchA->y;
        
		//	Set branch widths
		width = (length * growth) / 6.0f;

		ofSetColor(255, 255, 255, 255);
		ofColor c = scene->branch_colors.getColor(colorIndex);
		c.a = 255.0 * ofClamp( (1.0 - ((40.0 - length) / 40.0)), 0, 1);
        ofSetColor(c);

		ofPushMatrix();
		ofTranslate(x, y);
		ofRotateRad(-angle);
		ofDrawRectRounded(-width / 2, -width / 2, 0, width, length * growth + width, width / 2);
		ofPopMatrix();
        
        branchA->draw();
        if(branchB != nullptr) branchB->draw();
    }
    else {
		//	Draw leaf
        ofPushMatrix();
        ofTranslate(x, y);
        ofRotateRad(-angle);
      	ofSetColor(255);
        //leafBuffer.draw(-leafBuffer.getWidth() * ofClamp(growth, 0.05, 1.0) * leafScale / 2, -leafBuffer.getHeight() * ofClamp(growth, 0.05, 1.0) * leafScale / 2, leafBuffer.getWidth() * ofClamp(growth, 0.05, 1.0) * leafScale, leafBuffer.getHeight() * ofClamp(growth, 0.05, 1.0) * leafScale);
		leafBuffer.draw(-leafBuffer.getWidth() * ofClamp(growth, 0.1, 1.0) / 2, -leafBuffer.getHeight() * ofClamp(growth, 0.1, 1.0) / 2, leafBuffer.getWidth() * ofClamp(growth, 0.1, 1.0), leafBuffer.getHeight() * ofClamp(growth, 0.1, 1.0));
		ofPopMatrix();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tNode::clean() {
	leafBuffer.clear();
	if (branchA != nullptr) branchA->clean();
	if (branchB != nullptr) branchB->clean();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tNode::drawLeafBuffer(){
	//return;

	//	Size
	int w = ofRandom(0.025 * scene->screenW, 0.25 * scene->screenW) * leafScale;
	int h = w;

	//	Allocate
	leafBuffer.allocate(w, h, GL_RGBA16F_ARB, 4);
	leafBuffer.begin();
	ofClear(0, 0, 0, 0);
	leafBuffer.end();

	//	Draw settings
	int numLeaves = ofRandom(32, 128);
	int colorIdx = ofRandom(scene->leaf_colors.numColors());

	ofPushStyle();
	ofSetCircleResolution(360);

	//	Draw
	leafBuffer.begin();
	for (int i = 0; i < numLeaves; ++i) {
		ofColor color = scene->leaf_colors.getColor(colorIdx);
		color.a = 225;

		color.setSaturation(color.getSaturation() * ofRandom(0.5, 1.5));
		color.setBrightness(color.getBrightness() * ofRandom(0.5, 1.5));
		ofSetColor(color);

		int r = ofRandom(w / 16, w / 6);

		float rad = ofRandom(0, w / 2 - r);
		float theta = ofRandom(TWO_PI);

		int x = rad * cos(theta) + w / 2;
		int y = rad * sin(theta) + h / 2;

		ofFill();
		ofDrawCircle(x, y, r);

		ofNoFill();
		ofSetLineWidth(2.0);
		color.setHue(ofRandom(0, 255));
		color.setSaturation(ofRandom(0, 255));
		color.setBrightness(ofRandom(0, 255));
		ofSetColor(color);
		ofDrawCircle(x, y, r);
	}
	leafBuffer.end();

	ofPopStyle();
}
