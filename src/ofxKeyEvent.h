//
//  ofxKeyEvent.h
//
//  Created by ISHII 2bit on 2016/05/01.
//
//

#pragma once

#include <map>
#include <functional>

#include "ofEvent.h"
#include "ofEvents.h"
#include "ofEventUtils.h"

namespace ofx {
    namespace KeyEvent {
        
#define key_case(key) case(key): return #key;
        static std::string getOfKeyNameFromInt(int key) {
            switch(key) {
                    key_case(OF_KEY_RETURN);
                    key_case(OF_KEY_ESC);
                    key_case(OF_KEY_TAB);
                    key_case(OF_KEY_COMMAND);
                    key_case(OF_KEY_BACKSPACE)
                    key_case(OF_KEY_F1);
                    key_case(OF_KEY_F2);
                    key_case(OF_KEY_F3);
                    key_case(OF_KEY_F4);
                    key_case(OF_KEY_F5);
                    key_case(OF_KEY_F6);
                    key_case(OF_KEY_F7);
                    key_case(OF_KEY_F8);
                    key_case(OF_KEY_F9);
                    key_case(OF_KEY_F10);
                    key_case(OF_KEY_F11);
                    key_case(OF_KEY_F12);
                    key_case(OF_KEY_LEFT);
                    key_case(OF_KEY_UP);
                    key_case(OF_KEY_RIGHT);
                    key_case(OF_KEY_DOWN);
                    key_case(OF_KEY_PAGE_UP);
                    key_case(OF_KEY_PAGE_DOWN);
                    key_case(OF_KEY_HOME);
                    key_case(OF_KEY_END);
                    key_case(OF_KEY_INSERT);
                    key_case(OF_KEY_CONTROL);
                    key_case(OF_KEY_ALT);
                    key_case(OF_KEY_SHIFT);
                default: break;
            }
            return std::string("unknown key: ") + ofToString(key);
        };
#undef key_case
        
        struct Listener {
            Listener(std::function<void()> callback, const std::string &description)
            : callback(callback)
            , description(description)
            , bMute(false) {}
            
            void mute() { bMute = true; }
            void unmute() { bMute = false; }
            bool isMuted() const { return bMute; }
            void operator()() const { if(!isMuted()) callback(); }
            const std::string &getDescription() const { return description; }
        private:
            std::function<void()> callback;
            std::string description;
            bool bMute;
        };
        
        namespace detail {
            struct Listeners {
                void addListener(int key, std::function<void()> listener, const std::string &description) {
                    maps.insert(std::make_pair(key, Listener(listener, description)));
                    orderedKeys.push_back(key);
                }
                bool removeListener(int key) {
                    orderedKeys.erase(std::remove(orderedKeys.begin(), orderedKeys.end(), key),
                                      orderedKeys.end());
                    return maps.erase(key) != 0L;
                }
                bool isMuted(int key) const {
                    auto it = maps.find(key);
                    return it != maps.end() && it->second.isMuted();
                }
                
                inline bool setMute(int key, bool bMute) {
                    auto it = maps.find(key);
                    if(it == maps.end()) return false;
                    bMute ? it->second.mute() : it->second.unmute();
                    return true;
                }
                
                inline bool mute(int key) {
                    return setMute(key, true);
                }
                
                inline bool unmute(int key) {
                    return setMute(key, false);
                }
                
                void operator()(int key) const {
                    auto it = maps.find(key);
                    if(it != maps.end()) it->second();
                }
                const std::map<int, Listener> &getListenerMap() const {
                    return maps;
                }
                const std::vector<int> &getOrderedKeys() const {
                    return orderedKeys;
                }
            private:
                std::map<int, Listener> maps;
                std::vector<int> orderedKeys;
            };
        };
        
        class Manager {
            struct ListenerDescription {
                std::string listenerName;
                std::string functionDescription;
            };
            using ListenerDescriptions = std::multimap<int, ListenerDescription>;
            
            static ListenerDescriptions &press_descriptions() {
                static ListenerDescriptions descriptions;
                return descriptions;
            }
            static ListenerDescriptions &release_descriptions() {
                static ListenerDescriptions descriptions;
                return descriptions;
            }
            
            std::string name;
            
            detail::Listeners press;
            detail::Listeners release;
            
            void keyPressed(ofKeyEventArgs &event) { keyPressed(event.key); }
            void keyReleased(ofKeyEventArgs &event) { keyReleased(event.key); }
            std::string print(int key) const {
                if(std::isprint(static_cast<char>(key))) {
                    std::string str = "' '";
                    str[1] = static_cast<char>(key);
                    return str;
                } else {
                    return getOfKeyNameFromInt(key);
                }
            }
            
