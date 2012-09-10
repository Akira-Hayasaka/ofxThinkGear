//
//  ofxThinkGear.h
//  ofxThinkGear
//
//  Created by Akira Hayasaka on 8/24/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef ofxThinkGear_ofxThinkGear_h
#define ofxThinkGear_ofxThinkGear_h

#include "ofMain.h"

const int TG_BAUDRATE = 9600;

// Data format for use with TG_Connect() and TG_SetDataFormat()
const int TG_STREAM_PACKETS = 0;
const int TG_STREAM_5VRAW = 1; 
const int TG_STREAM_FILE_PACKETS = 2;

// Data type that can be requested from TG_GetValue()
const int TG_DATA_BATTERY = 0;
const int TG_DATA_POOR_SIGNAL = 1; // 0 = good. others = poor.
const int TG_DATA_ATTENTION = 2; // min = 0. max = 100.
const int TG_DATA_MEDITATION = 3; // min = 0. max = 100.
const int TG_DATA_RAW = 4;
const int TG_DATA_DELTA = 5;
const int TG_DATA_THETA = 6;
const int TG_DATA_ALPHA1 = 7;
const int TG_DATA_ALPHA2 = 8;
const int TG_DATA_BETA1 = 9;
const int TG_DATA_BETA2 = 10;
const int TG_DATA_GAMMA1 = 11;
const int TG_DATA_GAMMA2 = 12;
const int TG_DATA_BLINK_STRENGTH = 37; // min = 0. max = 255.

class ofxThinkGear
{
public:
    
    ofxThinkGear()
    {
        thinkGearBundle = NULL;
        TG_GetDriverVersion = NULL;
        TG_GetNewConnectionId = NULL;
        TG_ReadPackets = NULL;
        TG_GetValue = NULL;
        TG_GetValueStatus = NULL;
        TG_Disconnect = NULL;
        TG_FreeConnection = NULL;
        TG_EnableBlinkDetection = NULL;
        tgID = 0;
        prevBlinkTime = 0;
        bEnableBlinkAsClick = false;
    }
    
    void setup(string deviceName = "/dev/tty.MindWaveMobile-DevA", int _id = 0)
    {     
        tgID = _id;
        
        bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                                  CFSTR("ThinkGear.bundle"), 
                                                  kCFURLPOSIXPathStyle, 
                                                  true);    
        thinkGearBundle = CFBundleCreate(kCFAllocatorDefault, bundleURL);
        
        if (!thinkGearBundle) 
        {
            ofLog(OF_LOG_FATAL_ERROR) << "Error: Could not find ThinkGear.bundle. Does it exist in the current directory?";
            exit(1);
        }    
        
        // set func ptrs
        TG_GetDriverVersion = (int(*)())CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_GetDriverVersion"));
        TG_GetNewConnectionId = (int(*)())CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_GetNewConnectionId"));
        TG_Connect = (int(*)(int, const char*, int, int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_Connect"));
        TG_ReadPackets = (int(*)(int, int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_ReadPackets"));        
        TG_GetValue = (float(*)(int, int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_GetValue"));        
        TG_GetValueStatus = (bool(*)(int, int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_GetValueStatus"));
        TG_Disconnect = (int(*)(int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_Disconnect"));
        TG_FreeConnection = (void(*)(int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_FreeConnection"));
        TG_EnableBlinkDetection = (int(*)(int, int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_EnableBlinkDetection"));
        
        if (!TG_GetDriverVersion || 
            !TG_GetNewConnectionId || 
            !TG_Connect || 
            !TG_ReadPackets || 
            !TG_GetValue || 
            !TG_Disconnect || 
            !TG_FreeConnection ||
            !TG_EnableBlinkDetection) 
        {
            ofLog(OF_LOG_FATAL_ERROR) << "Error: Expected functions in ThinkGear.bundle were not found.";
            exit(1);
        }     
        
        connectionID = TG_GetNewConnectionId();
        
        ofLog() << "connecting to " << deviceName << " ...";        
        int conResult = TG_Connect(connectionID, deviceName.c_str(), TG_BAUDRATE, TG_STREAM_PACKETS);        
        
        if (conResult != 0)
        {
            ofLog(OF_LOG_FATAL_ERROR) << "Error: Connection Failed!";
            exit(1);
        }
        
        int resBlink = TG_EnableBlinkDetection(connectionID, 1);
        if (resBlink != 0)
            ofLog() << "fail to enable blink detection";
    }
    
    void update()
    {
        int numPackets = TG_ReadPackets(connectionID, -1);
        if (numPackets > 0) 
        {
            signalQuality = TG_GetValue(connectionID, TG_DATA_POOR_SIGNAL);

            if (signalQuality == 0.0) 
            {
                if (TG_GetValueStatus(connectionID, TG_DATA_ATTENTION) != 0.0)
                {
                    float attention = TG_GetValue(connectionID, TG_DATA_ATTENTION);
                    ofNotifyEvent(attentionChangeEvent, attention);
                }
                if (TG_GetValueStatus(connectionID, TG_DATA_MEDITATION) != 0.0)
                {
                    float meditation = TG_GetValue(connectionID, TG_DATA_MEDITATION);
                    ofNotifyEvent(meditationChangeEvent, meditation);
                }   
                if (TG_GetValueStatus(connectionID, TG_DATA_BLINK_STRENGTH) != 0.0)
                {
                    float blink = TG_GetValue(connectionID, TG_DATA_BLINK_STRENGTH);
                    ofNotifyEvent(blinkChangeEvent, blink);
                    
                    if (bEnableBlinkAsClick)
                    {
                        bool fake = true;
                        
                        if (ofGetElapsedTimeMillis() - prevBlinkTime < 500)
                        {
                            ofNotifyEvent(doubleClickEvent, fake);
                        }
                        else
                        {
                            ofNotifyEvent(singleClickEvent, fake);
                        }
                        prevBlinkTime = ofGetElapsedTimeMillis();
                    }
                }                   
            }
        }        
    }
    
    void enableBlinkAsClick()
    {
        bEnableBlinkAsClick = true;
    }
    
    void disableBlinkAsClick()
    {
        bEnableBlinkAsClick = false;
    }
    
    float getSignalQuality()
    {
        return signalQuality;
    }
    
    int getID()
    {
        return tgID;
    }
 
    ofEvent<float> attentionChangeEvent;
    ofEvent<float> meditationChangeEvent;
    ofEvent<float> blinkChangeEvent;
    
    // blink as click event
    ofEvent<bool> singleClickEvent;
    ofEvent<bool> doubleClickEvent;
    
protected:
    
    // function pointers;
    int (*TG_GetDriverVersion)(); 
    int (*TG_GetNewConnectionId)();
    int (*TG_Connect)(int, const char *, int, int);
    int (*TG_ReadPackets)(int, int);
    float (*TG_GetValue)(int, int);
    bool (*TG_GetValueStatus)(int, int);
    int (*TG_Disconnect)(int);
    void (*TG_FreeConnection)(int);
    int (*TG_EnableBlinkDetection)(int, int);    
    
private:
    
    int tgID;
    
    CFURLRef bundleURL; 
    CFBundleRef thinkGearBundle;
    
    int connectionID;
    float signalQuality;
    
    bool bEnableBlinkAsClick;
    int prevBlinkTime;
};

#endif
