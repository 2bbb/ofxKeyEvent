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
        struct Listener {
            Listener(std::function<void()> callback)
            : callback(callback)
            , bMute(false) {}
            
            void mute() { bMute = true; }
            void unmute() { bMute = false; }
            bool isMuted() const { return bMute; }
            void operator()() { if(!isMuted()) callback(); }
        private:
            std::function<void()> callback;
            bool bMute;
        };
        
        namespace detail {
            struct Listeners {
                void addListener(int key, std::function<void()> listener) {
                    maps.insert(std::make_pair(key, listener));
                }
                bool removeListener(int key) {
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
                
                void operator()(int key) {
                    auto it = maps.find(key);
                    if(it != maps.end()) it->second();
                }
            private:
                std::map<int, Listener> maps;
            };
        };
        
        class Manager {
            detail::Listeners press;
            detail::Listeners release;
            
            void keyPressed(ofKeyEventArgs &event) {
                press(event.key);
            }
            void keyReleased(ofKeyEventArgs &event) {
                release(event.key);
            }
        public:
            Manager() { resume(); }
            ~Manager() { pause(); }
            
            void resume() {
                ofAddListener(ofEvents().keyPressed, this, &Manager::keyPressed);
                ofAddListener(ofEvents().keyReleased, this, &Manager::keyReleased);
            }
            
            void pause() {
                ofRemoveListener(ofEvents().keyPressed, this, &Manager::keyPressed);
                ofRemoveListener(ofEvents().keyReleased, this, &Manager::keyReleased);
            }
            
            Manager &addPressListener(int key, std::function<void()> listener) {
                press.addListener(key, listener);
                return *this;
            }
            
            bool removePressListener(int key) {
                return press.removeListener(key);
            }
            
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
            
            Manager &addReleaseListener(int key, std::function<void()> listener) {
                release.addListener(key, listener);
                return *this;
            }
            
            bool removeReleaseListener(int key) {
                return release.removeListener(key);
            }
            
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
