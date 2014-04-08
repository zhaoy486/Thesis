#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    
    //soundDraw yuanjin 4/6 begin
    soundDraw.setup( );
    //soundDraw yuanjin 4/6 begin
	ofSetLogLevel( OF_LOG_VERBOSE );	
	
	ofSetFrameRate(60);
	ofSetVerticalSync( false );
	
	//ofSetFrameRate( 60 );
	//ofSetVerticalSync( true );
		
	fontWorld.loadFont( "Fonts/DIN.otf", 18, true, false, true );
	//fontWorld.loadFont( "Fonts/DIN.otf", 18 );

	oculusRift.init( 1280, 800, 4 );
	oculusRift.setPosition( 0,-30,0 );
	
	lastUpdateTime = ofGetElapsedTimef();
    
    
    //thinkgear yuanjin @4/2
    thinkGear.setup();
    ofAddListener(thinkGear.attentionChangeEvent, this, &testApp::attentionListener);
    ofAddListener(thinkGear.meditationChangeEvent, this, &testApp::meditationListener);
    
    //mesh
    current.x = 0;
    current.y = 0;
    current.z = 0;
    t = 0;
    pathLines.setMode(OF_PRIMITIVE_LINE_STRIP);
    
    
    //audioIn 4/6 yuanjin begin
    int bufferSize = 256;
	
	
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);
	
	bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0;
	scaledVol		= 0.0;
    
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    //audioIn 4/6 yuanjin end
   
}


//--------------------------------------------------------------
void testApp::update()
{
    //soundDraw yuanjin 4/6 begin
    soundDraw.update(scaledVol*100*20,20, -ofGetElapsedTimef()*100);
    //soundDraw yuanjin 4/6 begin
    
    
	float currTime = ofGetElapsedTimef();
	float frameDeltaTime = currTime - lastUpdateTime;
	lastUpdateTime = currTime;
	
	if(		 ofGetKeyPressed('i') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() + 0.001f ); }
	else if( ofGetKeyPressed('o') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() - 0.001f ); }
	else if( ofGetKeyPressed('k') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() + 1.0f ); }
	else if( ofGetKeyPressed('l') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() - 1.0f ); }
	
	if(	ofGetKeyPressed(OF_KEY_UP) )    { oculusRift.dolly(  30.0f * frameDeltaTime*3 ); }
	if( ofGetKeyPressed(OF_KEY_DOWN) )  { oculusRift.dolly( -30.0f * frameDeltaTime ); }
	if( ofGetKeyPressed(OF_KEY_LEFT) )  { oculusRift.truck(  30.0f * frameDeltaTime ); }
	if( ofGetKeyPressed(OF_KEY_RIGHT) ) { oculusRift.truck( -30.0f * frameDeltaTime ); }
    
    
    //thinkgear 4/4 yuanjin
    thinkGear.update();
    
    //audioIn 4/6 yuanjin begin
    //lets scale the vol up to a 0-1 range
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    //lets record the volume into an array
	volHistory.push_back( scaledVol );
	
	//if we are bigger the the size we want to record - lets drop the oldest value
	if( volHistory.size() >= 400 ){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}
    //audioIn 4/6 yuanjin end
    
    
    //yuanjin 4/2 spiral begin
    previous = current;
    t+=0.001;
    //yuanjin try input attention data @ 3/31
    //change = scaledVol;
    change = distAw*2;
    
    //theta = t* 36* 2;
    
    //螺丝
    //    current.x = 4* cos(t*(5*360));
    //    current.y = 4* sin(t*(5*360));
    //    current.z = 100*t;
    
    //change +=1;
    //current.x = 40* cos(t*(change*360));
    //current.y = 40* sin(t*(change*360));
    //old
    //    current.x = change* cos(t*(5*360));
    //    current.y = change* sin(t*(5*360));
    //3/31yuanjin
    current.x = change*0.5*cos(t*(4*360));
    current.y = change*0.5*sin(t*(4*360));
    
    //哆哆嗦嗦的螺旋……
    //current.y = (40+(sin(t*10000)))* sin(t*(5*180));
    current.z = -2000*t;
    
    
    
    // add the current position to the pathVertices deque
	pathVertices.push_back(current);
	// if we have too many vertices in the deque, get rid of the oldest ones
    //	while(pathVertices.size() > 400) {
    //		pathVertices.pop_front();
    //	}
	
	// clear the pathLines ofMesh from any old vertices
	pathLines.clear();
    

	// add all the vertices from pathVertices
	for(int i = 0; i < pathVertices.size(); i++) {
		pathLines.addVertex(pathVertices[i]);
        
	}

     offsets.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000), ofRandom(0,100000)));
    //add shaking 4/4 yuanjin begin{
    int numVerts = pathLines.getNumVertices();
    for (int i=0; i<numVerts; ++i) {
        ofVec3f vert = pathLines.getVertex(i);
        float time = ofGetElapsedTimef();
        float timeScale = 5.0;
        float displacementScale = 0.75;
        ofVec3f timeOffsets = offsets[i];
        vert.x += (ofSignedNoise(time*timeScale+timeOffsets.x)) * displacementScale*scaledVol*200;
        vert.y += (ofSignedNoise(time*timeScale+timeOffsets.y)) * displacementScale*scaledVol;
        vert.z += (ofSignedNoise(time*timeScale+timeOffsets.z)) * displacementScale;
        pathLines.setVertex(i, vert);
    }
    //add shaking 4/4 yuanjin end{
}


