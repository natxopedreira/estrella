#include "ofMain.h"
#include "testApp.h"
#include "ofxAppGLFWWindowMulti.h"
//========================================================================
int main( ){
	
	//////////////////////////////////////////////
	// oculta la ventanita esa de ms dos tan fea
	/*HWND handleWindow;
	AllocConsole();
	handleWindow = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(handleWindow, 0);*/

    ofxAppGLFWWindowMulti * window = new ofxAppGLFWWindowMulti();
	ofSetupOpenGL(window, 1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
