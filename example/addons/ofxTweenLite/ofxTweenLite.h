//
//  ofxTweenLite.h
//  emptyExample
//
//  Created by lukasz karluk on 6/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

//----------------------------------------------------------------------- ofxEasing
class ofxEasing {
public:
    virtual float easeIn    ( float t, float b, float c, float d )=0;
    virtual float easeOut   ( float t, float b, float c, float d )=0;
    virtual float easeInOut ( float t, float b, float c, float d )=0;
};

//----------------------------------------------------------------------- ofxEasingBack
class ofxEasingBack: public ofxEasing {
public:
    float easeIn (float t,float b , float c, float d) {
        float s = 1.70158f;
        float postFix = t/=d;
        return c*(postFix)*t*((s+1)*t - s) + b;
    }
    float easeOut(float t,float b , float c, float d) {
        float s = 1.70158f;
        return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b;
    }
    float easeInOut(float t,float b , float c, float d) {
        float s = 1.70158f;
        if ((t/=d/2) < 1) return c/2*(t*t*(((s*=(1.525f))+1)*t - s)) + b;
        float postFix = t-=2;
        return c/2*((postFix)*t*(((s*=(1.525f))+1)*t + s) + 2) + b;
    }
};

//----------------------------------------------------------------------- ofxEasingBack
class ofxEasingBounce: public ofxEasing {
public:
    float easeIn (float t,float b , float c, float d) {
        return c - easeOut (d-t, 0, c, d) + b;
    }
    float easeOut(float t,float b , float c, float d) {
        if ((t/=d) < (1/2.75f)) {
            return c*(7.5625f*t*t) + b;
        } else if (t < (2/2.75f)) {
            float postFix = t-=(1.5f/2.75f);
            return c*(7.5625f*(postFix)*t + .75f) + b;
        } else if (t < (2.5/2.75)) {
			float postFix = t-=(2.25f/2.75f);
            return c*(7.5625f*(postFix)*t + .9375f) + b;
        } else {
            float postFix = t-=(2.625f/2.75f);
            return c*(7.5625f*(postFix)*t + .984375f) + b;
        }
    }
    float easeInOut(float t,float b , float c, float d) {
        if (t < d/2) return easeIn (t*2, 0, c, d) * .5f + b;
        else return easeOut (t*2-d, 0, c, d) * .5f + c*.5f + b;
    }
};

//----------------------------------------------------------------------- ofxEasingCirc
class ofxEasingCirc: public ofxEasing {
public:
    float easeIn (float t,float b , float c, float d) {
        return -c * (sqrt(1 - (t/=d)*t) - 1) + b;
    }
    float easeOut(float t,float b , float c, float d) {
        return c * sqrt(1 - (t=t/d-1)*t) + b;
    }
    float easeInOut(float t,float b , float c, float d) {
        if ((t/=d/2) < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
        return c/2 * (sqrt(1 - t*(t-=2)) + 1) + b;
    }
};

//----------------------------------------------------------------------- ofxEasingCubic
class ofxEasingCubic: public ofxEasing {
public:
    float easeIn (float t,float b , float c, float d) {
        return c*(t/=d)*t*t + b;
    }
    float easeOut(float t,float b , float c, float d) {
        return c*((t=t/d-1)*t*t + 1) + b;
    }
    float easeInOut(float t,float b , float c, float d) {
        if ((t/=d/2) < 1) return c/2*t*t*t + b;
        return c/2*((t-=2)*t*t + 2) + b;
    }
};

//----------------------------------------------------------------------- ofxEasingElastic
class ofxEasingElastic: public ofxEasing {
public:
    float easeIn (float t,float b , float c, float d) {
        if (t==0) return b;  if ((t/=d)==1) return b+c;
        float p=d*.3f;
        float a=c;
        float s=p/4;
        float postFix =a*pow(2,10*(t-=1)); // this is a fix, again, with post-increment operators
        return -(postFix * sin((t*d-s)*(2*PI)/p )) + b;
    }
    float easeOut(float t,float b , float c, float d) {
        if (t==0) return b;  if ((t/=d)==1) return b+c;
        float p=d*.3f;
        float a=c;
        float s=p/4;
        return (a*pow(2,-10*t) * sin( (t*d-s)*(2*PI)/p ) + c + b);
    }
    float easeInOut(float t,float b , float c, float d) {
        if (t==0) return b;  if ((t/=d/2)==2) return b+c;
        float p=d*(.3f*1.5f);
        float a=c;
        float s=p/4;
        if (t < 1) {
            float postFix =a*pow(2,10*(t-=1)); // postIncrement is evil
            return -.5f*(postFix* sin( (t*d-s)*(2*PI)/p )) + b;
        }
        float postFix =  a*pow(2,-10*(t-=1)); // postIncrement is evil
        return postFix * sin( (t*d-s)*(2*PI)/p )*.5f + c + b;
    }
};

//----------------------------------------------------------------------- ofxEasingExpo
class ofxEasingExpo: public ofxEasing {
public:
    float easeIn (float t,float b , float c, float d) {
        return (t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b;
    }
    float easeOut(float t,float b , float c, float d) {
        return (t==d) ? b+c : c * (-pow(2, -10 * t/d) + 1) + b;
    }
    float easeInOut(float t,float b , float c, float d) {
        if (t==0) return b;
        if (t==d) return b+c;
        if ((t/=d/2) < 1) return c/2 * pow(2, 10 * (t - 1)) + b;
        return c/2 * (-pow(2, -10 * --t) + 2) + b;
    }
};

//----------------------------------------------------------------------- ofxEasingLinear
class ofxEasingLinear: public ofxEasing {
public:
    float easeIn (float t,float b , float c, float d) {
        return c*t/d + b;
    }
    float easeOut(float t,float b , float c, float d) {
        return c*t/d + b;
    }
    float easeInOut(float t,float b , float c, float d) {
        return c*t/d + b;
    }
};

//----------------------------------------------------------------------- ofxEasingQuad
class ofxEasingQuad: public ofxEasing {
public:
    float easeIn (float t,float b , float c, float d) {
        return c*(t/=d)*t + b;
    }
    float easeOut(float t,float b , float c, float d) {
        return -c *(t/=d)*(t-2) + b;
    }
    
