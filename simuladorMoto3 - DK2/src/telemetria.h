//
//  telemetria.h
//
//  Created by ignacio pedreira gonzalez on 08/07/14.
//
//

#ifndef __parseTelemetria__telemetria__
#define __parseTelemetria__telemetria__

#include "ofMain.h"
#include "ofxCsv.h"

#include "ofxSvg.h"

class frameTelemetria{
public:
    ofQuaternion rotacion;
    float rotRoll, rotPitch;
    float timeCurrent, speed, distance;
};

class telemetria {
    
public:
	void setup(string _urlCsv, float _offset, ofVideoPlayer * _videoPtr);
    void update();
    void drawOverlay();
    void toggleReadingData();
    void initVars();
    
    void setPath();
    
    void resetTelemetria();
    
    float getMotorData();
    
    
    
    ofVec2f getPlatFormData();
    
    ofQuaternion getQuaternion();
    
    bool isReading();
    int find(float _tb);
    
    int getNumRows();
    float getSpeed();
    float getTime();
    
private:
    wng::ofxCsv csv;
    vector<ofQuaternion> rotaciones;
    
    vector<frameTelemetria> datos;
    
    int numeroRegistros;
    int indexLectura;
    int indexOffset;
    bool readingData;
    
    float timeRunning;
    float timeOffset;
    
    float offset;
    ofTrueTypeFont fuente, fuenteMedio, fuenteSmall;
    
	ofVideoPlayer * videoPtr;
    
    // trazado circuito
    ofxSVG trazado;
    ofPolyline trazadoPoly;

	ofImage logoTelemetria, bandaTelemetria;
};

#endif /* defined(__parseTelemetria__telemetria__) */
