#include <KalmanFilterForUserDetection.hpp>



using name space std

Kalman_Filter_User::Kalman_Filter_User(){}


Kalman_Filter_User::Kalman_Filter_User_Detection(){

//image to show tracking
Mat img (500, 500, CV 8uc3);
KalmanFilter KF(3,3,0); // parametres 
Mat imG(500, 500, CV_8UC3);
KalmanFilter KF(5,3,2); // KF(dimension of state, dimension ofmeasurement,dimension of control vector, type of created matrices)
Mat state(5, 1, CV_32F);
Mat processNoise(2, 1, CV_32F);
Mat measurement = Mat::zeros(3,1, CV_32F);

state

//init
KF.statePre.at<float>(0) = x;
KF.statePre.at<float>(1) = y;
KF.statePre.at<float>(2) = r;
KF.statePre.at<float>(3) = 0; // speedx
KF.statePre.at<float>(4) = 0;	//speed y
KF.transitionMatrix = *(Mat_<float>(5, 5) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1, 0,0,1,0);
setIdentity(KF.measurementMatrix);
setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
setIdentity(KF.errorCovPost, Scalar::all(1));

// First predict, to update the internal statePre variable
Mat prediction = KF.predict(); //compute a predicted state
Point predictPt(prediction.at<float>(0),prediction.at<float>(1),prediction.at<float>(2) );




//Get position x, y and radius r

measurement(0) = x;
measurement(1) = y;
measurement(2) = r;
Point measPt(measurement(0),measurement(1), measurement(2));

//The "correct" phase that is going to  use the predicted value and our measurement
Mat estimated = KF.correct(measurement);
Point statePt(estimated.at<float>(0),estimated.at<float>(1), estimated.at<float>(2));
}

