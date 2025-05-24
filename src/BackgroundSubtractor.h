#ifndef BACKGROUNDSUBTRACTOR_H
#define BACKGROUNDSUBTRACTOR_H

#include <opencv2/opencv.hpp>
#include <string>

class BackgroundSubtractor {
public:
    static void processVideo(const std::string& inputPath, const std::string& outputPath);
};

#endif