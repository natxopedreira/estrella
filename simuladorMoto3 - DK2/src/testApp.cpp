#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup2(){
    ofBackground(0);
    ofSetVerticalSync(false);
	//ofSetFrameRate(90);
    float offsetTelemetry = 29.0;
    

    /// nice splash screen
    this->splashScreen.init("splashscreen.png");
	this->splashScreen.begin();

    
    // video panorama
    videoPanorama.setup("ESCENA-02-COLOR.mp4", "casco estrella.png"); /// jpeg video only 30%
    
    
    // telemetry pasamos la referencia del videoplayer para controlar el frame de la telemetria
    datosTelemetria.setup("ESCENA-02-COLOR.csv", offsetTelemetry, &videoPanorama.videoplayer);
    
    
    // plataforma
    plataforma.setup();
    plataforma.startThread();

	/// luto para la tv
	luto.loadImage("lutoTelemetria.png");
    
	this->splashScreen.end();

	
    ///oculus
	oculusRift.baseCamera = &cam;
	oculusRift.setup();

    // camera
	
    cam.begin();
    cam.end();
  //  cam.setGlobalPosition(0,0,0);

    camTv.begin();
    camTv.end();
    camTv.setGlobalPosition(0,0,45);
	camTv.setFov(75);


	overlay.allocate(1920, 1080);
	overlay.begin();
	ofClear(0);
	overlay.end();
    
    
	// gestor ventanas
	//winManager.setup((ofxAppGLFWWindowMulti *)ofGetWindowPtr());
    //winManager.loadWindowSettings();



}

//--------------------------------------------------------------
void testApp::update(){
    // splashscreen
    if (ofGetFrameNum() == 1) {
		this->setup2();
	}
    
	// comprueba si la peli ya se acabo para volver a empezar
	// solo usar para debug luego comentar
	if(videoPanorama.videoplayer.getIsMovieDone()){
		 // reseteamos al inicio
        videoPanorama.resetPanorama();
        datosTelemetria.resetTelemetria();
	}

    // video panorama
    videoPanorama.update(datosTelemetria.getMotorData());
    
    
    // plataforma
    // vienen mapeados de la clase telemetria
    plataforma.setData(datosTelemetria.getPlatFormData().x, datosTelemetria.getPlatFormData().y);
    
    
    // telemetry
    if(videoPanorama.mustUpdate()){

		

        // nuevo frame del video, actualiza la telemetria
        datosTelemetria.update();

			overlay.begin();
			ofClear(0);
			//ofDisableDepthTest();
			//giramos 90 contrareloj
			ofPushMatrix();
			// video panorama
			camTv.begin(ofRectangle(0,0,1920,1080));
			ofRotateZ(90);
			videoPanorama.draw();


			camTv.end();
			ofPopMatrix();
		
			//ofPushStyle();
			// telemetry
			ofDisableDepthTest();
			datosTelemetria.drawOverlay();
			//ofEnableDepthTest();
			//ofPopStyle();
		
		
			ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
			ofSetColor(255,255,255,255);
			luto.draw(0,0);
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);

			overlay.end();

    }

}

//--------------------------------------------------------------
void testApp::draw(){
    
    if(!videoPanorama.videoplayer.isLoaded()) return;
    

	
		overlay.draw(0,0);
	


	cam.begin(ofRectangle(1920,0,1920,1080));
	cam.setVFlip(true);
		ofPushMatrix();

		oculusRift.beginLeftEye();
		videoPanorama.draw();
		oculusRift.endLeftEye();

		oculusRift.beginRightEye();
		videoPanorama.draw();
		oculusRift.endRightEye();

		oculusRift.draw();

		
		ofPopMatrix();
	cam.end();

	ofDrawBitmapString(ofToString(ofGetFrameRate()),10,10);
 }

///--------------------------------------------------------------
void testApp::exit(){
//	winManager.saveWindowSettings();
    plataforma.waitForThread();
}

///--------------------------------------------------------------
void testApp::keyPressed(int key){

    if(key == ' '){
        /// pausa en el video
        if(videoPanorama.videoplayer.isPlaying()){
            videoPanorama.videoplayer.setPaused(true);
        }else{
            videoPanorama.videoplayer.setPaused(false);
        }
    
    }else if(key == 'r'){
        // reseteamos al inicio
        videoPanorama.resetPanorama();
        datosTelemetria.resetTelemetria();
    }

}

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
