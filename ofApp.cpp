#include "ofApp.h"
#include <time.h>

//--------------------------------------------------------------
void ofApp::setup(){

	menuactive			= true;
	collision			= false;
	gameover			= false;
	scoreCounter		= true;
	slowDown		    = false;
	restart			    = false;
	reset				= false;

	m_sound.load("song.mp3"); // Loads Music
	m_sound.play();			  // Music Plays when the game is launched

	m_palmPos = ofVec3f(0, 0, 0);	
	m_palmRotX = 200.0f;
	CurFrameNum = 0;
	frameNum = 0.0f;
	distance = 0.0f;
	speed = 1.2f;
	rotation = 0.0f;
	cameraPosition = ofVec3f(0.0f, 0.0f, 0.0f);

	m_device.connectEventHandler( &ofApp::onLeapFrame, this ); 

	//IMAGES///////////////////////////////////////////
	m_playerImg.load(ProjectConstants::IMG_PATH_PLAYER);
	m_MenuImg.load(ProjectConstants::IMG_PATH_MENU);
	m_BgImg.load(ProjectConstants::IMG_PATH_BG);
	m_BgImg2.load(ProjectConstants::IMG_PATH_BG2);
	m_BgImg3.load(ProjectConstants::IMG_PATH_BG3);
	m_gameOver.load(ProjectConstants::IMG_PATH_GO);

	ofSetFrameRate(ProjectConstants::PROJ_DESIRED_FRAMERATE);

	Leap::Controller * controller = m_device.getController();
	controller->enableGesture(Leap::Gesture::TYPE_SWIPE, true);
	controller->enableGesture(Leap::Gesture::TYPE_KEY_TAP, true);

	m_shipCol = ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f); //white
	myFont.loadFont("Montserrat-Bold.otf", 30);
	myFont2.loadFont("Montserrat-Bold.otf", 10);
	myFont3.loadFont("Montserrat-Bold.otf", 50);

	weather = rand() % 50;

	m_pinchStrength = 0.0f;
	m_grabStrength = 0.0f;

	float enemyWidth = 3.0f;
	float enemyHeight = 3.0f;


	for (int i = 0; i < 70; i ++) 
	{
		numX = rand() % 80 + (-40);
		numZ = rand() % 500 + 300;
		numZ = numZ * -1;

			enemyPosX[i] = numX;
			enemyPosZ[i] = numZ;
	}

	enemyPosY = 400;


}

