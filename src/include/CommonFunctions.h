//
// Created by Ines Mansour on 02/06/2025.
//

#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H



#include <opencv2/opencv.hpp>
#include <string>

namespace CommonFunctions {
    void pauseAndReturn();
    bool loadImage(const std::string& path, cv::Mat& image, bool grayscale = false);
    void showImageAndSave(const std::string& windowName, const cv::Mat& image, const std::string& savePath, int width = 800, int height = 600);
}



#endif //COMMONFUNCTIONS_H
