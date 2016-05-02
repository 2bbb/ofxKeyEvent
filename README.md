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
