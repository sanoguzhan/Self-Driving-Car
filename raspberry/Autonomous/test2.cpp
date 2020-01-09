#include <opencv2/opencv.hpp>
#include <raspicam_cv.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>
#include <wiringPi.h>
#include <string>

using namespace std;
using namespace cv;
using namespace raspicam;

//Creates the frames
Mat Frame, FramePerspective, FrameGray, Matrix, FrameThreshold, FrameEdge, FrameFinal;
Mat RnLane, FrameFinalDuplicate, FrameFinalDuplicate0, RnLaneEnd,FrameScreen;
RaspiCam_Cv Camera;

//Prototypes
void Capture();
void Threshold();
void RegionofInterest();
void Histogram();
void LineDetector();
void LineCenter();
void ScreenOutput(std::string direction);
void MotorCommands(int &diff, int &LaneEnd);
void StopDetection();
void ObjectDetection();

Point2f Source[] ={Point2f(60,160), Point2f(325,160), Point2f(20,210), Point2f(365,210)};
Point2f Destination[] ={Point2f(120,0), Point2f(320,0), Point2f(120,240), Point2f(320,240)};

//Global variables
stringstream ss;
vector<int> HistogramLane;
vector<int> HistogramLaneEnd;
int LeftLane, RightLane, linecenter, framecenter, diff, LaneEnd;

//Neural Network
CascadeClassifier StopCascade, ObjectCascade;
Mat FrameStop, ROIStop, GrayStop, ROIObject, GrayObject, FrameObject;
vector<Rect> Stop;
vector<Rect> Object;

int StopDistance, ObjectDistance;

void Setup (int argc, char **argv, RaspiCam_Cv &Camera){
	Camera.set( CAP_PROP_FRAME_WIDTH,("-w", argc, argv, 400));
	Camera.set( CAP_PROP_FRAME_HEIGHT, ("-h", argc, argv, 240));
	Camera.set( CAP_PROP_BRIGHTNESS, ("-br", argc, argv, 50));
	Camera.set( CAP_PROP_CONTRAST, ("-co", argc, argv, 50));
	Camera.set( CAP_PROP_SATURATION, ("-sa", argv, argc, 50));
	Camera.set( CAP_PROP_GAIN, ("-g", argc, argv, 50));
	Camera.set( CAP_PROP_FPS, ("-fps", argc, argv,0));
	}
	


int main(int argc, char **argv){
	
	wiringPiSetup();
	pinMode(21,OUTPUT);
	pinMode(22,OUTPUT);
	pinMode(23,OUTPUT);
	pinMode(24,OUTPUT);
	
	Setup(argc, argv, Camera);
	cout << "Connection to the Camera " << endl;
	if(!Camera.open()){
		cout << "Failed to connect to camera " << endl;
		return -1;
		}
	cout << "Camera ID= " << Camera.getId() << endl;
	
	while(1){
		auto start = std::chrono::system_clock::now();
		
		Capture();

		waitKey(1);
		auto end = std::chrono::system_clock::now();
		
		std::chrono::duration<double> elapsed_seconds = end-start;
		
		float t = elapsed_seconds.count();
		int FPS = 1/t;
		

		MotorCommands(diff, LaneEnd);
		
		
		RegionofInterest();
		Threshold();
		Histogram();
		LineDetector();
		LineCenter();
		StopDetection();
		ObjectDetection();
		
		//((namedWindow("Frame_Gray", WINDOW_KEEPRATIO);
		//moveWindow("Frame_Gray", 280,10);
		//resizeWindow("Frame_Gray",360,240);
		//imshow("Frame_Gray", FrameThreshold);
		
		//cout << "FPS = " <<FPS << endl;
		ss.str(" ");
		ss.clear();
		ss << "FPS: "  << FPS;
		putText(FrameScreen, ss.str(), Point2f(1,20), 0, 0.6, Scalar(0,0,0), 2);
		


		
		namedWindow("Frame_RGB", WINDOW_KEEPRATIO);
		moveWindow("Frame_RGB", 1100,10);
		resizeWindow("Frame_RGB",720,480);
		imshow("Frame_RGB", FrameScreen);
		
		namedWindow("Perspective", WINDOW_KEEPRATIO);
		moveWindow("Perspective", 880,490);
		resizeWindow("Perspective",480,360);
		imshow("Perspective", FramePerspective);
		
		namedWindow("Frame_Edge", WINDOW_KEEPRATIO);
		moveWindow("Frame_Edge", 1340,490);
		resizeWindow("Frame_Edge",480,360);
		imshow("Frame_Edge", FrameFinal);
		/*
		namedWindow("Frame_Object", WINDOW_KEEPRATIO);
		moveWindow("Frame_Object", 500,490);
		resizeWindow("Frame_Object",480,360);
		imshow("Frame_Object", FrameObject); 
				*/
		//namedWindow("Stop Sign", WINDOW_KEEPRATIO);
		//moveWindow("Stop Sign", 380,10);
		//resizeWindow("Stop Sign",720,480);
		//imshow("Stop Sign", ROIStop);
		
		
		}
	return 0;
	}
	
	
	
