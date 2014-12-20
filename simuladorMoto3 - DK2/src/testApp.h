#pragma once

#include "ofMain.h"
#include "telemetria.h"
#include "renderPanorama.h"
#include "ckas2Dof.h"
#include "ofxSplashScreen.h"
#include "WindowManager.h"
#include "ofxOculusDK2.h"



class testApp : public ofBaseApp{

	public:
		void setup2(); // cabiado a setup2 por el splash screen
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxOculusDK2 oculusRift;
	
		ofMesh sphereMesh;
		telemetria datosTelemetria;
		renderPanorama videoPanorama;
		ckas2Dof plataforma;
    
		ofCamera cam, camTv;
    
		ofxSplashScreen splashScreen;

	//	WindowManager winManager;
		ofFbo overlay;

		ofImage luto;
};