            bool bDraw;
            bool bEnabled;
            
        public:
            Manager()
            : name("no name listener")
            , bDraw(false) { enable(); }
            ~Manager() { disable(); }
            
            void setup(const std::string &name) {
                this->name = name;
            }
            
            int draw(float x, float y) const {
                if(!bDraw) return 0;
                
                int offset = 0;
                {
                    auto &maps = press.getListenerMap();
                    std::string text;
                    ofDrawBitmapString(name + " [key press listeners]", x, y + offset);
                    offset += 18;
                    for(auto it = maps.cbegin(); it != maps.cend(); ++it) {
                        text = print(it->first) + ":  " + it->second.getDescription();
                        ofDrawBitmapString(text, x, y + offset);
                        offset += 18;
                    }
                }
                offset += 4;
                {
                    auto &maps = release.getListenerMap();
                    std::string text;
                    ofDrawBitmapString(name + " [key release listeners]", x, y + offset);
                    offset += 18;
                    for(auto it = maps.cbegin(); it != maps.cend(); ++it) {
                        text = print(it->first) + ":  " + it->second.getDescription();
                        ofDrawBitmapString(text, x, y + offset);
                        offset += 18;
                    }
                }
                return offset;
            }
            
            int draw(const ofVec2f &vec = ofVec2f()) const {
                return draw(vec.x, vec.y);
            }
            
            int drawWithRegisteredOrder(float x, float y) const {
                if(!bDraw) return 0;
                
                int offset = 0;
                {
                    auto &keys = press.getOrderedKeys();
                    std::string text;
                    ofDrawBitmapString(name + " [key press listeners]", x, y + offset);
                    offset += 18;
                    for(const auto &key : keys) {
                        drawPressKeyDescription(key, x, y + offset);
                        offset += 18;
                    }
                }
                offset += 4;
                {
                    auto &keys = release.getOrderedKeys();
                    std::string text;
                    ofDrawBitmapString(name + " [key release listeners]", x, y + offset);
                    offset += 18;
                    for(const auto &key : keys) {
                        drawReleaseKeyDescription(key, x, y + offset);
                        offset += 18;
                    }
                }
                return offset;
            }
            
            int drawWithRegisteredOrder(const ofVec2f &vec = ofVec2f()) const {
                return drawWithRegisteredOrder(vec.x, vec.y);
            }
            
            void drawPressKeyDescription(int key, float x, float y) const {
                if(!bDraw) return;
                auto it = press.getListenerMap().find(key);
                if(it == press.getListenerMap().end()) return;
                ofDrawBitmapString(print(it->first) + ":  " + it->second.getDescription(), x, y);
            }
            
            void drawPressKeyDescription(int key, const ofVec2f &vec) const {
                if(!bDraw) return;
                drawPressKeyDescription(key, vec.x, vec.y);
            }
            
            void drawReleaseKeyDescription(int key, float x, float y) const {
                if(!bDraw) return;
                auto it = press.getListenerMap().find(key);
                if(it == press.getListenerMap().end()) return;
                ofDrawBitmapString(print(it->first) + ":  " + it->second.getDescription(), x, y);
            }
            
            void drawReleaseKeyDescription(int key, const ofVec2f &vec) const {
                if(!bDraw) return;
                drawReleaseKeyDescription(key, vec.x, vec.y);
            }

            void enable() {
                ofAddListener(ofEvents().keyPressed, this, &Manager::keyPressed);
                ofAddListener(ofEvents().keyReleased, this, &Manager::keyReleased);
                bEnabled = true;
            }
            OF_DEPRECATED_MSG("resume() is deprecated: please use enable().", inline void resume()) { enable(); } // DEPRECATED
            
            void disable() {
                ofRemoveListener(ofEvents().keyPressed, this, &Manager::keyPressed);
                ofRemoveListener(ofEvents().keyReleased, this, &Manager::keyReleased);
                bEnabled = false;
            }
            OF_DEPRECATED_MSG("pause() is deprecated: please use disable().", inline void pause()) { disable(); } // DEPRECATED
            
            bool isEnabled() const { return bEnabled; }
            
            void enableDraw() { bDraw = true; }
            
            void disableDraw() {
                bDraw = false;
            }
            
