# ofxKeyEvent

```
class ofApp : public ofBaseApp {
	ofxKeyEventManager manager;
	int value{0};
	void minus() {};
public:
	void setup() {
		manager.addListener('+', [this] { value++; });
		manager.addListener('-', std::bind(&ofApp::minus, this));
	}
};
```

## API

### ofxKeyEventManager

----

#### void setup(const std::string &name = "no name")

setup with name

* name: manager's name

----

#### ofxKeyEventManager &addPressListener(int key, std::function<void()> callback, const std::string &description)
#### ofxKeyEventManager &addReleaseListener(int key, std::function<void()> callback, const std::string &description = "no description")

add listener for `key` with `callback`

* key: char code
* callback: is called when press/release key
* description: event description

* returns itself. can method chain.

#### ofxKeyEventManager &addListener(int key, std::function<void()> callback, const std::string &description)

same as addPressListener

----

#### bool removePressListener(int key)
#### bool removeReleaseListener(int key)

remove listener for `key`

* key: char code

#### bool removeListener(int key)

same as removePressListener

----

#### void keyPressed(int key)
#### void keyReleased(int key)

call listener manually

----

#### void enable()
#### void disable()

##### void resume() // deprecated 
##### void pause() // deprecated

* enable/disable to listen event

#### isEnabled() const

* return `true` if listen now else `false`

----

#### void enableDraw()
#### void disableDraw()

* enable/disable draw()

#### bool isDrawNow() const
##### bool isDrawingNow() const // deprecated

* get drawing status

----

#### mutePressListener(int key)
#### muteReleaseListener(int key)
#### unmutePressListener(int key)
#### unmuteReleaseListener(int key)
#### setMutePressing(int key, bool bMute)
#### setMuteReleasing(int key, bool bMute)

* mute/unmute a specified press/release listener

#### isPressListenerMuted(int key) const
#### isReleaseListenerMuted(int key) const

* get mute status for key given.

----

#### void draw(int x, int y) const
#### void draw(const ofVec2f &pos) const

draw descriptions

----

#### void drawPressKeyDescription(int key, float x, float y) const
#### void drawPressKeyDescription(int key, const ofVec2f &pos) const
#### void drawReleaseKeyDescription(int key, float x, float y) const
#### void drawReleaseKeyDescription(int key, const ofVec2f &pos) const

draw specified description for `key` given

----

#### void dump() const

dump out descriptions of listener that registered to all of ofxKeyEvent object to console.

## Update history

### 2016/05/03 ver 0.0.1 release

* initial

## License

MIT License.

## Author

* ISHII 2bit [bufferRenaiss co., ltd.]
* ishii[at]buffer-renaiss.com

## At the last

Please create a new issue if there is a problem.
And please throw a pull request if you have a cool idea!!
