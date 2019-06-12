#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(30);
    
    status = "Waiting for files to be dropped";
    
    
    gui.setup(); // most of the time you don't need a name
    gui.add(scale.setup("scale", 1, 0, 2));
   
    gui.add(rows.setup("rows", 1, 1, 12));
    gui.add(cols.setup("cols", 1, 1, 12));
    
    gui.add(rectXScale.setup("rectXScale,", 1, 0, 1));
    gui.add(rectYScale.setup("rectYScale,", 1, 0, 1));


    
    
    gui.add(generate.setup("generate"));
    generate.addListener(this, &ofApp::generateMarkers);


}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0);
    ofSetColor(255);
    
    ofDrawBitmapString(status, 20, 20);
    
    rectangles.clear();
    
    if(draggedImages.size() > 0 && draggedImages[0].isAllocated()) {
        
        ofPushMatrix();
        
        ofTranslate(ofGetWidth() * .5, ofGetHeight() * .5);
        ofScale(scale,scale);
        ofTranslate(-draggedImages[0].getWidth() * .5, -draggedImages[0].getHeight() * .5);
        draggedImages[0].draw(0.0, 0.0);
        
        for(int i=0; i<cols; i++) {
            
            for(int j=0; j< rows; j++) {
                
                float w = draggedImages[0].getWidth() / rows;
                float h = draggedImages[0].getHeight() / cols;
                float x = w * j;
                float y = h * i;
                
                // add rectangles for check
                ofRectangle rect;
                rect.set(x,y,w,h);
                rect.x += offSetX;
                rect.y += offSetY;
                rect.x *= scale;
                rect.y *= scale;
                rect.x += ofGetWidth() * .5 -draggedImages[0].getWidth() * scale * .5;
                rect.y += ofGetHeight() * .5 -draggedImages[0].getHeight() * scale * .5;
                
                
                
                rect.width *= scale * rectXScale;
                rect.height *= scale * rectYScale;
                rectangles.push_back(rect);
                
                
            }
            
        }
        
        ofPopMatrix();
        
        if(selecteds.size() != rectangles.size()) {
            selecteds.clear();
            for(int i=0; i<rectangles.size(); i++) {
                selecteds.push_back(0);
            }
        }
        
        for(int i=0; i<rectangles.size(); i++) {
            
            int strokeWeight = 1;
            if(selecteds[i] == true) {
                strokeWeight = 3;
            }
            
            ofNoFill();
            glLineWidth(strokeWeight);
            ofDrawRectangle(rectangles[i]);
        }
        
        gui.draw();
    }
    
    ofNoFill();
    ofDrawRectangle(mouseRect);
    
}

void ofApp::generateMarkers() {
    
    if(selecteds.size() == 0)
        return;
    
    int count = 0;
    int index = 0;
    for(int i=0; i<cols; i++) {
        
        for(int j=0; j< rows; j++) {
            
            if(selecteds[index] == true ) {
                
                float w = draggedImages[0].getWidth() / rows;
                float h = draggedImages[0].getHeight() / cols;
                
                w *= rectXScale;
                h *= rectYScale;
                
                ofPoint pos;
                pos.x = j * w;
                pos.y = i * h;
                
                pos.x += round(offSetX);
                pos.y += round(offSetY);
                
                
                ofImage imgClone = draggedImages[0];
                imgClone.crop(pos.x, pos.y, w, h);
                string filename = imgFileName + "_" + ofToString(count) + ".jpg";
                imgClone.save(filename);
                
                ofFile configFile;
                configFile.open(imgFileName + "_" + ofToString(count) + ".cnf",ofFile::WriteOnly);
                configFile << "# Number of markers\n";
                configFile << "1\n";
                configFile << imgFileName + "_" + ofToString(count)+"\n";
                configFile << "NFT\n";
                configFile << "FILTER 5.0\n";

                status = "Processing";
                
                string path = ofToDataPath(filename, true);
                string command = "../../../../../utils/genTexData " + path + " -level=4 -leveli=3 -dpi=300 -max_dpi=300 -min_dpi=72";
                ofLogNotice("command ") << command;

                string result = ofSystem(command);
                ofLogNotice("result ") << result;
                
                count++;
                
            }
            index++;

        }
        
    }
    
    // generate config file
    
   
    
    // copy everything into the main app
    
    string path = "";
    ofDirectory dir(path);
    dir.listDir();
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        dir.getFile(i).moveTo("../../bin/data/markers/"+dir.getFile(i).getBaseName()+"/"+dir.getFile(i).getFileName(), true);
    }
    
    status = "¨All done. ";
    
    
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
    
   // mouseRect.set(clickedPoint.x, clickedPoint.y, x - clickedPoint.x, y - clickedPoint.y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    for(int i=0; i<rectangles.size(); i++) {
        
        if(rectangles[i].inside(x, y))
            selecteds[i] = !selecteds[i];
        
    }
    
    //clickedPoint.set(x,y);
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
    
    if( dragInfo.files.size() > 0 ){
        
        status = "Analyzing";

        dragPt = dragInfo.position;
        
        float subDivisions = 1;
        
        draggedImages.assign( dragInfo.files.size(), ofImage() );
        for(unsigned int k = 0; k <1; k++){
            
            string path = dragInfo.files[k];
            draggedImages[k].load(path);
            imgFileName = path.substr(path.find_last_of("/\\") + 1);
            size_t lastindex = imgFileName.find_last_of(".");
            imgFileName = imgFileName.substr(0, lastindex);
            
            gui.add(offSetX.setup("offSetX", 0, 0, draggedImages[k].getWidth()));
            gui.add(offSetY.setup("offSetY", 0, 0, draggedImages[k].getHeight()));
            
            return;
        }
    

    }
    
}
