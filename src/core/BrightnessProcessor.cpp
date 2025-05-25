//
// Created by Ines Mansour on 19/05/2025.
//

#include "../include/BrightnessProcessor.h"


cv::Mat BrightnessProcessor::adjustBrightness(const cv::Mat& image, double brightness) {
    cv::Mat result;
    image.convertTo(result, -1, 1, brightness);  // alpha = 1.0, beta = brightness
    return result;
}
