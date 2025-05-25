#include "../include/MorphologyProcessor.h"

cv::Mat MorphologyProcessor::applyDilation(const cv::Mat& input, int kernelSize) {
    cv::Mat result;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,
        cv::Size(kernelSize, kernelSize));
    cv::dilate(input, result, kernel);
    return result;
}

cv::Mat MorphologyProcessor::applyErosion(const cv::Mat& input, int kernelSize) {
    cv::Mat result;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,
        cv::Size(kernelSize, kernelSize));
    cv::erode(input, result, kernel);
    return result;
}