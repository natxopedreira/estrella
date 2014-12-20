//
//  renderPanorama.h
//
//  Created by ignacio pedreira gonzalez on 12/07/14.
//
//

#ifndef __parseTelemetria__renderPanorama__
#define __parseTelemetria__renderPanorama__

#include "ofMain.h"


class renderPanorama {

public:
    
    void setup(string _urlVideo, string _urlNadir);
    void update(float _rollOffset);
    void draw();
    
    void resetPanorama();
    
    bool mustUpdate();
    
    ofVideoPlayer videoplayer;
    
private:
    void createMeshWithTexture(int _width, int _height);
    void drawSphere();
    
    // render stuff
    ofTexture videoTexture;
    ofImage casco;
    
    ofMesh sphereMesh, helmetMesh;
    
    float roll;
    
};

#endif /* defined(__parseTelemetria__renderPanorama__) */