void RegionofInterest(){
	line(Frame,Source[0], Source[1], Scalar(0,0,255), 1);
	line(Frame,Source[1], Source[3], Scalar(0,0,255), 1);
	line(Frame,Source[3], Source[2], Scalar(0,0,255), 1);
	line(Frame,Source[2], Source[0], Scalar(0,0,255), 1);
	
	//line(Frame,Destination[0], Destination[1], Scalar(0,255,0), 1);
	//line(Frame,Destination[1], Destination[3], Scalar(0,255,0), 1);
	//line(Frame,Destination[3], Destination[2], Scalar(0,255,0), 1);
	//line(Frame,Destination[2], Destination[0], Scalar(0,255,0), 1);
	
	Matrix = getPerspectiveTransform(Source, Destination);   //Perspective transformation of Region of interest
	
	warpPerspective(Frame,FramePerspective, Matrix, Size(400,240));
	}
	
void Capture(){
	
	Camera.grab();
	Camera.retrieve(Frame);
	cvtColor(Frame,Frame,COLOR_BGR2RGB);
	cvtColor(Frame,FrameObject,COLOR_BGR2RGB);
	cvtColor(Frame,FrameStop,COLOR_BGR2RGB);
}

void Threshold(){
	cvtColor(FramePerspective, FrameGray, COLOR_RGB2GRAY);
	inRange(FrameGray,180,255,FrameThreshold);
	inRange(FrameGray,150,255,FrameGray);
	Canny(FrameGray, FrameEdge, 170, 400 ,3,false);
	add(FrameThreshold, FrameEdge, FrameFinal);
	cvtColor(FrameFinal, FrameFinal, COLOR_GRAY2RGB);
	cvtColor(FrameFinal, FrameFinalDuplicate, COLOR_RGB2GRAY); // for histogram only 
	cvtColor(FrameFinal, FrameFinalDuplicate0, COLOR_RGB2GRAY); // for histogram only
	}
	
void Histogram(){
	
	HistogramLane.resize(400);
	HistogramLane.clear();

	for(size_t i{0}; i < Frame.size().width; ++i){
		
		RnLane = FrameFinalDuplicate(Rect(i,140,1,100));
		divide(255, RnLane, RnLane);
		HistogramLane.push_back((int)(sum(RnLane)[0]));
		
		}
			
	HistogramLaneEnd.resize(400);
	HistogramLaneEnd.clear();

	for(size_t i{0}; i < Frame.size().width; ++i){
		
		RnLaneEnd = FrameFinalDuplicate0(Rect(i,0,1,240));
		divide(255, RnLaneEnd, RnLaneEnd);
		HistogramLaneEnd.push_back((int)(sum(RnLaneEnd)[0]));
		
		}
		LaneEnd = sum(HistogramLaneEnd)[0];
		cout << "Lane End: " << LaneEnd << endl;
}

void LineCenter(){
	linecenter = (RightLane - LeftLane) / 2 + LeftLane;
	framecenter = 214;
	
	line(FrameFinal, Point2f(linecenter, 0 ), Point2f(linecenter, 240), Scalar(255,255,0), 3);
	line(FrameFinal, Point2f(framecenter, 0 ), Point2f(framecenter, 240), Scalar(255,0,0), 3);
	
	diff =  linecenter - framecenter;
	/*
		ss.str(" ");
		ss.clear();
		ss << "FPS: "  << diff;
		putText(Frame, ss.str(), Point2f(1,20), 0, 0.6, Scalar(0,0,0), 2);*/
	}

void LineDetector(){
	vector<int>:: iterator LeftPtr;
	LeftPtr = max_element(HistogramLane.begin(), HistogramLane.begin() +200);
	LeftLane = distance(HistogramLane.begin(),LeftPtr);
	
	vector<int>:: iterator RightPtr;
	RightPtr = max_element(HistogramLane.begin() + 240 , HistogramLane.end());
	RightLane = distance(HistogramLane.begin(), RightPtr);
	
	line(FrameFinal, Point2f(LeftLane, 0), Point2f(LeftLane, 240), Scalar(0,255,0),3);
	line(FrameFinal, Point2f(RightLane, 0), Point2f(RightLane, 240), Scalar(0,255,0),3);
	}
	
