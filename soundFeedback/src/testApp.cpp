#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	//the number of libpd ticks per buffer
	//used to compute the audio buffer len: tpb * blocksize (always 64)
	#ifdef TARGET_LINUX_ARM
	//longer latency for Raspberry PI
	int ticksPerBuffer = 32; //32 * 64 = buffer len of 2048
	
	//internal mic, if needed an external one (like in the Raspi) need to be changed
	int numInputs = 0; 
	#else
		int ticksPerBuffer = 8; //8 * 64 = buffer len of 512
		int numInputs = 1;
	#endif
	
	//setup OF sound stream
	ofSoundStreamSetup(2, numInputs, this, 44100, ofxPd::blockSize()*ticksPerBuffer, 3);
	
	//setup pd
	if (!pd.init(2, numInputs, 44100, ticksPerBuffer)) { //num out, chann-num in, chann-samp rate-thicks x buffer
		OF_EXIT_APP(1);
	}
		
	//in patch is received
	pd.subscribe("vol_in");
	pd.subscribe("del_time");
	pd.subscribe("resonance");
	pd.subscribe("cutoff");
	pd.subscribe("gain_mpf");
	pd.subscribe("amount");
	pd.subscribe("depth");
	pd.subscribe("limit");
	pd.subscribe("gain_ms");
	pd.subscribe("vol_out");
	pd.addToSearchPath("pd");
	pd.start();
		
	//open patch
	Patch patch = pd.openPatch("pd/_main.pd"); //name of the patch 
	cout << patch << endl;
	
	//setup - set
	gui.setup(); 
	gui.add(varA.set("Microphone volume", 0.2, 0, 1));
	gui.add(varB.set("Delay", 0, 0, 20000));
	gui.add(varC.set("Filter: Resonance", 0, 0, 1));
	gui.add(varD.set("Filter: Cut off", 0, 0, 1));
	gui.add(varE.set("Filter: Gain", 0, 0, 1));
	gui.add(varF.set("Shaper: Amount", 0, 0, 1));
	gui.add(varG.set("Shaper: Depth", 0, 0, 1));
	gui.add(varH.set("Shaper: Limit", 0, 0, 1));
	gui.add(varI.set("Shaper: Gain", 0, 0, 1));
	gui.add(varJ.set("Volume", 0.5, 0, 1));
	
	guiHide = true;

}

//--------------------------------------------------------------
void testApp::update(){
	
	//update scope array from pd
	pd.readArray("oscope", scopeArray);
	
	//update pd from gui
	//getFloatSlider(string)
	//getParameter()
	//compund messages
	pd << StartMessage() << "vol_in" << varA << FinishList("TO_PD");
	pd << StartMessage() << "del_time" << varB << FinishList("TO_PD");
	pd << StartMessage() << "resonance" << varC << FinishList("TO_PD");
	pd << StartMessage() << "cutoff" << varD << FinishList("TO_PD");
	pd << StartMessage() << "gain_mpf" << varE << FinishList("TO_PD");
	pd << StartMessage() << "amount" << varF << FinishList("TO_PD");
	pd << StartMessage() << "depth" << varG << FinishList("TO_PD");
	pd << StartMessage() << "limit" << varH << FinishList("TO_PD");
	pd << StartMessage() << "gain_ms" << varI << FinishList("TO_PD");
	pd << StartMessage() << "vol_out" << varJ << FinishList("TO_PD");

    //single messages
	//pd << Float("test", val); //"test" is the receiver name in pd
	//pd << Bang("test") ///pd << Symbol("test", "a symbol")
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofBackgroundGradient(ofColor::white, ofColor::gray);
	
	
	//draw scope (visuals)
	ofSetColor(0, 0, 0, 127);
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofSetLineWidth(2.0);
	float x = 1;
	float y = ofGetHeight()/2;
	float w = ofGetWidth() / (float) scopeArray.size();
	float h = ofGetHeight()/10;
	for (int i = 0; i < scopeArray.size()-1; ++i) {
		ofLine(x, y+scopeArray[i]*h, x+w, y+scopeArray[i+1]*h);
		x += w;
	}
	ofSetLineWidth(1.0);
	 
	
	//on-off gui
	if (guiHide) {
		gui.draw();
	}

}

//--------------------------------------------------------------
void testApp::audioReceived(float * input, int bufferSize, int nChannels) {
	pd.audioIn(input, bufferSize, nChannels);
}
		
//--------------------------------------------------------------
void testApp::audioRequested(float * output, int bufferSize, int nChannels) {
	pd.audioOut(output, bufferSize, nChannels);
}
		
//--------------------------------------------------------------
void testApp::print(const std::string& message) {
	cout << message << endl;
}
		
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if (key == 'h') {
		guiHide = !guiHide; //on-off gui
	}

}
/*
//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
*/
