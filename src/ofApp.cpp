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

bool		confirmPage;
int		currentSel;

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
    img.load("images/eyesy_engine_03.png");

    // clear main screen
    ofClear(0,0,0);

    // init selection
    selection = 0;
    finalSel = 0;
    
    currentSel = getCurrentSel();
}

//--------------------------------------------------------------
void ofApp::update() {
    
    saved = false;
    up = false;
    down = false;
    osdTrig = false;

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
	    	
		// scene up
	   	if (m.getArgAsInt32(0) == 50 && m.getArgAsInt32(1) > 0) {
			up = true;
		}
	    	
		// scene down
       	    	if (m.getArgAsInt32(0) == 49 && m.getArgAsInt32(1) > 0) {
			down = true;
		}

		// osd toggle
       	    	if (m.getArgAsInt32(0) == 1 && m.getArgAsInt32(1) > 0) {
			osdTrig = true;
		}

	    	
	}	

    } // end of receiving messages from pure data
   
} // end of update function

//--------------------------------------------------------------
void ofApp::draw() {
	// selection buttons
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
	
	// draw the graphics
	//
	string currentRez = nameSel( currentSel );
	int space = font.stringHeight( currentRez );
	
	// draw the png
	ofSetBackgroundColor(0);
	ofSetColor(255);
	img.draw(0,0);
	
	// draw the text
	ofPushMatrix();	
		ofSetColor(255);
		ofTranslate( (w/3)-(w/16), (h/3)-(h/11) );
		ofTranslate( space, space);
		font.drawString( currentRez, 0, 0);
        ofPopMatrix();
	
	// display selection box
	ofPushMatrix();
		ofTranslate( (w/2)+(w/24.59), (h/3)+(h/5.8) );
		ofNoFill();
		int boxHeight = floor(h/17.35);
		int boxWidth = floor(w/7.45);
		int highlite = selection * boxHeight;
        	ofDrawRectangle(0, highlite, boxWidth, boxHeight);
		ofDrawRectangle(1, highlite+1, boxWidth-2, boxHeight );
	ofPopMatrix();
	
	// display window for confirmation
	if( saved == true && confirmPage == false ) {
		confirmPage = true;
	}
	
	// turn off confirmation page if any of the scene buttons are pressed
	if( confirmPage == true && (up == true || down == true ) ) {
		confirmPage = false;
	}
	
	// draw confirmation page
	if( confirmPage == true ) {
		string		selString;
		selString = nameSel( selection );
		
		ofPushMatrix();
			ofTranslate(w/4, h/4);
			ofFill();
			ofSetColor(50,50,50);  // draw a gray background
			ofDrawRectangle( h/128, h/128, w/2, h/2);
			ofSetColor(255);
			ofDrawRectangle( 0, 0, w/2, h/2);
			ofSetColor(0);
			//ofScale(0.5,0.5);
			font.drawString("Press The OSD Button to Reboot your EYESY", space/2, space*2);
			font.drawString("with your selection:", space/2, space*4);
		ofPopMatrix();
		ofPushMatrix();
			ofTranslate( w/2, h/2 );
			font.drawString( selString, -(font.stringWidth(selString)/2), space*2);
		ofPopMatrix();	
	}

	
	
	// rewrite config.txt and reboot
	if( confirmPage == true && osdTrig == true ) {
		rezSelected( selection );
		cout << "it is rebooting and stuff" << "\n";
	}
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
int ofApp::getCurrentSel() {
    return 0;
}

//--------------------------------------------------------------
string ofApp::nameSel( int select ) {
	string	sel;
	if ( select == 0 ) {
		sel = "openFrameworks: 1080p";
	} else if (select == 1) {
		sel = "openFrameworks: 720p";
	} else if (select == 2) {
		sel = "openFrameworks: 480p";
	} else if (select == 3) {
		sel = "Pygame: 1080p (Slow)";
	} else if (select == 4) {
		sel = "Pygame: 720p";
	} else if( select == 5) {
		sel = "Pygame: 480p";
	}
	return sel;
}

//--------------------------------------------------------------
void ofApp::rezSelected(int sel) {
    cout << "reboot! with this rez computer please!: " << nameSel( sel ) << "\n";
}

//--------------------------------------------------------------
