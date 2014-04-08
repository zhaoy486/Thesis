//
//  SoundDrawing.h
//  OculusRenderingBasic
//
//  Created by Yuanjin Zhao on 4/6/14.
//
//

#ifndef __OculusRenderingBasic__SoundDrawing__
#define __OculusRenderingBasic__SoundDrawing__

#include <iostream>
#include "ofMain.h"

#endif /* defined(__OculusRenderingBasic__SoundDrawing__) */

class SoundDrawing : public ofBaseApp{

    
public:
    void setup();
    void update(float posx, float posy, float posz);
    void draw();
    ofMesh MsoundShape;
    ofVec3f VcurrSoundShape;
    ofVec3f VprevSoundShape;
    deque<ofVec3f> SoundVertices;
    vector<ofVec3f> offsets;
};