//--------------------------------------------------------------
void testApp::draw()
{
	oculusRift.beginRenderSceneLeftEye();
    drawSceneGeometry();
	oculusRift.endRenderSceneLeftEye();
	
	oculusRift.beginRenderSceneRightEye();
    drawSceneGeometry();
	oculusRift.endRenderSceneRightEye();
	cout<<"attention"<<currAw<<endl;
    cout<<"Vol"<<scaledVol<<endl;
    
    cout<<"currTime"<<ofGetElapsedTimef()<<endl;
	ofSetColor( 255 );
	oculusRift.draw( ofVec2f(0,0), ofVec2f( ofGetWidth(), ofGetHeight() ) );
    
}

//--------------------------------------------------------------
void testApp::drawSceneGeometry()
{
    
    ofRotate(90, 0, 0, -1);
    ofDrawGridPlane(500.0f, 40.0f, false );

    //4/2 yuanjin begin{
	ofColor centerColor = ofColor(85, 78, 68);
    ofColor edgeColor(0, 0, 0);
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);
    //4/2 yuanjin end}
    glEnable( GL_DEPTH_TEST );
    //cam.begin();
    ofPushMatrix();
    ofTranslate( ofPoint(10,0,-80) );
    ofMultMatrix( oculusRift.getHeadsetOrientationMat() );
    ofSetLineWidth(2);
    
    pathLines.draw();
    
    //draw type yuanjin4/6 begin
    font.drawStringAsShapes("openFrameworks", 90, 260);
    //draw type yuanjin 4/6 end
    
    ofPushMatrix();
    ofTranslate(ofSignedNoise(20), ofSignedNoise(20),-ofGetElapsedTimef()*100);
   soundDraw.draw();
    ofPopMatrix();
  
    
    
    ofPopMatrix();
    
//    ofPushMatrix();
//    ofTranslate( ofPoint(10,0,-80) );
//    if(scaledVol>0.004){
//      
//        
//    ofBox( 0,scaledVol,scaledVol * -100-ofGetElapsedTimef()*10,20);
//        
//       
//    
//    }
//	ofPopMatrix();

    //cam.end();
    glDisable( GL_DEPTH_TEST );
	
    

    
//	ofSetColor(120);
//	
//	ofPushMatrix();
//    ofRotate(90, 0, 0, -1);
//    ofDrawGridPlane(500.0f, 40.0f, false );
//	ofPopMatrix();
//	
//	ofSetColor( 255, 0, 0 );
//	
//
//	string tmpStr = "Do Warping: " + ofToString( oculusRift.getDoWarping() ) + "\n";
//	tmpStr += "Inter Ocular Distance: "  + ofToString( oculusRift.getInterOcularDistance() ) + "\n";
//	tmpStr += "Shader Scale Factor: "  + ofToString( oculusRift.getShaderScaleFactor() ) + "\n";
//	
//	ofSetColor( 255 );
//	
//	ofPushMatrix();
//    ofTranslate( ofPoint(-60,280,-200) );
//    ofRotateZ( 180 );
//    ofRotateY( 180 );
//    fontWorld.drawString( tmpStr, 0.0f, 0.0f );
//	ofPopMatrix();
//	
//	ofSetColor(255);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
	if( key == 'w' )
	{
		oculusRift.setDoWarping( !oculusRift.getDoWarping() );
	}
}

void testApp::attentionListener(float &param)
{
    attention = param;
    distAw = ofMap(attention, 0.0, 100.0, 0, ofGetWidth());
    atChangeTime = ofGetElapsedTimef();
}

void testApp::meditationListener(float &param)
{
    meditation = param;
    distMw = ofMap(meditation, 0.0, 100.0, 0, ofGetWidth());
    meChangeTime = ofGetElapsedTimef();
}



//--------------------------------------------------------------
//audioIn 4/6 yuanjin begin
void testApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
        
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;
	
}
//audioIn 4/6 yuanjin end
//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}