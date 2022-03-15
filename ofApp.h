#pragma once

#include "ofMain.h"
#include "ofxLeap.h"
#include "ProjectConstants.h"

class ofApp : public ofBaseApp{

	public:

		ofxLeapMotion::Device m_device; //leap controller instance track
		Leap::Frame			  m_frame; //data relating to hand tracking

		void onLeapFrame(Leap::Frame _frame);
		void				setup();
		void				update();
		void				draw();

		ofImage				m_playerImg;
		ofImage				m_enemyImg;
		ofImage				m_MenuImg;
		ofImage				m_BgImg;
		ofImage				m_BgImg2;
		ofImage				m_BgImg3;
		ofImage				m_gameOver;

		ofSoundPlayer		m_sound; 

		ofVec3f				m_palmPos;
		ofVec3f				m_palmRot; 
		ofVec3f				cameraPosition;

		ofFloatColor		m_shipCol;	

		time_t				timev;

		ofTrueTypeFont		myFont;
		ofTrueTypeFont		myFont2;
		ofTrueTypeFont		myFont3;

		ofCamera			 camera;

		int					CurFrameNum;

		float				m_palmRotX;
		float				m_pinchStrength;
		float				m_grabStrength;
		float				enemyPosX[70];
		float				enemyPosY;
		float				enemyPosZ[70];
		float				m_enemyRadius;
		float				enemyPosRand;
		float				m_player;
		float				m_cameraPositionZ;
		float				m_cameraPositionX;
		float				m_cameraRadius;
		float				frameNum;
		float				score;
		float				 numX;
		float				 numZ;
		float				newScreenX;
		float				newScreenY;
		float				distance;
		float				opacity;
		float				playerPositionX;
		float				playerPositionY;
		float				speed;
		float				weather;
		float				rotation;
		float				angle;

		bool				collision;
		bool				slowDown;
		bool				gameover;
		bool				menuactive;
		bool				restart;
		bool				reset;
		bool				scoreCounter;
};
