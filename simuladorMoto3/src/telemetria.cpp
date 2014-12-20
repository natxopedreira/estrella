//
//  telemetria.cpp
//  parseTelemetria
//
//  Created by ignacio pedreira gonzalez on 08/07/14.
//
//

#include "telemetria.h"


//--------------------------------------------------------------
void telemetria::setup(string _urlCsv, float _offset, ofVideoPlayer * _videoPtr){
    /*
     para ESCENA-02-COLOR.mp4 el offset es 29
     */
    
    // cargamos el csv
    csv.loadFile(ofToDataPath(_urlCsv));
    
    
    // cargamos el trazado
    setPath();
    
    // pointer to main videoplayer
    videoPtr = _videoPtr;
    
    
    
    // primer valor vacio
    frameTelemetria ft0;
    
    ft0.rotacion = ofQuaternion(0, ofVec3f(0, 0, 1)) * ofQuaternion(0, ofVec3f(1, 0, 0)) * ofQuaternion(0, ofVec3f(1, 0, 1)); /// order its important roll*pitch*yaw
    
    ft0.speed = 0;
    ft0.distance = 0;
    ft0.timeCurrent = 0;
    
    // rotaciones en angulo
    ft0.rotRoll = 0;
    ft0.rotPitch = 0;
    
    datos.push_back(ft0);
    
    //parseamos el csv
    for(int i=0; i<csv.numRows; i++) {
        
            float roll = csv.getFloat(i,3);
            float pitch = csv.getFloat(i,4);
            float heading = 0.0f;
        
            ofQuaternion qr (roll, ofVec3f(0, 0, 1));	// quat roll.
            ofQuaternion qp (pitch, ofVec3f(1, 0, 0));	// quat pitch.
            ofQuaternion qh (heading, ofVec3f(1, 0, 1));	// quat heading or yaw.
        
        
            frameTelemetria ft;
        
            ft.rotacion = qr * qp * qh; /// order its important roll*pitch*yaw
        
            ft.speed = csv.getFloat(i,2);
            ft.distance = csv.getFloat(i,1);
            ft.timeCurrent = csv.getFloat(i,0);
        
            // rotaciones en angulo
            ft.rotRoll = csv.getFloat(i,3);
            ft.rotPitch = csv.getFloat(i,4);
        
            datos.push_back(ft);
    }
   

    //numero de registros
    numeroRegistros = csv.numRows;
    
    // fuente
    fuenteSmall.loadFont("HelveticaWorld-BoldItalic.ttf", 24);
	fuente.loadFont("DS-DIGIB.ttf", 160);
	fuenteMedio.loadFont("DS-DIGIB.ttf", 70);
    
    // offsetTelemetria
    offset = _offset;
    
    //cerramos el csv
    csv.clear();
    
    // start init vars
    initVars();
    
	logoTelemetria.loadImage("logo_telemetria.png");
	bandaTelemetria.loadImage("fondo_telemetria.png");
	
}

//--------------------------------------------------------------
void telemetria::update(){

    
	timeRunning = floorf((videoPtr->getPosition()*videoPtr->getDuration()) * 100 + 0.5) / 100;
    //timeRunning = floorf(videoPtr->getPositionInSeconds() * 100 + 0.5) / 100;
    
    if(timeRunning<offset) return;
    
    if(indexOffset == 0){
        indexOffset = videoPtr->getCurrentFrame();
        timeOffset = timeRunning;
    }
   
	float indexBuscado = find(((videoPtr->getCurrentFrame()-indexOffset)/25.0));
    
    // si hay data para ese index
    if(indexBuscado<datos.size()-1 && indexBuscado>=0){
        indexLectura = indexBuscado;
    }
}

//--------------------------------------------------------------
float telemetria::getMotorData(){
    float valor = 0.0;
    /// comprueba que haya datos
    if(datos.size()>0) valor = datos.at(indexLectura).rotRoll;
    
    ///// FALTA CONVERTIR LOS  A FORMATO MOTORES CKAS

    return valor;
}

//--------------------------------------------------------------
ofVec2f telemetria::getPlatFormData(){
    ofVec2f salida = ofVec2f(512, 512);
    
    // rango de valores de la plataforma ******** 324 - 700 ***********
    
    if(datos.size()>0){
        
        int pitchVal = ofMap(datos.at(indexLectura).rotPitch, -8, 8, 324, 700, true);
        int rollVal = ofMap(datos.at(indexLectura).rotRoll, -50, 50, 324, 700, true);
        
        salida.set(pitchVal, rollVal);
    }
    
    return salida;
}