//--------------------------------------------------------------
void ofApp::update() {


	score = frameNum - CurFrameNum; // Get current frame of the game for score to start at 0


	m_device.update(); //update the device

	const Leap::HandList& hands = m_frame.hands();

	for (int i = 0; i < hands.count(); i++)
	{

		const Leap::Hand& hand = hands[i]; // Get the current player hand
		const Leap::Vector palmPos = hand.palmPosition(); //Get the position of the current players hand
		m_palmPos = ofxLeapMotion::toVec3(palmPos); //Convert the hand position to a vector
		m_palmRot = ofVec3f(ofRadToDeg(hand.direction().pitch()), ofRadToDeg(hand.direction().yaw()), ofRadToDeg(hand.direction().roll()));
		m_palmRotX = ofRadToDeg(hand.direction().roll());
		m_pinchStrength = hand.pinchStrength();
		m_grabStrength = hand.grabStrength();
	}

	//Determine whether any gestures have been made based on the game screens/score etc..
	const Leap::GestureList& gestures = m_frame.gestures();

	if (m_palmRotX < 0)
	{
		m_palmRotX *= -1;
	}


	if (m_palmRotX < 70)
	{
		menuactive = false;
	}


	for (int i = 0; i < gestures.count(); i++)
	{
		const Leap::Gesture& gesture = gestures[i];
		Leap::Gesture::Type type = gesture.type();
		if (score > 1000 && score < 2000)
		{
			if (type == Leap::Gesture::Type::TYPE_KEY_TAP)
			{
				//slowDown = true; //If the user taps down like a keyboard then set the slow down bool to true
				if (slowDown == false) {
					slowDown = true;
				}
				else
				{
					slowDown = false;
				}

			}
		}

		if (collision ==true && type == Leap::Gesture::Type::TYPE_SWIPE) //If user swipes at the end of the game when a collision has happened 
		{																	  //set the restart to true, collision to false, menuactive to true and game over to false
				collision = false;
				menuactive = true;
				gameover = false;
		}
	}

	if (menuactive == true) { //Allows for score to start and stop based on frame num bool
		CurFrameNum = ofGetFrameNum();

		cameraPosition = ofVec3f(0.0f, 0.0f, 0.0f);
		camera.setPosition(0.0f, 0.0f, 0.0f);
		speed = 1.2f;

	}

	if (menuactive == false)
	{
		ofVec3f cameraPosition = camera.getPosition(); //track the camera and palm positions to move the camera based on the players hand position
		ofVec3f moveVec = (cameraPosition - m_palmPos) * 0.5f; 

		camera.setPosition(-1 * (moveVec.x * 0.2f), 0.0f, cameraPosition.z - speed);
		m_cameraPositionZ = cameraPosition.z;
		m_cameraPositionX = cameraPosition.x;


		///////////////////// SPEEDS ////////////////////
		//Setting the if statement speeds so user can slow down speed if they make it to the required score

		if (score < 1000)
		{
			speed;
		}

		else if (score < 2000)
		{
			if (slowDown == true) 
			{
				speed = 0.75f;
			}
			else 
			{
				speed = 1.6f;
			}
		}
		else if(score > 2000)
		{
			slowDown = false;
			speed = 2.0f;
		}

		if (menuactive == false && gameover == false) 
		{
			frameNum = ofGetFrameNum();
		}

		for (int i = 0; i < 70; i++) 
		{
			distance = ofDist(-1 * (moveVec.x * 0.2f), cameraPosition.z - speed, enemyPosX[i], enemyPosZ[i]);

			if (distance <= 2)
			{
				collision = true;
				gameover = true;
				scoreCounter = false;
			}
		}
	}
}


