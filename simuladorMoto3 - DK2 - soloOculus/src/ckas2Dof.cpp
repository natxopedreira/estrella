//
//  ckas2Dof.cpp
//  simulador
//
//  Created by ignacio pedreira gonzalez on 20/07/14.
//
//

#include "ckas2Dof.h"

/*
    rango de valores de 324 - 700
    el centro es 512
    
    formato paquete
    N pitch     roll\n

*/

ckas2Dof::ckas2Dof(){
    
    
    conectado = false;

    pitch = 512;
    roll = 512;
    
    offsetPitch = 12;
    offsetRoll = 15;
    
    time = ofGetElapsedTimeMillis();
    count = 0;
}

//--------------------------------------------------------------
void ckas2Dof::setup(){
    
    if(conect()){
        ofLogWarning("plataforma ckas 2dof CONECTADA");
        conectado = true;
        
    }else{
        ofLogWarning("ERROR CONECTANDO plataforma ckas 2dof");
        conectado = false;
    }
}

//--------------------------------------------------------------
bool ckas2Dof::conect(){
    bool hayPlataforma  = false;
    /*
    //listamos las plataformas
    std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    
    //buscamos la ckas
    for (int i = 0; i < devicesInfo.size(); i++) {
        if(ofIsStringInString(devicesInfo.at(i).getPort(),"SLAB_USBtoUART")){
            
            // intentamos conectar
            hayPlataforma = device.setup("/dev/tty.SLAB_USBtoUART", 250000);
            
        }
    }
    */
    hayPlataforma = device.setup("COM19", 250000);
    
    return hayPlataforma;
}

//--------------------------------------------------------------
void ckas2Dof::threadedFunction(){
    while(isThreadRunning())
    {
        // Attempt to lock the mutex.  If blocking is turned on,
        if(lock())
        {
            float timeElapsed = ofGetElapsedTimeMillis()-time;
            
            if(timeElapsed>1000){
                
                count = 0;
                time = ofGetElapsedTimeMillis();
            }else{
                if(ofGetElapsedTimeMillis()%10 && count < 100){
                    
                    if(device.isInitialized()){
                    
                        unsigned char buffer[14] = {'N',
                            32,
                            '0'+ pitch / 100%10,
                            '0'+ pitch / 10%10,
                            '0' + pitch %10,
                            32,
                            32,
                            32,
                            32,
                            32,
                            '0'+ roll / 100%10,
                            '0'+ roll / 10%10,
                            '0' + roll %10,
                            13};
                        
                        device.writeBytes(&buffer[0], 14);
                    
                    
                    }
                    
                    count++;
                }
            }
            
            // Unlock the mutex.  This is only
            // called if lock() returned true above.
            unlock();
            
            // Sleep for 1 second.
            sleep(5);
        }
        else
        {
            // If we reach this else statement, it means that we could not
            // lock our mutex, and so we do not need to call unlock().
            // Calling unlock without locking will lead to problems.
            ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
        }
    }
}

//--------------------------------------------------------------
void ckas2Dof::setData(int _pitch, int _roll){
    //ofScopedLock lock(mutex);
    
    /// nos aseguramos que el valor este entre 324 y 700
        // pitch
        // el valor que nos mandan + el offset
        int totalPitch = _pitch + offsetPitch;
    
        // clamp para los valores maximos y minimo
        if(totalPitch > 700){
            totalPitch = 700;
        }if (totalPitch < 324) {
            totalPitch = 324;
        }
    
    
        // roll
        // el valor que nos mandan + el offset
        int totalRoll = _roll + offsetRoll;
    
        // clamp para los valores maximos y minimo
        if(totalRoll > 700){
            totalRoll = 700;
        }if (totalRoll < 324) {
            totalRoll = 324;
        }
    
    
        pitch = totalPitch;
        roll = totalRoll;
        
}

//--------------------------------------------------------------
void ckas2Dof::drawDebug(int _x, int _y){
    string cadena = "DATOS SALIENDO A LA PLATAFORMA\n\nroll:: " + ofToString(roll) + "\npitch:: " + ofToString(pitch);
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    ofDrawBitmapString(cadena,  10, 0);
    
    ofPopMatrix();
}
