#pragma once

#include "ofMain.h"
#include "ofxOculusRift.h"
#include "ofxThinkGear.h"
#include "SoundDrawing.h"

class testApp : public ofBaseApp
{
	public:
	
		void setup();
		void update();
		void draw();
		
		void drawSceneGeometry();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		ofxOculusRift		oculusRift;
	
		float				lastUpdateTime;
	
		ofLight				light;
		ofEasyCam			cam;
	
		ofTrueTypeFont		fontWorld;
    
    
    //thinkgear yuanjin @3/31
    ofxThinkGear thinkGear;
    void attentionListener(float &param);
    void meditationListener(float &param);
    
    
    float atChangeTime;
    float meChangeTime;
    float attention;
    float meditation;
    float distAw;
    float prevAw;
    float currAw;
    float distMw;
    float prevMw;
    float currMw;
    //thinkgear yuanjin @3/31
    
    
    //spiral yuanjin 4/2 begin
    ofMesh pathLines;
    deque<ofVec3f> pathVertices;
    vector<ofVec3f> offsets;
    ofVec3f previous, current;
    float t, theta, change;
    //spiral yuanjin 4/3 end
    
    
    //soundIn yuanjin 4/5 begin
    void audioIn(float * input, int bufferSize, int nChannels); 
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    
    int 	bufferCounter;
    int 	drawCounter;
    
    float smoothedVol;
    float scaledVol;
    
    ofSoundStream soundStream;
    //soundIn yuanjin 4/5 begin
    
    //sounddrawing
    SoundDrawing soundDraw;
   
	//type yuanjin 4/6 begin
    ofTrueTypeFont font;
    //type yuanjin 4/6 end
};
