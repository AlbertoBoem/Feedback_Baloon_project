#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPd.h"

//a namespace for the Pd types
using namespace pd;

class testApp : public ofBaseApp, public PdReceiver{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		//void keyReleased(int key);
		//void mouseMoved(int x, int y );
		//void mouseDragged(int x, int y, int button);
		//void mousePressed(int x, int y, int button);
		//void mouseReleased(int x, int y, int button);
		//void windowResized(int w, int h);
		//void dragEvent(ofDragInfo dragInfo);
		//void gotMessage(ofMessage msg);
		
	    //audio callbacks
		void audioReceived(float *input, int bufferSize, int nChannels);
		void audioRequested(float *input, int bufferSize, int nChannels);
	
		//pd message receiver callbacks
		void print(const std::string& message);
	    
	    //pd instance
		ofxPd pd;
	    
	    //array from pd
		vector<float> scopeArray;
	
	    //----------------------GUI-----------------------
		//On-off the GUI
		bool guiHide; 
	    
	    //ctrl pd
	    //adc~
		ofParameter<float> varA; //vol input
	    
		//filter
		ofParameter<float> varB; //delay
		ofParameter<float> varC; //resonance
		ofParameter<float> varD; //cutoff
		ofParameter<float> varE; //gain
	
		//ctrl fx mono shaper
		ofParameter<float> varF; //amount
		ofParameter<float> varG; //depth
		ofParameter<float> varH; //limit
		ofParameter<float> varI; //gain
	
		//dac~
		ofParameter<float> varJ; //vol output
	    
	    //not in use
		ofxToggle soundInput; //audio in on-off
		ofxToggle soundOutput; //audio in on-off
	    	
		ofxPanel gui;
	
		
};
