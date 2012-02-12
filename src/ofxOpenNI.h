/*
 * ofxOpenNI.h
 *
 * Copyright 2011 (c) Matthew Gingold [gameover] http://gingold.com.au
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef	_H_OFXOPENNI
#define _H_OFXOPENNI

// comment this out to NOT use a hack that silences occasional
// crashes on exit for MAC OSX in a 'production/live' environment
// NOTE: it does not work if you're using GDB but will work if 
// you execute the application standalone.
// use setSafeThreading(true) for a much cleaner solution
// (but much lower FPS in main thread...;-() see line ~1712 of ofxOpenNI.cpp
#define USE_SIGNALS_HACK 1

#include <set>
#include <map>

#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
#include <XnLog.h>
#include <XnTypes.h>

#include "ofLog.h"
#include "ofConstants.h"
#include "ofPixels.h"
#include "ofTexture.h"
#include "ofThread.h"
#include "ofEvents.h"

#include "ofxOpenNITypes.h"
#include "ofxOpenNIUtils.h"

using namespace xn;

class ofxOpenNI : public ofThread {
	
public:
	
	ofxOpenNI();
	~ofxOpenNI();
	
	bool setup(bool threaded = true);
    bool setupFromONI(string oniFilePath, bool threaded = true);
    bool setupFromXML(string xmlFilePath, bool threaded = true);
	
    void stop();
    
	bool addDepthGenerator();
	bool addImageGenerator();
	bool addInfraGenerator();
	bool addUserGenerator();
    bool addGestureGenerator();
    bool addHandsGenerator();
    bool addAudioGenerator();

    bool removeDepthGenerator();
    bool removeImageGenerator();
    bool removeInfraGenerator();
    bool removeUserGenerator();
    bool removeGestureGenerator();
    bool removeHandsGenerator();
    bool removeAudioGenerator();
    
	void update();
	
    void drawDebug();
	void drawDebug(float x, float y);
	void drawDebug(float x, float y, float w, float h);
    
	void drawDepth();
	void drawDepth(float x, float y);
	void drawDepth(float x, float y, float w, float h);
	
	void drawImage();
	void drawImage(float x, float y);
	void drawImage(float x, float y, float w, float h);
	
    void drawSkeletons();
    void drawSkeletons(float x, float y);
    void drawSkeletons(float x, float y, float w, float h);
    
    void drawSkeleton(int nID);
    void drawSkeleton(float x, float y, int nID);
    void drawSkeleton(float x, float y, float w, float, int nID);
    
    void drawHands();
    void drawHands(float x, float y);
    void drawHands(float x, float y, float w, float h);
    
    void drawHand(int nID);
    void drawHand(float x, float y, int nID);
    void drawHand(float x, float y, float w, float, int nID);
    
    bool setResolution(int w, int h, int fps);

	float getWidth();
	float getHeight();
	
    void setLogLevel(XnLogSeverity logLevel);
    void setLogLevel(ofLogLevel logLevel);
    
    void setSafeThreading(bool b);
    bool getSafeThreading();
    
	int getNumDevices();
	
    bool isContextReady();
	bool isNewFrame();
    
    bool isDepthOn();
	bool isImageOn();
    bool isInfraOn();
    bool isUserOn();
    bool isGestureOn();
    bool isHandsOn();
    bool isAudioOn();
    bool isPlaying();
    bool isPaused();
    bool isRecording();
    
    void setPaused(bool b); // not true pause, just stops updating context for live, recording and playback streams
    
	void setUseTexture(bool useTexture);
	void setDepthColoring(DepthColoring coloring);
	
    void setUseBackBuffer(bool b);
    bool getUseBackBuffer();
    
    // recording methods
    bool startRecording(string ofOniFileName, XnCodecID depthFormat = XN_CODEC_16Z_EMB_TABLES, XnCodecID imageFormat = XN_CODEC_JPEG, XnCodecID infraFormat = XN_CODEC_JPEG, XnCodecID audioFormat = XN_CODEC_NULL);
    bool stopRecording();
    
    // playback methods
    bool startPlayer(string ofOniFileName); // same as setupFromONIbut but allows switching from a device to playback
    
    void setLooped(bool b);
    bool getLooped();
    
    void setSpeed(float speed);
    float getSpeed();
    
    void setFrame(int frame);
    int getCurrentFrame();
    int getTotalNumFrames();
    
    void setPosition(float pct);
    float getPosition();
    
    void firstFrame();
    void nextFrame();
    void previousFrame();
    
    bool getIsONIDone();
    
    // user tracker methods
    ofxOpenNIUser&	getTrackedUser(int index); // only returns tracked users upto getNumTrackedUsers()
    int	getNumTrackedUsers();
    
    ofxOpenNIUser& getUser(XnUserID nID); // finds a user if it exists (whether tracked or not)
    
    void setMaxNumUsers(int numUsers);
    int	getMaxNumUsers();
    
    void setUseMaskTextureAllUsers(bool b);
    void setUseMaskPixelsAllUsers(bool b);
    void setUsePointCloudsAllUsers(bool b);
    void setPointCloudDrawSizeAllUsers(int size);
    void setPointCloudResolutionAllUsers(int resolution);
    
    void setBaseUserClass(ofxOpenNIUser & user);
    
    void resetUserTracking(XnUserID nID, bool forceImmediateRestart = false);
    
	void setUserSmoothing(float smooth);
	float getUserSmoothing();
    
    bool getAutoUserCalibrationPossible();
    
    bool setSkeletonProfile(XnSkeletonProfile profile);
    XnSkeletonProfile getSkeletonProfile();
    
    // gesture methods
    bool addAllGestures();
    bool removeAllGestures();
    bool addGesture(string niteGestureName, ofPoint LeftBottomNear = NULL, ofPoint RightTopFar = NULL);
    bool removeGesture(string niteGestureName);
    
    vector<string> & getAvailableGestures();
    bool isGestureAvailable(string niteGestureName);
    
    void setMinTimeBetweenGestures(int millis);
    int getMinTimeBetweenGestures();
    
    // hands methods
    bool addAllHandFocusGestures();
    bool removeAllHandFocusGestures();
    bool addHandFocusGesture(string niteGestureName, ofPoint LeftBottomNear = NULL, ofPoint RightTopFar = NULL);
    bool removeHandFocusGesture(string niteGestureName);
    
    ofxOpenNIHand& getTrackedHand(int index); // only returns tracked hands upto getNumTrackedHands()
    int	getNumTrackedHands();
    
    ofxOpenNIHand& getHand(XnUserID nID); // finds a hand if it exists (whether tracked or not)
    
    void setMaxNumHands(int numHands);
    int	getMaxNumHands();
    
    void setMinTimeBetweenHands(int millis);
    int getMinTimeBetweenHands();
    
    void setMinDistanceBetweenHands(int worldDistance);
    int getMinDistanceBetweenHands();
    
    void setBaseHandClass(ofxOpenNIHand & hand);
    
    // generator 'capabilities'
    void toggleRegister();
	void setRegister(bool b);
    bool getRegister();
    
    void toggleMirror();
	void setMirror(bool b);
    bool getMirror();
    
    void toggleSync();
	void setSync(bool b);
    bool getSync();
    
	ofPixels& getDepthPixels();
	ofShortPixels& getDepthRawPixels();
	ofPixels& getImagePixels();
	
	ofTexture& getDepthTextureReference();
	ofTexture& getimageTextureReference();
	
	ofPoint worldToProjective(const ofPoint & p);
	ofPoint worldToProjective(const XnVector3D & p);
	
	ofPoint projectiveToWorld(const ofPoint & p);
	ofPoint projectiveToWorld(const XnVector3D & p);
	
	ofPoint cameraToWorld(const ofVec2f& c);
	void cameraToWorld(const vector<ofVec2f>& c, vector<ofVec3f>& w);
	
    int getDeviceID();
    
    xn::Context& getContext();
	xn::Device& getDevice();
   
	xn::DepthGenerator& getDepthGenerator();
	xn::ImageGenerator& getImageGenerator();
	xn::IRGenerator& getIRGenerator();
    xn::UserGenerator& getUserGenerator();
    xn::GestureGenerator& getGestureGenerator();
    xn::HandsGenerator& getHandsGenerator();
	xn::AudioGenerator& getAudioGenerator();
	
	xn::DepthMetaData& getDepthMetaData();
	xn::ImageMetaData& getImageMetaData();
	xn::IRMetaData& getIRMetaData();
	xn::AudioMetaData& getAudioMetaData();
	
    string LOG_NAME;
    
    ofEvent<ofxOpenNIUserEvent> userEvent;
    ofEvent<ofxOpenNIGestureEvent> gestureEvent;
    ofEvent<ofxOpenNIHandEvent> handEvent;

protected:
	
	void threadedFunction();
    
private:
    
    bool init(string oniFilePath, string xmlFilePath, bool threaded);
    bool initContext(string xmlFilePath = "");
    bool initDevice();
    bool initCommon();
    
    void stopCommon();
    
    bool setGeneratorResolution(xn::MapGenerator & generator, int w, int h, int f);
    
    bool addLicence(string sVendor, string sKey);
    void logErrors(xn::EnumerationErrors & errors);
    
    void addGenerator(XnPredefinedProductionNodeType type, bool & bIsOn);
    void removeGenerator(XnPredefinedProductionNodeType type, bool & bIsOn);
    
	void updateGenerators();
    
	void allocateDepthBuffers();
	void allocateDepthRawBuffers();
	void allocateImageBuffers();
	void allocateIRBuffers();
    bool allocateUsers();
    bool allocateGestures();
    bool allocateHands();
    
    void updateDepthPixels();
	void updateImagePixels();
	void updateIRPixels();
    void updateHandTracker();
    void updateUserTracker();
    void updateUserPixels(ofxOpenNIUser & user);
	void updatePointClouds(ofxOpenNIUser & user);
	void updateRecorder();
    
	bool g_bIsDepthOn;
	bool g_bIsImageOn;
	bool g_bIsInfraOn;
    bool g_bIsUserOn;
    bool g_bIsGestureOn;
    bool g_bIsHandsOn;
	bool g_bIsAudioOn;
	bool g_bIsDepthRawOnOption;
    bool g_bIsRecordOn;
    bool g_bIsPlayerOn;

    bool bIsLooped;
    bool bIsThreaded;
    bool bIsContextReady;
    bool bIsDeviceReady;
    bool bIsShuttingDown;
    bool bIsRecording;
    
    bool bUseBackBuffer;
	bool bUseTexture;
    bool bUseSafeThreading;
    
    bool bUseRegistration;
    bool bUseMirror;
    bool bUseSync;
    
    bool bAutoCalibrationPossible;
    
    bool bPaused;
	bool bNewPixels;
	bool bNewFrame;
    bool bUsePointClouds;
    bool bUseMaskPixels;
    
    int numDevices;
    
    int width;
    int height;
    int fps;
    
	// depth
	ofTexture depthTexture;
	ofPixels depthPixels[2];
	ofPixels* backDepthPixels;
	ofPixels* currentDepthPixels;
	DepthColoring depthColoring;
	float maxDepth;
	
	// depth raw
	ofShortPixels depthRawPixels[2];
	ofShortPixels* backDepthRawPixels;
	ofShortPixels* currentDepthRawPixels;
	
	// image
	ofTexture imageTexture;
	ofPixels imagePixels[2];
	ofPixels* backImagePixels;
	ofPixels* currentImagePixels;
	
    // generators
    xn::Context g_Context;
	xn::Device g_Device;
	xn::DepthGenerator g_Depth;
	xn::ImageGenerator g_Image;
	xn::IRGenerator g_Infra;
	xn::UserGenerator g_User;
    xn::GestureGenerator g_Gesture;
    xn::GestureGenerator g_HandsFocusGesture;
    xn::HandsGenerator g_Hands;
    xn::SceneAnalyzer g_Scene;
	xn::AudioGenerator g_Audio;
    xn::Recorder g_Recorder;
    xn::Player g_Player;
    
	// meta data
	xn::DepthMetaData g_DepthMD;
	xn::ImageMetaData g_ImageMD;
	xn::IRMetaData	g_InfraMD;
    xn::SceneMetaData g_SceneMD;
	xn::AudioMetaData g_AudioMD;
	
	// generators/nodes
	//xn::MockDepthGenerator mockDepth;
	
    // xml storage
    string ofXmlFilePath;
    
    // oni storage
    string oniFilePath;
    XnCodecID oniDepthFormat;
    XnCodecID oniImageFormat;
    XnCodecID oniInfraFormat;
    XnCodecID oniAudioFormat;
    
    ONITask g_ONITask;
    
    // user callback handlers
    static void XN_CALLBACK_TYPE UserCB_handleNewUser(xn::UserGenerator& userGenerator, XnUserID nID, void* pCookie);
    static void XN_CALLBACK_TYPE UserCB_handleLostUser(xn::UserGenerator& userGenerator, XnUserID nID, void* pCookie);
    static void XN_CALLBACK_TYPE UserCB_handlePoseDetected(xn::PoseDetectionCapability& poseDetectionCapability, const XnChar* strPose, XnUserID nID, void* pCookie);
    static void XN_CALLBACK_TYPE UserCB_handleCalibrationStart(xn::SkeletonCapability& skeletonCapability, XnUserID nID, void* pCookie);
//    static void XN_CALLBACK_TYPE UserCB_handleCalibrationProgress(xn::SkeletonCapability& skeletonCapability, XnUserID nID, XnCalibrationStatus calibrationStatus, void* pCookie);
    static void XN_CALLBACK_TYPE UserCB_handleCalibrationEnd(xn::SkeletonCapability& skeletonCapability, XnUserID nID, XnCalibrationStatus calibrationStatus, void* pCookie);
    
    // user tracking functions
    XnChar	userCalibrationPose[20];
    void stopTrackingUser(XnUserID nID);
    void startTrackingUser(XnUserID nID);
    void requestCalibration(XnUserID nID);
    void startPoseDetection(XnUserID nID);
    void stopPoseDetection(XnUserID nID);
    
    // user storage
	map<XnUserID, ofxOpenNIUser> currentTrackedUsers;
	vector<XnUserID> currentTrackedUserIDs;
    ofxOpenNIUser baseUser;
    
    int maxNumUsers;
    float userSmoothFactor;
    XnSkeletonProfile skeletonProfile;

    // gesture storage
    vector<string> availableGestures;
    ofxOpenNIGestureEvent lastGestureEvent;
    int minTimeBetweenGestures;
    
    // gesture callbacks
    static void XN_CALLBACK_TYPE GestureCB_handleGestureRecognized(xn::GestureGenerator& gestureGenerator, const XnChar* strGesture, const XnPoint3D* pIDPosition, const XnPoint3D* pEndPosition, void* pCookie);
    static void XN_CALLBACK_TYPE GestureCB_handleGestureProgress(xn::GestureGenerator& gestureGenerator, const XnChar* strGesture, const XnPoint3D* pIDPosition, XnFloat fProgres, void* pCookie);
    
    // hands storage
    map<XnUserID, ofxOpenNIHand> currentTrackedHands;
	vector<XnUserID> currentTrackedHandIDs;
    
    ofxOpenNIHand baseHand;
    ofxOpenNIHandEvent lastHandEvent;
    
    int maxNumHands;
    int minTimeBetweenHands;
    int minDistanceBetweenHands;
    
    // hands callbacks
    static void XN_CALLBACK_TYPE HandsCB_handleGestureRecognized(xn::GestureGenerator& gestureGenerator, const XnChar* strGesture, const XnPoint3D* pIDPosition, const XnPoint3D* pEndPosition, void* pCookie);
    static void XN_CALLBACK_TYPE HandsCB_handleGestureProgress(xn::GestureGenerator& gestureGenerator, const XnChar* strGesture, const XnPoint3D* pIDPosition, XnFloat fProgress, void* pCookie);
    static void XN_CALLBACK_TYPE HandsCB_handleHandCreate(xn::HandsGenerator& handsGenerator, XnUserID nID, const XnPoint3D* pPosition, XnFloat fTime, void* pCookie);
	static void XN_CALLBACK_TYPE HandsCB_handleHandUpdate(xn::HandsGenerator& handsGenerator, XnUserID nID, const XnPoint3D* pPosition, XnFloat fTime, void* pCookie);
	static void XN_CALLBACK_TYPE HandsCB_handleHandDestroy(xn::HandsGenerator& handsGenerator, XnUserID nID, XnFloat fTime, void* pCookie);
	
    int instanceID;
    ofLogLevel logLevel;
    
    // block copy ctor and assignment operator
    ofxOpenNI(const ofxOpenNI& other);
    ofxOpenNI& operator=(const ofxOpenNI&);
	
};

#endif
