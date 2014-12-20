//
//  renderPanorama.cpp
//  parseTelemetria
//
//  Created by ignacio pedreira gonzalez on 12/07/14.
//
//

#include "renderPanorama.h"

///--------------------------------------------------------------
void renderPanorama::setup(string _urlVideo, string _urlNadir){
    
    // video
    videoplayer.loadMovie(_urlVideo);
	videoplayer.setLoopState(OF_LOOP_NONE);
	videoplayer.setPaused(true);
    //videoplayer.play();
    
    // casco
    casco.loadImage(_urlNadir);



    // rotacion
    roll = 0.0;
    
    
    // allocate texture and set textcoords for meshes
    int anchoVideo = 1894;
    int altoVideo = 948;


    videoTexture.allocate(anchoVideo, altoVideo, GL_RGB);

    createMeshWithTexture(anchoVideo, altoVideo);
}

///--------------------------------------------------------------
void renderPanorama::update(float _rollOffset){
    
    // video
    videoplayer.update();

    if(videoplayer.isFrameNew() && videoplayer.isLoaded()){
    
        videoTexture.loadData(videoplayer.getPixelsRef());
    }

    roll = _rollOffset;
}

///--------------------------------------------------------------
void renderPanorama::draw(){
    
    //sphere
    ofPushMatrix();

    ofRotateZ(roll*0.4);
    drawSphere();

    ofPopMatrix();
}

///--------------------------------------------------------------
bool renderPanorama::mustUpdate(){
    /*if(videoPanorama.videoplayer.isFrameNew() && videoPanorama.videoplayer.isLoaded()){}*/
    
    bool answer = false;
    if(videoplayer.isFrameNew() && videoplayer.isLoaded()) answer = true;
    
    return answer;
}

///--------------------------------------------------------------
void renderPanorama::resetPanorama(){
    
    // volvemos el video de la pano al inicio
    videoplayer.stop();
    roll = 0.0;
    videoplayer.play();
}
///--------------------------------------------------------------
void renderPanorama::drawSphere(){
    
	ofPushStyle();
	ofDisableDepthTest();
    ofSetColor(255,255,255,255);

    
        ofPushMatrix();

            videoTexture.bind();
            sphereMesh.draw();
            videoTexture.unbind();
    
        ofPopMatrix();
    
    
    
		
        ofPushMatrix();
            ofRotateX(-120);
			//
            casco.getTextureReference().bind();
            helmetMesh.draw();
            casco.getTextureReference().unbind();
			
        ofPopMatrix();
		
	ofEnableDepthTest();
	ofPopStyle();
    
}

///--------------------------------------------------------------
void renderPanorama::createMeshWithTexture(int _width, int _height){
	
	ofSpherePrimitive p = ofSpherePrimitive(100.0,60);
	sphereMesh = p.getMesh();
    
    
	for(int i = 0; i < sphereMesh.getNumVertices(); i++){
		ofVec2f texCoord = sphereMesh.getTexCoord(i);
        
        texCoord.x *= _width;
		texCoord.y  = (1.0 - texCoord.y) * _height;
        
		sphereMesh.setTexCoord(i, texCoord);
	}
    
	helmetMesh = sphereMesh;
}