//--------------------------------------------------------------
ofQuaternion telemetria::getQuaternion(){
    return datos.at(indexLectura).rotacion;
}

//--------------------------------------------------------------

void telemetria::setPath(){
    
    // cargamoz el svg
    trazado.load("circuito.svg");
    ofPath p = trazado.getPathAt(0);
    
    vector<ofPolyline> lines = p.getOutline();
    
    
    
    ofPolyline ap;
    
    for(int i = 0; i < lines.size(); i ++){
        
        /// para cada polilinea
        ofPolyline pl = lines.at(i);
        //std::reverse(pl.getVertices().begin(),pl.getVertices().end());
        
        for(int j = 0; j < pl.getVertices().size(); j++){
            
            ap.addVertex(pl.getVertices().at(j));
            
        }
    }
    
    
    trazadoPoly = ap.getResampledByCount(2550);
}

//--------------------------------------------------------------
int telemetria::find(float _tb){
    int salida = -1;
    for (int i = 0; i < datos.size(); i++) {
        float dato = datos.at(i).timeCurrent;
        if( dato == _tb){
            salida = i;
            break;
        }
    }
    return salida;
}

//--------------------------------------------------------------
void telemetria::resetTelemetria(){
    // reseteamos las variables para volver a empezar
    
    initVars();
}

