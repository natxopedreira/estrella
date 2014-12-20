//
//  ckas2Dof.h
//  simulador
//
//  Created by ignacio pedreira gonzalez on 20/07/14.
//
//

#ifndef __simulador__ckas2Dof__
#define __simulador__ckas2Dof__

#include "ofMain.h"
//#include "ofxSerial.h"

class ckas2Dof: public ofThread {
    
public:
    
    ckas2Dof();
    
    void setup();
    void drawDebug(int _x, int _y);
    void threadedFunction();
    void setData(int _pitch, int _roll);
    
    bool conect();
    
    ofSerial device;
    
    bool conectado;
    
    int pitch, roll;
    
    int offsetPitch, offsetRoll;
    
    float time;
    int count;
};

#endif /* defined(__simulador__ckas2Dof__) */
