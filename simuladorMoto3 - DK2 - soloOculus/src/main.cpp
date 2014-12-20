#include "ofMain.h"
#include "testApp.h"
//#include "ofxAppGLFWWindowMulti.h"

DISPLAY_DEVICE GetPrimaryDevice()
{
    int index=0;
    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);

    while (EnumDisplayDevices(NULL, index++, &dd, 0))
    {
        if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) return dd;
    }
    return dd;
}

BOOL SetDisplayResolution(long degrees)
{
    DISPLAY_DEVICE dd = GetPrimaryDevice();
    DEVMODE dm;
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(DEVMODE);
    if (!EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm))
    {
        printf("EnumDisplaySettings failed:%d\n", GetLastError());
        return FALSE;
    }
    switch(degrees)
    {
    case 90:
        dm.dmOrientation = DMORIENT_PORTRAIT; //error C2065: 'DMDO_90' : undeclared identifier
        break;
    case 270:
        dm.dmOrientation = DMORIENT_LANDSCAPE; //error C2065: 'DMDO_90' : undeclared identifier
        break;
    default:
        break;

    }
    DWORD dwTemp = dm.dmPelsHeight;
    dm.dmPelsHeight = dm.dmPelsWidth;
    dm.dmPelsWidth = dwTemp;

    dm.dmFields = (DM_PELSWIDTH | DM_PELSHEIGHT);
    if (ChangeDisplaySettings(&dm, CDS_TEST) !=DISP_CHANGE_SUCCESSFUL)
    {
        printf("\nIllegal graphics mode: %d\n", GetLastError());
        return FALSE;
    }

    return (ChangeDisplaySettings(&dm, 0)==DISP_CHANGE_SUCCESSFUL);
}
//#define OF_VIDEO_PLAYER_WMFPLAYER

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
	//window.setMultiDisplayFullscreen(true);
    ofSetupOpenGL(&window, 1920, 1080, OF_WINDOW);

	//ofSetupOpenGL(1920*2,1080, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my apps
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
