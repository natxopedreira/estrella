#include "ofMain.h"
#include "testApp.h"
//#include "ofxAppGLFWWindowMulti.h"
//========================================================================
int main( ){
	
	//////////////////////////////////////////////
	// oculta la ventanita esa de ms dos tan fea
	/*HWND handleWindow;
	AllocConsole();
	handleWindow = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(handleWindow, 0);*/

   // ofxAppGLFWWindowMulti * window = new ofxAppGLFWWindowMulti();

	ofAppGLFWWindow window;
	window.setMultiDisplayFullscreen(true);
    ofSetupOpenGL(&window, 1920*2, 1080, OF_FULLSCREEN);

	//ofSetupOpenGL(1920*2,1080, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my apps
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
