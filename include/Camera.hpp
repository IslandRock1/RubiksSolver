//
// Created by oyste on 2/15/2025.
//

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <opencv2/opencv.hpp>

class Camera {
private:
	cv::VideoCapture cap;

public:
	explicit Camera(int cameraIX);
	[[nodiscard]] bool isOpened() const;
	void showFeed();
};



#endif //CAMERA_HPP