    float easeInOut(float t,float b , float c, float d) {
        if ((t/=d/2) < 1) return c/2*t*t + b;
        return -c/2 * ((--t)*(t-2) - 1) + b;
    }
};

//----------------------------------------------------------------------- ofxEasingQuart
class ofxEasingQuart: public ofxEasing {
public:
    float easeIn (float t,float b , float c, float d) {
        return c*(t/=d)*t*t*t + b;
    }
    float easeOut(float t,float b , float c, float d) {
        return -c * ((t=t/d-1)*t*t*t - 1) + b;
    }
    float easeInOut(float t,float b , float c, float d) {
        if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
        return -c/2 * ((t-=2)*t*t*t - 2) + b;
    }
};

//----------------------------------------------------------------------- ofxEasingQuint
class ofxEasingQuint: public ofxEasing {
public:
    float easeIn (float t,float b , float c, float d) {
        return c*(t/=d)*t*t*t*t + b;
    }
    float easeOut(float t,float b , float c, float d) {
        return c*((t=t/d-1)*t*t*t*t + 1) + b;
    }
    float easeInOut(float t,float b , float c, float d) {
        if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
        return c/2*((t-=2)*t*t*t*t + 2) + b;
    }
};

//----------------------------------------------------------------------- ofxEasingSine
class ofxEasingSine: public ofxEasing {
public:
    float easeIn (float t,float b , float c, float d) {
        return -c * cos(t/d * (PI/2)) + c + b;
    }
    float easeOut(float t,float b , float c, float d) {
        return c * sin(t/d * (PI/2)) + b;
    }
    float easeInOut(float t,float b , float c, float d) {
        return -c/2 * (cos(PI*t/d) - 1) + b;
    }
};

//----------------------------------------------------------------------- ofxTweenLite

enum ofEaseFunction
{
    OF_EASE_BACK_IN         = 0,
    OF_EASE_BACK_OUT        = 1,
    OF_EASE_BACK_INOUT      = 2,
    OF_EASE_BOUNCE_IN       = 3,
    OF_EASE_BOUNCE_OUT      = 4,
    OF_EASE_BOUNCE_INOUT    = 5,
    OF_EASE_CIRC_IN         = 6,
    OF_EASE_CIRC_OUT        = 7,
    OF_EASE_CIRC_INOUT      = 8,
    OF_EASE_CUBIC_IN        = 9,
    OF_EASE_CUBIC_OUT       = 10,
    OF_EASE_CUBIC_INOUT     = 11,
    OF_EASE_ELASTIC_IN      = 12,
    OF_EASE_ELASTIC_OUT     = 13,
    OF_EASE_ELASTIC_INOUT   = 14,
    OF_EASE_EXPO_IN         = 15,
    OF_EASE_EXPO_OUT        = 16,
    OF_EASE_EXPO_INOUT      = 17,
    OF_EASE_LINEAR_IN       = 18,
    OF_EASE_LINEAR_OUT      = 19,
    OF_EASE_LINEAR_INOUT    = 20,
    OF_EASE_QUAD_IN         = 21,
    OF_EASE_QUAD_OUT        = 22,
    OF_EASE_QUAD_INOUT      = 23,
    OF_EASE_QUART_IN        = 24,
    OF_EASE_QUART_OUT       = 25,
    OF_EASE_QUART_INOUT     = 26,
    OF_EASE_QUINT_IN        = 27,
    OF_EASE_QUINT_OUT       = 28,
    OF_EASE_QUINT_INOUT     = 29,
    OF_EASE_SINE_IN         = 30,
    OF_EASE_SINE_OUT        = 31,
    OF_EASE_SINE_INOUT      = 32
};

typedef float (ofxEasing::*ofxEasingMemFn)( float t, float b, float c, float d );   // typedef of a pointer to ofxEasing member function.
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

class ofxTweenLite
{
public:
    
