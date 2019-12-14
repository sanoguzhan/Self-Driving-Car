#include <opencv2/opencv.hpp>
#include <raspicam_cv.h>
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;
using namespace cv;
using namespace raspicam;

Mat frame;

void Setup (int argc, char **argv, RaspiCam_Cv &Camera){
	Camera.set( CAP_PROP_FRAME_WIDTH,("-w", argc, argv, 400));
	Camera.set( CAP_PROP_FRAME_HEIGHT, ("-h", argc, argv, 240));
	Camera.set( CAP_PROP_BRIGHTNESS, ("-br", argv, argc, 50));
	Camera.set( CAP_PROP_CONTRAST, ("-co", argv, argc, 50));
	Camera.set( CAP_PROP_SATURATION, ("-sa", argv, argc, 50));
	Camera.set( CAP_PROP_GAIN, ("-g", argv, argc, 50));
	Camera.set( CAP_PROP_FPS, ("-fps", argv, argc, 100));
	}
int main(int argc, char **argv){
	
	RaspiCam_Cv Camera;
	Setup(argc, argv, Camera);
	cout << "Connection to the Camera " << endl;
	if(!Camera.open()){
		cout << "Failed to connect to camera " << endl;
		}
	cout << "Camera ID= " << Camera.getId() << endl;
	
	while(1){
		
		auto start = std::chrono::system_clock::now();
		Camera.grab();
		Camera.retrieve( frame);
		auto end = std::chrono::system_clock::now();
		
		std::chrono::duration<double> elapsed_seconds = end-start;
		
		float t = elapsed_seconds.count();
		int FPS = 1/t;
		cout << "FPS = " <<FPS << endl;
		imshow("Frame", frame);
		
		waitKey(1);
		}
	return 0;
	}
