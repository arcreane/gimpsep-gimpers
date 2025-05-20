//
// Created by Ines Mansour on 19/05/2025.
//
#ifndef BRIGHTNESS_PROCESSOR_H
#define BRIGHTNESS_PROCESSOR_H

#include <opencv2/core.hpp>

class BrightnessProcessor {
public:
    static cv::Mat adjustBrightness(const cv::Mat& image, double brightness);
};

#endif

