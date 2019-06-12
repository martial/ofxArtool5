#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofSetCircleResolution(92);
    
    cam.listDevices();
    cam.setDeviceID(1);
    cam.initGrabber(640, 480);
    
    // you'll need to import your AR assets in Resources folder in osx
    art.setup(ofVec2f(640,480),ofVec2f(640,480), OF_PIXELS_BGR, "../Resources/camera_para.dat", "../Resources/1200x630bf_0.cnf" );
    
    ofAddListener(art.evNewMarker, this, &ofApp::onNewMarker);
    ofAddListener(art.evLostMarker, this, &ofApp::onLostMarker);
    
    ofSetDataPathRoot("../Resources/");
    img.load("../Resources/1200x630bf_0.jpg");
    
    ghostImg.load("../Resources/ghost.png");
    
    centeredGhostImg.load("../Resources/centered_ghost.png");
    
    float scale     = 12.0;
    arImgWidth     = img.getWidth() / scale;
    arImgHeight    = img.getHeight() / scale;
    
    mask.allocate(img.getWidth(), img.getHeight(),GL_RGBA);
    maskedFbo.allocate(img.getWidth(), img.getHeight(),GL_RGBA);
    
    mask.begin();
    ofClear(0,0,0,0);
    ofSetColor(255, 255);
    ofDrawRectangle(0.0, 0.0, img.getWidth(), img.getHeight());
    mask.end();

}

//--------------------------------------------------------------
void ofApp::update(){
    
    cam.update();
    if(cam.isFrameNew()){
        art.update(cam);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255, 255);

    cam.draw(0, 0);
    maskedFbo.getTexture().setAlphaMask(mask.getTexture());

    maskedFbo.begin();
    ofClear(255,0);
    ofEnableAlphaBlending();
    
    float pct = 0.5 + cos((float)ofGetElapsedTimeMillis() / 500) * .5f;
    ofSetColor(255, 0,0, 125);
    ofDrawEllipse(img.getWidth() * .5, img.getHeight() * .5, pct * 1000, pct * 1000);
    ofSetColor(255, 255);

    ghostImg.draw(0.0, 0.0);

    maskedFbo.end();
    maskedFbo.getTexture().setAlphaMask(mask.getTexture());
    //maskedFbo.draw(0.0,0.0);

    
    ofEnableAlphaBlending();

    if(art.isFound()){
        
        // test with scale
        ofSetColor(255);

        art.beginAR();
        ofPushMatrix();
        
        ofScale(1 / 12.0f, - 1 /( 12.0f), 1/12.0f);
        ofTranslate(0.0, -img.getHeight());
        
        maskedFbo.draw(0.0,0.0);
        
        ofSetColor(255,0,0);
        ofDrawEllipse(0.0, 0.0, 30, 30);
        
        ofSetColor(0,255,0);
        ofDrawEllipse( img.getWidth(), 0.0, 30, 30);
        
        ofSetColor(0,0,255);
        ofDrawEllipse(0.0, img.getHeight(), 30, 30);
        
        ofSetColor(255,255,0);
        ofDrawEllipse( img.getWidth(),img.getHeight(), 30, 30);
        
        for(int i=0; i<2; i++) {
            ofPushMatrix();
            
            // get in the center, so pos of
            ofTranslate(img.getWidth() * .5 - centeredGhostImg.getWidth() * .5,
                        img.getHeight() * .5-centeredGhostImg.getHeight() * .5,
                        i * 10);
            
            // rotate from it center
            ofTranslate(centeredGhostImg.getWidth() * .5,centeredGhostImg.getHeight() * .5, 0);
            //ofRotateXDeg(-90);
            //ofRotateYDeg(ofGetMouseX());
            ofTranslate(-centeredGhostImg.getWidth() * .5,-centeredGhostImg.getHeight() * .5, 0);
            
            ofSetColor(255);

            centeredGhostImg.draw(0.0, 0.0);
            ofSetRectMode(OF_RECTMODE_CORNER);

            ofPopMatrix();
            
        }
        
        
        ofPopMatrix();

      
        
        art.endAR();
    }
    
}

void ofApp::onNewMarker(int & mId){
}

void ofApp::onLostMarker(int & mId){
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
