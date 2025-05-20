//
// Created by Ines Mansour on 19/05/2025.
//

#ifndef EDGE_DETECTOR_H
#define EDGE_DETECTOR_H

#include <opencv2/core.hpp>

class EdgeDetector {
public:
    static cv::Mat detectEdges(const cv::Mat& image, double threshold1, double threshold2);
};

#endif
