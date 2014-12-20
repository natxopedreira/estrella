#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup2(){
    ofBackground(100);
    ofSetVerticalSync(false);
    
    
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
    

    // camera
    cam.begin();
    cam.end();
    cam.setGlobalPosition(0,0,0);

    camTv.begin();
    camTv.end();
    camTv.setGlobalPosition(0,0,45);
	camTv.setFov(75);
	///oculus
	oculusRift.baseCamera = &cam;
	oculusRift.setup();



    
	// gestor ventanas
	winManager.setup((ofxAppGLFWWindowMulti *)ofGetWindowPtr());
    winManager.loadWindowSettings();



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
    }

}

//--------------------------------------------------------------
void testApp::draw(){
    
    if(!videoPanorama.videoplayer.isLoaded()) return;
    
    if( winManager.getActiveWindowNo() == 0){
		// ventana principal
		// tv con los datos de telemetria
		ofBackground(0);
		
		//giramos 90 contrareloj
		ofPushMatrix();
		// video panorama
		camTv.begin();
		ofRotateZ(90);
		videoPanorama.draw();
		camTv.end();
		ofPopMatrix();

		// telemetry
		datosTelemetria.drawOverlay();

		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		ofSetColor(255,255,255,255);
		luto.draw(0,0);
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);

		//plataforma
		//plataforma.drawDebug(900, 40);
	
	}else{
        // ventana de las oculus
		ofBackground(0);
		ofPushMatrix();


		oculusRift.beginLeftEye();
		videoPanorama.draw();
		oculusRift.endLeftEye();

		oculusRift.beginRightEye();
		videoPanorama.draw();
		oculusRift.endRightEye();


		ofPopMatrix();
    }

 }

///--------------------------------------------------------------
void testApp::exit(){
	winManager.saveWindowSettings();
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
