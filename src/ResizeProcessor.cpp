#include "ResizeProcessor.h"

cv::Mat ResizeProcessor::resizeImage(const cv::Mat& input, int width, int height) {
    cv::Mat result;
    cv::resize(input, result, cv::Size(width, height));
    return result;
}