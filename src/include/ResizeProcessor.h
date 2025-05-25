#ifndef RESIZEPROCESSOR_H
#define RESIZEPROCESSOR_H

#include <opencv2/opencv.hpp>

class ResizeProcessor {
public:
    static cv::Mat resizeImage(const cv::Mat& input, int width, int height);
};

#endif