//--------------------------------------------------------------
void telemetria::drawOverlay(){
    
    /// comprueba que haya datos
    if(datos.size()<=0) return ;


    ofPushMatrix();
	ofPushStyle();

	// logo estrella
	ofSetColor(255,255,255,255);
	logoTelemetria.draw(1680,380);


	// banda gris
	ofSetColor(255,255,255,50);
	bandaTelemetria.draw(50,0);
	




	//////////////////////////////////////////////
	//////////////////////////////////////////////
	// speed display
	//////////////////////////////////////////////
    ofPushMatrix();
    ofTranslate(205, 140);
	ofRotateZ(-90);

	ofSetColor(200, 200, 200, 80);
    fuente.drawString("000", -218, 0);

	ofSetColor(50, 50, 50, 100);
	fuenteSmall.drawString("km/h", -300, -110);

    ofSetColor(255, 255, 255);
    string str  = ofToString((int)datos.at(indexLectura).speed);

	size_t length = str.length();
	char * letters = new char[length];


	strcpy(letters, str.c_str());

	switch (length)
	{

	case 1:
			if(ofToString(letters[0])=="1"){
				fuente.drawString(ofToString(letters[0]), 56, 0);
			}else{
				fuente.drawString(ofToString(letters[0]), 0, 0);
			}
			
		break;

	case 2:
			if(ofToString(letters[0])=="1"){
				fuente.drawString(ofToString(letters[0]), -52, 0);
			}else{
				fuente.drawString(ofToString(letters[0]), -110, 0);
			}

			if(ofToString(letters[1])=="1"){
				fuente.drawString(ofToString(letters[1]), 58, 0);
			}else{
				fuente.drawString(ofToString(letters[1]), 0, 0);
			}

		break;

	case 3:
			if(ofToString(letters[0])=="1"){
				fuente.drawString(ofToString(letters[0]), -160, 0);
			}else{
				fuente.drawString(ofToString(letters[0]), -220, 0);
			}
			if(ofToString(letters[1])=="1"){
				fuente.drawString(ofToString(letters[1]), -52, 0);
			}else{
				fuente.drawString(ofToString(letters[1]), -110, 0);
			}

			if(ofToString(letters[2])=="1"){
				fuente.drawString(ofToString(letters[2]), 58, 0);
			}else{
				fuente.drawString(ofToString(letters[2]), 0, 0);
			}

		break;
	default:
		break;
	}

    ofPopMatrix();
    
    


	//////////////////////////////////////////////
	//////////////////////////////////////////////
	// trazado del circuito
	//////////////////////////////////////////////

	/*
	si tiene 5110m pues lo mapeamos al 100% del circuito
	
	*/

    ofPushMatrix();
	
    ofTranslate(-220, 1360);
	ofScale(1.2,1.2,1.0);
	ofRotateZ(-90);


	/// todo el trazado en blanco
    ofSetColor(255,255,255);
    for(int k = 0; k < trazadoPoly.getVertices().size(); k++){
        ofCircle(trazadoPoly.getVertices().at(k).x-2, trazadoPoly.getVertices().at(k).y-2, 4);
    }
    



	/// trazado recorrido
    ofSetColor(255,0,0);
	float metrosActuales = datos.at(indexLectura).distance;
	
	if(metrosActuales<5110.0){
		float posRelativa = metrosActuales / 5110.0;

		int indexAtPos = trazadoPoly.getIndexAtPercent(posRelativa);

		for(int l = 0; l < indexAtPos; l++){
            ofCircle(trazadoPoly.getVertices().at(l).x-2, trazadoPoly.getVertices().at(l).y-2, 4);
        }

		ofCircle(trazadoPoly.getVertices().at(indexAtPos).x-3, trazadoPoly.getVertices().at(indexAtPos).y-3, 6);
	}

    ofPopMatrix();



	//////////////////////////////////////////////
	//////////////////////////////////////////////
	// distance
	//////////////////////////////////////////////
    ofPushMatrix();
    ofTranslate(184, 640);
	ofRotateZ(-90);
	ofSetColor(200, 200, 200, 80);
    fuenteMedio.drawString("0000", -40, 20);

    ofSetColor(255,255,255);
    fuenteMedio.drawString(ofToString((int)datos.at(indexLectura).distance), -40, 20);
    ofPopMatrix();




	//////////////////////////////////////////////
	//////////////////////////////////////////////
    //////////// time display ///////////////////
	//////////////////////////////////////////////
    ofPushMatrix();
    ofTranslate(110, 640);
	ofRotateZ(-90);

	ofSetColor(200, 200, 200, 80);
    fuenteMedio.drawString("000", -41, 20);

    ofSetColor(255,0,0);
	string strTime  = ofToString((int)datos.at(indexLectura).timeCurrent);

	size_t lengthTime = strTime.length();
	char * lettersTime = new char[lengthTime];

	strcpy(lettersTime, strTime.c_str());

	switch (lengthTime)
	{

	case 1:
			if(ofToString(lettersTime[0])=="1"){
				fuenteMedio.drawString(ofToString(lettersTime[0]), 81, 20);
			}else{
				fuenteMedio.drawString(ofToString(lettersTime[0]), 55, 20);
			}
			
		break;

	case 2:
			if(ofToString(lettersTime[0])=="1"){
				fuenteMedio.drawString(ofToString(lettersTime[0]), 33, 20);
			}else{
				fuenteMedio.drawString(ofToString(lettersTime[0]), 8, 20);
			}

			if(ofToString(lettersTime[1])=="1"){
				fuenteMedio.drawString(ofToString(lettersTime[1]), 81, 20);
			}else{
				fuenteMedio.drawString(ofToString(lettersTime[1]), 55, 20);
			}

		break;

	case 3:
			if(ofToString(lettersTime[0])=="1"){
				fuenteMedio.drawString(ofToString(lettersTime[0]), -15, 20);
			}else{
				fuenteMedio.drawString(ofToString(lettersTime[0]), -40, 20);
			}
			if(ofToString(lettersTime[1])=="1"){
				fuenteMedio.drawString(ofToString(lettersTime[1]), 33, 20);
			}else{
				fuenteMedio.drawString(ofToString(lettersTime[1]), 8, 20);
			}

			if(ofToString(lettersTime[2])=="1"){
				fuenteMedio.drawString(ofToString(lettersTime[2]), 81, 20);
			}else{
				fuenteMedio.drawString(ofToString(lettersTime[2]), 56,20);
			}

		break;
	default:
		break;
	}

    ofPopMatrix();

  

    /*
    // roll display --- 100x100
    ofPushMatrix();
    ofTranslate(90, 60);
    ofDrawBitmapStringHighlight("ROLL " + ofToString(datos.at(indexLectura).rotRoll), -40, -15);
    ofRotateZ(datos.at(indexLectura).rotRoll);
    ofSetColor(255, 0, 0);
    ofRect(-50, 2, 100, 5);
    ofPopMatrix();
    
    
    
    // pitch display --- 100x100
    ofPushMatrix();
    ofTranslate(250, 60);
    ofDrawBitmapStringHighlight("PITCH " + ofToString(datos.at(indexLectura).rotPitch), -40, -15);
    ofRotateZ(datos.at(indexLectura).rotPitch);
    ofSetColor(0, 255, 0);
    ofRect(-50, 2, 100, 5);
    ofPopMatrix();
    
    
    
    // speed display
    ofPushMatrix();
    ofTranslate(400, 60);
    ofDrawBitmapStringHighlight("SPEED KM/H", -40, -15);
    ofSetColor(255, 0, 0);
    fuente.drawString(ofToString(datos.at(indexLectura).speed), -40, 20);
    ofPopMatrix();
    

    
    // time display
    ofPushMatrix();
    ofTranslate(550, 60);
    ofDrawBitmapStringHighlight("INDEX LECTURA", -40, -15);
    ofSetColor(255, 0, 0);
    fuente.drawString(ofToString(indexLectura), -40, 20);
    ofPopMatrix();
    
    
    // time running
    ofPushMatrix();
    ofTranslate(700, 60);
    ofDrawBitmapStringHighlight("TIME RUNNING", -40, -15);
    ofSetColor(255, 0, 0);
    fuente.drawString(ofToString(timeRunning), -40, 20);
    ofPopMatrix();
    
    
    // app fps
    ofPushMatrix();
    ofTranslate(850, 60);
    ofDrawBitmapStringHighlight("APP FPS", -40, -15);
    ofSetColor(255, 0, 0);
    fuente.drawString(ofToString((int)ofGetFrameRate()), -40, 20);
    ofPopMatrix();
    
    
    // distance
    ofPushMatrix();
    ofTranslate(950, 60);
    ofDrawBitmapStringHighlight("DISTANCE", -40, -15);
    ofSetColor(255, 0, 0);
    fuente.drawString(ofToString(datos.at(indexLectura).distance), -40, 20);
    ofPopMatrix();
    
    
    
    // trazado del circuito
    ofSetColor(255,255,255);
    for(int k = 0; k < trazadoPoly.getVertices().size(); k++){
        ofCircle(trazadoPoly.getVertices().at(k).x, trazadoPoly.getVertices().at(k).y, 2);
    }
    
    ofSetColor(255,0,0);
    ofCircle(trazadoPoly.getVertices().at(0).x, trazadoPoly.getVertices().at(0).y, 2);
    
    if((int)datos.at(indexLectura).distance/2 < trazadoPoly.getVertices().size()){
        
        for(int l = 0; l < (int)datos.at(indexLectura).distance/2.2; l++){
            ofCircle(trazadoPoly.getVertices().at(l).x, trazadoPoly.getVertices().at(l).y, 2);
        }
    }

    
    
    
    
    
    
    ofPopStyle();
    
    // graph display
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(0, 160);
    
    int start = indexLectura-0;
    
    int x = 0;
    if(start < 0){
        ofTranslate(-start, 0);
        start = 0;
    }
    
    float amp = 1;
    int end = start+ofGetWidth();
    
    ofPolyline polyRoll,polyPitch;
    for (int i = start; i < end && i < datos.size(); i++ ) {
        
        polyRoll.addVertex(ofPoint(x,datos.at(i).rotRoll*amp - 25));
        polyPitch.addVertex(ofPoint(x,datos.at(i).rotPitch*amp ));
        
        x++;
    }
    
    ofSetColor(255, 0, 0);
    polyRoll.draw();
    
    ofSetColor(0, 255, 0);
    polyPitch.draw();
    

    
    ofPopStyle();
    ofPopMatrix();
    
    */
	ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void telemetria::initVars(){

    // no estamos pidiendo datos
    readingData = false;
    
    // index de lectura a 0
    indexLectura = 0;
    
    //
    timeRunning = 0.0;
    timeOffset = 0.0;
    indexOffset = 0;
}

//--------------------------------------------------------------
bool telemetria::isReading(){
    return readingData;
}

//--------------------------------------------------------------
void telemetria::toggleReadingData(){
    readingData = !readingData;
}

//--------------------------------------------------------------
int telemetria::getNumRows(){
    return numeroRegistros;
    
}
//--------------------------------------------------------------
float telemetria::getSpeed(){
    return datos.at(indexLectura).speed;
}
//--------------------------------------------------------------
float telemetria::getTime(){
    return datos.at(indexLectura).timeCurrent;
}
