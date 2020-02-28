#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	manager.addListener('+', [this] { value++; }, "increment value");
	manager.addListener('-', std::bind(&ofApp::minus, this), "decrement value");

	manager.addListener('c', [this] { bColor = !bColor; }, "flip color");

	manager.enableDraw();
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofPushStyle();

	//key manager info
	ofSetColor(128);
	manager.draw(10, 10);

	//scene
	if (bColor)
		ofSetColor(0);
	else
		ofSetColor(255);
	ofDrawCircle(ofGetWidth()*0.5, ofGetHeight()*0.5, value * 10);

	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	//toggle key manager draw
	if (key == 'd')
	{
		manager.isDrawingNow() ? manager.disableDraw() : manager.enableDraw();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
