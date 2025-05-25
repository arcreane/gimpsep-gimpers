//
// Created by Ines Mansour on 19/05/2025.
//
#include "../include/EdgeDetector.h"
#include <opencv2/imgproc.hpp>


cv::Mat EdgeDetector::detectEdges(const cv::Mat& image, double threshold1, double threshold2) {
    cv::Mat gray, edges;

    if (image.channels() == 3)
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    else
        gray = image.clone();

    cv::Canny(gray, edges, threshold1, threshold2);
    return edges;
}