            bool isDrawingNow() const { return bDraw; }
            OF_DEPRECATED_MSG("isDrawNow() is deprecated: please use isDrawingNow().", inline bool isDrawNow() const) { return isDrawingNow(); }
            
            void dump() const {
                ofLogNotice("ofxKeyEvent") << "==== press listners ====";
                if(press_descriptions().size() == 0) {
                    ofLogNotice("ofxKeyEvent") << "no event registered";
                } else {
                    auto &descriptions = press_descriptions();
                    for(auto it = descriptions.begin(), end = descriptions.end(); it != end; ++it) {
                        ofLogNotice("ofxKeyEvent") << print(it->first) << "  " << it->second.functionDescription << "[at " << it->second.listenerName << "]";
                    }
                }
                ofLogNotice("ofxKeyEvent") << "==== release listners ====";
                if(release_descriptions().size() == 0) {
                    ofLogNotice("ofxKeyEvent") << "  no event registered";
                } else {
                    auto &descriptions = release_descriptions();
                    for(auto it = descriptions.begin(), end = descriptions.end(); it != end; ++it) {
                        ofLogNotice("ofxKeyEvent") << print(it->first) << "  " << it->second.functionDescription << " [at " << it->second.listenerName << "]";
                    }
                }
            }
            
            Manager &addListener(int key, std::function<void()> listener, const std::string &description = "no description"){
                return addPressListener(key, listener, description);
            }
            
            Manager &addPressListener(int key, std::function<void()> listener, const std::string &description = "no description") {
                
                auto &descriptions = press_descriptions();
                if(0 < descriptions.count(key)) {
                    auto it = descriptions.find(key);
                    ofLogWarning("ofxKeyEvent") << print(key) << " is already registered.";
                    for(std::size_t i = 0; i < descriptions.count(key); ++i, ++it) {
                        ofLogWarning("ofxKeyEvent") << "  " << it->second.functionDescription << "[at " << it->second.listenerName << "]";
                    }
                }
                descriptions.insert(std::make_pair(key, ListenerDescription{name, description} ));
                
                press.addListener(key, listener, description);
                return *this;
            }
            
            bool removeListener(int key){
                return removePressListener(key);
            }

            bool removePressListener(int key) {
                auto &descriptions = press_descriptions();
                std::size_t count = descriptions.count(key);
                auto it = descriptions.find(key);
                if(count) for(std::size_t i = 0; i < count; ++i, ++it) {
                    if(it->second.listenerName == name) {
                        it = descriptions.erase(it);
                    }
                }
                return press.removeListener(key);
            }
            
            void keyPressed(int key) const { press(key); }
            
            bool isPressListenerMuted(int key) const {
                return press.isMuted(key);
            }
            
            inline bool setMutePressing(int key, bool bMute) {
                return press.setMute(key, bMute);
            }
            
            inline bool mutePressListener(int key) {
                return press.mute(key);
            }
            
            inline bool unmutePressListener(int key) {
                return press.unmute(key);
            }
            
            Manager &addReleaseListener(int key, std::function<void()> listener, const std::string &description = "no description") {
                
                auto &descriptions = release_descriptions();
                if(0 < descriptions.count(key)) {
                    auto it = descriptions.find(key);
                    ofLogWarning("ofxKeyEvent") << print(key) << " is already registered.";
                    for(std::size_t i = 0; i < descriptions.count(key); ++i, ++it) {
                        ofLogWarning("ofxKeyEvent") << "  [" << it->second.listenerName << "] " << it->second.functionDescription;
                    }
                }
                descriptions.insert(std::make_pair(key, ListenerDescription{name, description} ));
                
                release.addListener(key, listener, description);
                return *this;
            }
            
            bool removeReleaseListener(int key) {
                auto &descriptions = release_descriptions();
                std::size_t count = descriptions.count(key);
                auto it = descriptions.find(key);
                if(count) for(std::size_t i = 0; i < count; ++i, ++it) {
                    if(it->second.listenerName == name) {
                        it = descriptions.erase(it);
                    }
                }
                return release.removeListener(key);
            }
            
            void keyReleased(int key) const { release(key); }
            
            bool isReleaseListenerMuted(int key) const {
                return release.isMuted(key);
            }
            
            inline bool setMuteReleaseListener(int key, bool bMute) {
                return release.setMute(key, bMute);
            }
            
            inline bool muteReleaseListener(int key) {
                return release.mute(key);
            }
            
            inline bool unmuteReleaseListener(int key) {
                return release.unmute(key);
            }
        };
    };
};

using ofxKeyEventManager = ofx::KeyEvent::Manager;
