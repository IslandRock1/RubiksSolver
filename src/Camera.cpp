//
// Created by oyste on 2/15/2025.
//

#include "Camera.hpp"

Camera::Camera(int cameraIX): cap(cameraIX) {}

bool Camera::isOpened() const {
	return cap.isOpened();
}


void Camera::showFeed() {
	cv::Mat frame;
	cap >> frame;


	cv::imshow("Camera", frame);
}

