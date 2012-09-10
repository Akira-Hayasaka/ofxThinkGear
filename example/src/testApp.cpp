#include "testApp.h"

void testApp::setup()
{
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    
    thinkGear.setup();
    ofAddListener(thinkGear.attentionChangeEvent, this, &testApp::attentionListener);
    ofAddListener(thinkGear.meditationChangeEvent, this, &testApp::meditationListener);    
    
    font.loadFont("font/DroidSansMono.ttf", 20);
    
    distAw = 0.0;
    prevAw = 0.0;
    currAw = 0.0;
    distMw = 0.0;
    prevMw = 0.0;
    currMw = 0.0;
    
    atChangeTime = 0.0;
    attention = 0.0;
    meChangeTime = 0.0;
    meditation = 0.0;
}

void testApp::update()
{
    thinkGear.update();
    
    float cur = ofGetElapsedTimef();
    float progress = cur - atChangeTime;
    progress = ofClamp(progress, 0.0, 1.0);
    if (progress == 0.0) prevAw = currAw;
    currAw = ofxTweenLite::tween(prevAw, distAw, progress, OF_EASE_SINE_INOUT);
    
    progress = cur - meChangeTime;
    progress = ofClamp(progress, 0.0, 1.0);
    if (progress == 0.0) prevMw = currMw;
    currMw = ofxTweenLite::tween(prevMw, distMw, progress, OF_EASE_SINE_INOUT);    
}

void testApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    
    ofPushStyle();
    ofSetColor(ofColor::black);
    string qStr = "";
    if (thinkGear.getSignalQuality() == 0.0)
    {
        qStr = "good";
    }
    else 
    {
        qStr = "poor (" + ofToString(thinkGear.getSignalQuality()) + ")";
    }
    font.drawString("signal quality = " + qStr, 10, 40);    
    ofPopStyle();
    
    
    ofPushMatrix();
    ofTranslate(0, 30);
    ofPushStyle();
    ofSetColor(ofColor::black);
    font.drawString("Attention", 10, ofGetHeight()/4 - 10);
    ofNoFill();
    ofRect(1, ofGetHeight()/4, ofGetWidth() - 2, 60);
    ofSetColor(ofColor::magenta, ofMap(currAw, 0.0, ofGetWidth(), 50, 255));
    ofFill();    
    ofRect(0, ofGetHeight()/4, currAw, 59);
    if (attention > 0.0)
    {
        ofSetColor(ofColor::black, ofMap(currAw, 0.0, ofGetWidth(), 50, 255));
        font.drawString(ofToString(attention), 10, ofGetHeight()/4 + 40);
    }
    ofPopStyle();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0, -30);
    ofPushStyle();
    ofSetColor(ofColor::black);
    font.drawString("Meditation", 10, ofGetHeight()-(ofGetHeight()/4) - 10); 
    ofNoFill();
    ofRect(1, ofGetHeight()-(ofGetHeight()/4), ofGetWidth() - 2, 60);    
    ofSetColor(ofColor::cyan, ofMap(currMw, 0.0, ofGetWidth(), 50, 255));
    ofFill();
    ofRect(0, ofGetHeight()-(ofGetHeight()/4), currMw, 59);
    if (meditation > 0.0)
    {
        ofSetColor(ofColor::black, ofMap(currMw, 0.0, ofGetWidth(), 50, 255));
        font.drawString(ofToString(meditation), 10, ofGetHeight()-(ofGetHeight()/4) + 40);
    }    
    ofPopStyle();
    ofPopMatrix();
    
    ofSetWindowTitle("fps = " + ofToString(ofGetFrameRate()));
}

void testApp::attentionListener(float &param)
{
    attention = param;
    distAw = ofMap(attention, 0.0, 100.0, 0, ofGetWidth());
    atChangeTime = ofGetElapsedTimef();
}

void testApp::meditationListener(float &param)
{
    meditation = param;
    distMw = ofMap(meditation, 0.0, 100.0, 0, ofGetWidth());
    meChangeTime = ofGetElapsedTimef();    
}


void testApp::keyPressed(int key){}
void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y){}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}