void MotorCommands(int &diff, int &LaneEnd){
	
			if(StopDistance > 5 && StopDistance < 25){
			digitalWrite(21,0);			//8
			digitalWrite(22,0);
			digitalWrite(23,0);
			digitalWrite(24,1);
			StopDistance = 0;
			goto StopS;
			}
			
			if(ObjectDistance > 5 && ObjectDistance < 25){
			digitalWrite(21,1);			//9
			digitalWrite(22,0);
			digitalWrite(23,0);
			digitalWrite(24,1);
			ObjectDistance = 0;
			goto ObjectS;
			}
				//Line End
			if(LaneEnd > 6000){
			digitalWrite(21,1);			//7
			digitalWrite(22,1);
			digitalWrite(23,1);
			digitalWrite(24,0);
			putText(FrameScreen, "Lane End", Point2f(140,90), 0, 0.7, Scalar(0,50,250), 2);
			}

		
		 if(diff > -15 && diff < 15){
			digitalWrite(21,0);			//0
			digitalWrite(22,0);
			digitalWrite(23,0);
			digitalWrite(24,0);
			ScreenOutput("Forward");
		}else if(diff > 15 && diff < 25){
			digitalWrite(21,1);			//1
			digitalWrite(22,0);
			digitalWrite(23,0);
			digitalWrite(24,0);
			ScreenOutput("Right");
		}else if(diff >= 25 && diff < 35){
			digitalWrite(21,0);			//2
			digitalWrite(22,1);
			digitalWrite(23,0);
			digitalWrite(24,0);
			ScreenOutput("Right");
		}else if(diff > 35){
			digitalWrite(21,1);			//3
			digitalWrite(22,1);
			digitalWrite(23,0);
			digitalWrite(24,0);
			ScreenOutput("Right");
		}else if(diff < -15 && diff > -25){
			digitalWrite(21,0);			//4
			digitalWrite(22,1);
			digitalWrite(23,1);
			digitalWrite(24,0);
			ScreenOutput("Left");
		}else if(diff <= -25 && diff > -35){
			digitalWrite(21,1);			//5
			digitalWrite(22,0);
			digitalWrite(23,1);
			digitalWrite(24,0);
			ScreenOutput("Left");
		}else if(diff < -35){
			digitalWrite(21,0);			//6
			digitalWrite(22,1);
			digitalWrite(23,1);
			digitalWrite(24,0);
			ScreenOutput("Left");
		}
		
		StopS:
		ObjectS:
		{}
		
	}
	
void ScreenOutput(string direction){
		ss.str(" ");
		ss.clear();
		ss << "Diretion: "  << direction;
		putText(FrameScreen, ss.str(), Point2f(110,20), 0, 0.7, Scalar(255,0,0), 2);
	}
	
void StopDetection(){
	
	if(!StopCascade.load("//home//pi//Desktop//Autonomous//stop_cascade.xml")){
		cout << "Couldn't open the file " << endl;
		}
		FrameScreen = FrameStop(Rect(0,0,400,240));
		ROIStop = FrameStop(Rect(200,0,200,240));
		cvtColor(ROIStop, GrayStop, COLOR_RGB2GRAY);
		equalizeHist(GrayStop, GrayStop);
		StopCascade.detectMultiScale(GrayStop, Stop);
		
		for(size_t i{0}; i < Stop.size(); i++){
			Point P1(Stop[i].x, Stop[i].y);
			Point P2(Stop[i].x + Stop[i].width, Stop[i].y + Stop[i].height);
			rectangle(ROIStop, P1, P2, Scalar(0,0,255), 2);
			putText(ROIStop, "STOP SIGN", P1, FONT_HERSHEY_PLAIN, 1,Scalar(0,0,0), 1);
			StopDistance =(-0.5) * (P2.x - P1.x) + 50;
			ss.str(" ");
			ss.clear();
			ss << "Stop Dist: "  << StopDistance << "cm";
			putText(FrameScreen, ss.str(), Point2f(1,45), 0, 0.8, Scalar(0,0,255), 1);
			
			}
	}
	
void ObjectDetection(){
	
	if(!ObjectCascade.load("//home//pi//Desktop//Autonomous//object_cascade.xml")){
		cout << "Couldn't open the file " << endl;
		}
		//FrameScreen = FrameObject(Rect(0,0,400,240));
		ROIObject = FrameObject(Rect(0,0,400,240));
		cvtColor(ROIObject, GrayObject, COLOR_RGB2GRAY);
		equalizeHist(GrayObject, GrayObject);
		ObjectCascade.detectMultiScale(GrayObject, Object);
		
		for(size_t i{0}; i < Object.size(); i++){
			Point P1(Object[i].x, Object[i].y);
			Point P2(Object[i].x + Object[i].width, Object[i].y + Object[i].height);
			rectangle(FrameScreen, P1, P2, Scalar(0,0,255), 2);
			putText(FrameScreen, "OBJECT", P1, FONT_HERSHEY_PLAIN, 1,Scalar(50,50,50), 1);
			ObjectDistance =(-0.68) * (P2.x - P1.x) + 59;
			ss.str(" ");
			ss.clear();
			ss << "Object Dist: "  << ObjectDistance << "cm";
			putText(FrameScreen, ss.str(), Point2f(1,62), 0, 0.8, Scalar(0,0,255), 1);
			
			}
	}