//DRAW//
void ofApp::draw() {

	ofSetRectMode(OF_RECTMODE_CENTER);

	//////////////////////////////// End Score Screen //////////////////////////
	if (collision == true && gameover == true)
	{
		ofClear(ofFloatColor(m_pinchStrength, 0.0f, 0.0f, 1.0f));
			ofPushMatrix();
				m_gameOver.draw(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
				myFont3.drawString("SCORE:  " + ofToString(score), 430, 450);
			ofPopMatrix();
		return;
	}

	////////////////////////////// Beginning Menu Screen
	if (menuactive == true)
	{
		scoreCounter = false;

		for (int i = 0; i < 70; i++)
		{
			numX = rand() % 80 + (-40);
			numZ = rand() % 500 + 300;
			numZ = numZ * -1;

			enemyPosX[i] = numX;
			enemyPosZ[i] = numZ;
		}

		ofClear(ofFloatColor(m_pinchStrength, 0.0f, 0.0f, 1.0f));
			ofPushMatrix();
				m_MenuImg.draw(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
			ofPopMatrix();
		return;
	}

	else if (menuactive == false)
	{
		scoreCounter = true;
		ofClear(ofFloatColor(m_pinchStrength, 0.0f, 0.0f, 1.0f));
		if (score < 1000)
		{
			ofPushMatrix();
				m_BgImg.draw(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
				ofSetColor(255, 255, 255);
				m_playerImg.draw(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
			ofPopMatrix();
		}

		else if (score > 2000)
		{
			ofPushMatrix();
				m_BgImg3.draw(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
				ofSetColor(255, 255, 255);
				m_playerImg.draw(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
			ofPopMatrix();
		}
		else
		{
			ofPushMatrix();
				m_BgImg2.draw(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
				ofSetColor(0, 255, 0);
				m_playerImg.draw(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
			ofPopMatrix();
		}

		newScreenX = m_palmPos.x * 10.0f; 
		newScreenY = m_palmPos.z * 10.0f;
		newScreenX += ProjectConstants::PROJ_WINDOW_RES_X / 2.0f;
		newScreenY += ProjectConstants::PROJ_WINDOW_RES_Y / 2.0f;

		newScreenX = ofClamp(newScreenX, 0, ProjectConstants::PROJ_WINDOW_RES_X);
		newScreenY = ofClamp(newScreenY, 0, ProjectConstants::PROJ_WINDOW_RES_Y);

		//////////CAMERA START//////////////////////////////////////////////////////////////////////////

		camera.begin();

		if (score < 1000 || score > 2000)
		{
			ofNoFill();
			for (int i = 0; i < 70; i++)
			{

			//	ofVec2f vec = cameraPosition - enemyPosZ[i];

				ofPushMatrix();
					ofPushMatrix();
						ofSetColor(255, 255, 255);
						ofTranslate(enemyPosX[i], 0, enemyPosZ[i]);
						ofDrawBox(3.0f);
					ofPopMatrix();

				for (int i = 0; i < 70; i++);
				{
					if (m_cameraPositionZ < enemyPosZ[i]) {
						numZ = rand() % 600 + 300;
						enemyPosZ[i] = enemyPosZ[i] - 400;
						numX = rand() % 80 + (-40);
						enemyPosX[i] = numX;
						ofTranslate(enemyPosX[i], 0, enemyPosZ[i]);
						ofDrawBox(3.0f);
					}
				}
				ofPopMatrix();
			}
		}

		/////////////////////ENEMIES SCORE MORE THAN 1000 AND LESS THAN 2000//////////////////////////////////////////////////////////////////
		else
		{

			ofNoFill();
			for (int i = 0; i < 70; i++) 
			{
				ofPushMatrix();
					ofPushMatrix();
						ofSetColor(0, 255, 0);
						ofTranslate(enemyPosX[i], 0, enemyPosZ[i]);
						ofDrawBox(3.0f);
					ofPopMatrix();

				for (int i = 0; i < 70; i++);
				{
					if (m_cameraPositionZ < enemyPosZ[i]) {
						numZ = rand() % 600 + 300;
						enemyPosZ[i] = enemyPosZ[i] - 400;
						numX = rand() % 80 + (-40);
						enemyPosX[i] = numX;
						ofTranslate(enemyPosX[i], 0, enemyPosZ[i]);
						ofDrawBox(3.0f);
					}
				}
				ofPopMatrix();
			}
		}

		camera.end();

		//////////CAMERA END//////////////////////////////////////////////////////////////////////////

		ofFill();

		if (m_shipCol == ofFloatColor(0.0f, 0.0f, 1.0f, 1.0f))
		{
			ofSetColor(255, 255, 255);
			
		}

		if (score <= 1000 || score >= 2000)
		{
			ofPushMatrix();
				ofSetColor(255, 255, 255);		
				myFont2.drawString("FIELD SLAYER", 20, 40);
				myFont2.drawString("Local Weather in degrees: " + ofToString(weather), 20, 60);
				myFont2.drawString("Move hand LEFT and RIGHT to dodge boxes", 480, 80);
				myFont.drawString(" " + ofToString(score), 590, 150);
				ofSetColor(m_shipCol);
			ofPopMatrix();
		}


		else
		{
			ofPushMatrix();
				ofSetColor(0, 255, 0);
				myFont2.drawString("FIELD SLAYER", 20, 40);
				ofSetColor(0, 255, 0);
				myFont2.drawString("Local Weather in degrees: " + ofToString(weather), 20, 60);

				myFont2.drawString("Tap Fingers Down for SLOW MOTION", 480, 80);
				myFont.drawString(" " + ofToString(score), 590, 150);
				ofSetColor(m_shipCol);
			ofPopMatrix();
		}

	}
}



void ofApp::onLeapFrame(Leap::Frame _frame)
{

	m_frame = _frame; 

}
