//
//  SoundDrawing.cpp
//  OculusRenderingBasic
//
//  Created by Yuanjin Zhao on 4/6/14.
//
//

#include "SoundDrawing.h"
//--------------------------------------------------------------
void SoundDrawing::setup()
{   ofSetLogLevel( OF_LOG_VERBOSE );
    ofSetFrameRate(60);
	ofSetVerticalSync( false );
    
    VcurrSoundShape.x = 10;
    VcurrSoundShape.y = 0;
    VcurrSoundShape.z = 0;
    MsoundShape.setMode(OF_PRIMITIVE_LINE_STRIP);
    
}


//--------------------------------------------------------------
void SoundDrawing::update(float posx,float posy,float posz)
{
    VprevSoundShape=VcurrSoundShape;
    //begin shape
    VcurrSoundShape.x=posx;
    VcurrSoundShape.y=posy;
    VcurrSoundShape.z=-posz;
   
    
    
    
    
    SoundVertices.push_back(VcurrSoundShape);
    // if we have too many vertices in the deque, get rid of the oldest ones
    //	while(pathVertices.size() > 400) {
    //		pathVertices.pop_front();
    //	}
	
	// clear the pathLines ofMesh from any old vertices
	MsoundShape.clear();
    
    // add all the vertices from pathVertices
	for(int i = 0; i < SoundVertices.size(); i++) {
		MsoundShape.addVertex(SoundVertices[i]);
        
	}
    
    //offsets.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000), ofRandom(0,100000)));
}




//--------------------------------------------------------------
void SoundDrawing::draw()
{


ofPushMatrix();
    
ofTranslate( ofPoint(10,0,-80) );
//if(scaledVol>0.004){
    
    //ofBox( 0,0,-ofGetElapsedTimef()*10, 50);
    ofSetLineWidth(2);
    MsoundShape.draw();
//}

ofPopMatrix();

}