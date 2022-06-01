/*
 * Copyright (c) 2020 Owen Osborn, Critter & Gutiari, Inc.
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
*/

#include "ofApp.h"
#include <stddef.h>
#include <stdint.h>
#include <string>

float 		h; 
float 		w; 


ofImage		img;

//--------------------------------------------------------------
void ofApp::setup() {
    
    // declare the OS version
    string 	Version = "oFLua 1.0";
    
    // declare screen size
    w = ofGetScreenWidth();
    h = ofGetScreenHeight(); 
   
    // listen on the given port
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup(PORT);  
    sender.setup("localhost", PORT+1);  
    
    ofSetVerticalSync(true);
    
    ofSetLogLevel("ofxLua", OF_LOG_VERBOSE);
    
    // hide the cursor
    ofHideCursor();

        
    // init the lua state
    lua.init( true ); // true because we want to stop on an error
    
    // listen to error events
    lua.addListener(this);
    
        
    // call the script's setup() function
    lua.scriptSetup();

    // load font
    font.load("CGFont_0.18.otf", 124, true, false, false, 1, 24);

    // load image
    img.load("images/eyesy-engine.png");

    // clear main screen
    ofClear(0,0,0);

    // init selection
    selection = 0;
    finalSel = 0;
}

//--------------------------------------------------------------
void ofApp::update() {
    
    saved = false;
    up = false;
    down = false;
    

    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
	
	// get various key messages
        if(m.getAddress() == "/key") {   
            	//cout << m.getArgAsInt32(0) << "\n";
		// grab screen
            	if (m.getArgAsInt32(0) == 8 && m.getArgAsInt32(1) > 0) {
            		saved = true; 	
		}
	    	
		// trigger button
	   	if (m.getArgAsInt32(0) == 50 && m.getArgAsInt32(1) > 0) {
			up = true;
		}
	    	
		// osd toggle button
       	    	if (m.getArgAsInt32(0) == 49 && m.getArgAsInt32(1) > 0) {
			down = true;
		}

	    	
	}	

	////// the knobs
        if(m.getAddress() == "/knob1") {
		//knob1Local = m.getArgAsInt32(0);
		
	}
	if(m.getAddress() == "/knob2") {
		
	}
	if(m.getAddress() == "/knob3") {
		
	}
	if(m.getAddress() == "/knob4") {
		
	}	
	if(m.getAddress() == "/knob5") {
		
	}
    } // end of receiving messages from pure data
   
} // end of update function

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetBackgroundColor(255);
	
	string 	intro1 = "Hi - please select the Video Engine and Resolution.";
	string 	intro2 = "Use 'Scene Select' buttons to navigate to your choice.";
	string	which;
	string	intro = "When finished, press 'Scene Save' button, the EYESY will restart with: ";
	
	if( finalSel == 0 ) {which = "oF 1080p";}
	if( finalSel == 1 ) {which = "oF 720p";}
	if( finalSel == 2 ) {which = "oF 480p";}
	if( finalSel == 3 ) {which = "pY 1080p";}
	if( finalSel == 4 ) {which = "pY 720p";}
	if( finalSel == 5 ) {which = "pY 480p";}

	
	string	intro3 = intro  + which;	
	
	if( up ) { 
		if( selection >= 5 ) {
			selection = 0;
		} else {
			selection++;
		}	
	}
	if( down ) { 
		if( selection <= 0 ) {
			selection = 5;
		} else {
			selection--;
		}	
	}
	
	if( saved ) {
		finalSel = selection;
	}
	
	// draw top third instructions and hello
	int space = font.stringHeight( intro1 );
	ofPushMatrix();
		ofSetColor(0);
		ofFill();
		ofTranslate( space, space + (space/2) );
		font.drawString( intro1, 0, 0);
        	ofTranslate( 0, h/9 );
		font.drawString( intro2, 0, 0);
		ofTranslate( 0, h/9 );
		font.drawString( intro3, 0, 0);
	ofPopMatrix();
	
	// draw the image
	ofPushMatrix();
		ofSetColor(255);
		ofTranslate( (w/6)*4, h/100 );
		img.draw(0,0);
	ofPopMatrix();
	
	// draw bottom 2 backgrounds
	ofPushMatrix();
		// draw middle third bg
		ofTranslate( 0, h/3);
		ofSetColor(45, 150, 100);
		ofDrawRectangle( 0, 0, w, h/3);
		// draw bottom third bg
		ofTranslate( 0, h/3);
		ofSetColor(190, 112, 210);
		ofDrawRectangle( 0, 0, w, h/3);
		
	ofPopMatrix();
	
	// draw selection highlite
	ofPushMatrix();
		ofTranslate( w/2, h/3 );
		ofSetColor(200,200,0);
		ofFill();
		int highlite1 = finalSel * (h/9);
        	ofDrawRectangle(0, highlite1, w/4, h/9);
	ofPopMatrix();
	
	// draw texts
	ofPushMatrix();
		ofTranslate( 0, h/3);
		//// draw openframeworks title
		ofSetColor(0);
		font.drawString("OpenFrameWorks", space*2, (h/6) + (space/2) );
		
		//// draw the 3 rez
		ofTranslate( (w/8)*5, (h/9) - space );
		ofTranslate( -(font.stringWidth("1080p") / 2), 0 );
		font.drawString("1080p", 0, 0 );
		ofTranslate( 0, h/9);
		font.drawString( "720p", 0, 0 );
		ofTranslate( 0, h/9);
		font.drawString( "480p", 0, 0 );
	ofPopMatrix();
	
	ofPushMatrix();
		// draw bottom third rectangle
		ofTranslate( 0, (h/3)*2);
		// draw python title
		ofSetColor(0);
		
		font.drawString("Pygame", space*2, (h/6) + (space/2) );
		//// draw the 3 rez
		ofTranslate( (w/8)*5, (h/9) - space );
		ofTranslate( -(font.stringWidth("1080p") / 2), 0 );
		font.drawString("1080p(slow)", 0, 0 );
		ofTranslate( 0, h/9);
		font.drawString( "720p", 0, 0 );
		ofTranslate( 0, h/9);
		font.drawString( "480p", 0, 0 );
	ofPopMatrix();
	
	// display through a Highlight saved selection
	
	
	ofPushMatrix();
		ofTranslate( w/2, h/3 );
		ofNoFill();
		int highlite = selection * (h/9);
        	ofDrawRectangle(0, highlite, w/4, h/9);
	ofPopMatrix();

	
   
        
}

//--------------------------------------------------------------
void ofApp::exit() {
	lua.clear();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    /*
    switch(key) {
    
        case 'r':
            reloadScript();
            break;
    
        case OF_KEY_LEFT:
            prevScript();
            break;
            
        case OF_KEY_RIGHT:
            nextScript();
            break;
            
        case ' ':
            lua.doString("print(\"this is a lua string saying you hit the space bar!\")");
            cout << "fps: " << ofGetFrameRate() << "\n";    
            break;
    }
    
    lua.scriptKeyPressed(key);
    */
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    //lua.scriptMouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    //lua.scriptMouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
   // lua.scriptMousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
//    lua.scriptMouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::errorReceived(std::string& msg) {
    ofLogNotice() << "got a script error: " << msg;
}


//--------------------------------------------------------------
