#ifndef MORPHOLOGYPROCESSOR_H
#define MORPHOLOGYPROCESSOR_H

#include <opencv2/opencv.hpp>

class MorphologyProcessor {
public:
    static cv::Mat applyDilation(const cv::Mat& input, int kernelSize);
    static cv::Mat applyErosion(const cv::Mat& input, int kernelSize);
};

#endif