    ofxTweenLite ()
    {
        setEasingFunction( OF_EASE_LINEAR_IN );
    }
    
    //-------------------------------------------------------------------------------------- static method for calling tweens.
    static float tween ( float from, float to, float progress, ofEaseFunction easingFunc )
    {
        ofxTweenLite tween;
        return tween.go( from, to, progress, easingFunc );
    }
    
    //-------------------------------------------------------------------------------------- object method for calling tweens.
    float go ( float from, float to, float progress, ofEaseFunction easingFunc )
    {
        setEasingFunction( easingFunc );
        
        progress = ofClamp( progress, 0, 1 );
        
        float t = progress;
        float b = from;
        float c = to - from;
        float d = 1.0;
        
        return CALL_MEMBER_FN( *easing, ptFunc )( t, b, c, d );
    }
    
    //-------------------------------------------------------------------------------------- selecting easing function.
    void setEasingFunction ( ofEaseFunction easeFunc )
    {
        if( easingFunc == easeFunc )
            return;
        
        //------------------------------------------------- back.
        if( easeFunc == OF_EASE_BACK_IN ) { 
            easing = &easingBack;
            ptFunc = &ofxEasing :: easeIn;
        } else if( easeFunc == OF_EASE_BACK_OUT ) { 
            easing = &easingBack;
            ptFunc = &ofxEasing :: easeOut;
        } else if( easeFunc == OF_EASE_BACK_INOUT ) { 
            easing = &easingBack;
            ptFunc = &ofxEasing :: easeInOut;
        //------------------------------------------------- bounce.
        } else if( easeFunc == OF_EASE_BOUNCE_IN ) { 
            easing = &easingBounce;
            ptFunc = &ofxEasing :: easeIn;
        } else if( easeFunc == OF_EASE_BOUNCE_OUT ) { 
            easing = &easingBounce;
            ptFunc = &ofxEasing :: easeOut;
        } else if( easeFunc == OF_EASE_BOUNCE_INOUT ) { 
            easing = &easingBounce;
            ptFunc = &ofxEasing :: easeInOut;
        //------------------------------------------------- circ.
        } else if( easeFunc == OF_EASE_CIRC_IN ) { 
            easing = &easingCirc;
            ptFunc = &ofxEasing :: easeIn;
        } else if( easeFunc == OF_EASE_CIRC_OUT ) { 
            easing = &easingCirc;
            ptFunc = &ofxEasing :: easeOut;
        } else if( easeFunc == OF_EASE_CIRC_INOUT ) { 
            easing = &easingCirc;
            ptFunc = &ofxEasing :: easeInOut;
        //------------------------------------------------- cubic.
        } else if( easeFunc == OF_EASE_CUBIC_IN ) { 
            easing = &easingCubic;
            ptFunc = &ofxEasing :: easeIn;
        } else if( easeFunc == OF_EASE_CUBIC_OUT ) { 
            easing = &easingCubic;
            ptFunc = &ofxEasing :: easeOut;
        } else if( easeFunc == OF_EASE_CUBIC_INOUT ) { 
            easing = &easingCubic;
            ptFunc = &ofxEasing :: easeInOut;
        //------------------------------------------------- elastic.
        } else if( easeFunc == OF_EASE_ELASTIC_IN ) { 
            easing = &easingElastic;
            ptFunc = &ofxEasing :: easeIn;
        } else if( easeFunc == OF_EASE_ELASTIC_OUT ) { 
            easing = &easingElastic;
            ptFunc = &ofxEasing :: easeOut;
        } else if( easeFunc == OF_EASE_ELASTIC_INOUT ) { 
            easing = &easingElastic;
            ptFunc = &ofxEasing :: easeInOut;
        //------------------------------------------------- expo.
        } else if( easeFunc == OF_EASE_EXPO_IN ) { 
            easing = &easingExpo;
            ptFunc = &ofxEasing :: easeIn;
        } else if( easeFunc == OF_EASE_EXPO_OUT ) { 
            easing = &easingExpo;
            ptFunc = &ofxEasing :: easeOut;
        } else if( easeFunc == OF_EASE_EXPO_INOUT ) { 
            easing = &easingExpo;
            ptFunc = &ofxEasing :: easeInOut;
        //------------------------------------------------- linear.
        } else if( easeFunc == OF_EASE_LINEAR_IN ) { 
            easing = &easingLinear;
            ptFunc = &ofxEasing :: easeIn;
        } else if( easeFunc == OF_EASE_LINEAR_OUT ) { 
            easing = &easingLinear;
            ptFunc = &ofxEasing :: easeOut;
        } else if( easeFunc == OF_EASE_LINEAR_INOUT ) { 
            easing = &easingLinear;
            ptFunc = &ofxEasing :: easeInOut;
        //------------------------------------------------- quad.
        } else if( easeFunc == OF_EASE_QUAD_IN ) { 
            easing = &easingQuad;
            ptFunc = &ofxEasing :: easeIn;
        } else if( easeFunc == OF_EASE_QUAD_OUT ) { 
            easing = &easingQuad;
            ptFunc = &ofxEasing :: easeOut;
        } else if( easeFunc == OF_EASE_QUAD_INOUT ) { 
            easing = &easingQuad;
            ptFunc = &ofxEasing :: easeInOut;
        //------------------------------------------------- quart.
        } else if( easeFunc == OF_EASE_QUART_IN ) { 
            easing = &easingQuart;
            ptFunc = &ofxEasing :: easeIn;
        } else if( easeFunc == OF_EASE_QUART_OUT ) { 
            easing = &easingQuart;
            ptFunc = &ofxEasing :: easeOut;
        } else if( easeFunc == OF_EASE_QUART_INOUT ) { 
            easing = &easingQuart;
            ptFunc = &ofxEasing :: easeInOut;
        //------------------------------------------------- quint.
        } else if( easeFunc == OF_EASE_QUINT_IN ) { 
            easing = &easingQuint;
            ptFunc = &ofxEasing :: easeIn;
        } else if( easeFunc == OF_EASE_QUINT_OUT ) { 
            easing = &easingQuint;
            ptFunc = &ofxEasing :: easeOut;
        } else if( easeFunc == OF_EASE_QUINT_INOUT ) { 
            easing = &easingQuint;
            ptFunc = &ofxEasing :: easeInOut;
        //------------------------------------------------- sine.
        } else if( easeFunc == OF_EASE_SINE_IN ) { 
            easing = &easingSine;
            ptFunc = &ofxEasing :: easeIn;
        } else if( easeFunc == OF_EASE_SINE_OUT ) { 
            easing = &easingSine;
            ptFunc = &ofxEasing :: easeOut;
        } else if( easeFunc == OF_EASE_SINE_INOUT ) { 
            easing = &easingSine;
            ptFunc = &ofxEasing :: easeInOut;
        }
    }
    
    ofEaseFunction      easingFunc;
    ofxEasing*          easing;
    ofxEasingMemFn      ptFunc;

    ofxEasingBack       easingBack;
    ofxEasingBounce     easingBounce;
    ofxEasingCirc       easingCirc;
    ofxEasingCubic      easingCubic;
    ofxEasingElastic    easingElastic;
    ofxEasingExpo       easingExpo;
    ofxEasingLinear     easingLinear;
    ofxEasingQuad       easingQuad;
    ofxEasingQuart      easingQuart;
    ofxEasingQuint      easingQuint;
    ofxEasingSine       easingSine;
};

