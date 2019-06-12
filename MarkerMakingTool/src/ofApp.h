#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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
    
        void generateMarkers();
    
        string imgFileName;
        vector <ofImage> draggedImages;
        ofPoint dragPt;
        string status;
    
        ofxFloatSlider scale;
        ofxIntSlider rows, cols;
        ofxButton generate;
    
        ofxFloatSlider rectXScale, rectYScale;
        ofxFloatSlider offSetX, offSetY;

        //ofxFloatSlider rectScale;

    
        ofxPanel gui;
    
        vector<bool> selecteds;
        vector<ofRectangle> rectangles;
    
        ofPoint clickedPoint;
        ofRectangle mouseRect;

};
