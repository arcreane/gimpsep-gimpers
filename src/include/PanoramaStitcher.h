//
// Created by Ines Mansour on 01/06/2025.
//

#ifndef PANORAMASTITCHER_H
#define PANORAMASTITCHER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class PanoramaStitcher {
public:
    PanoramaStitcher();
    ~PanoramaStitcher();

    bool loadImages(const std::vector<std::string>& imagePaths);
    bool createPanorama();
    void showPanorama(const std::string& windowName = "Panorama") const;
    bool savePanorama(const std::string& outputPath) const;
    const cv::Mat& getPanorama() const;  // Ajout utile pour GUI

private:
    std::vector<cv::Mat> images_;
    std::vector<cv::Mat> resizedImages_;
    cv::Mat panorama_;
    int refHeight_;

    void computeReferenceHeight();
    void resizeImages();
    void stitchImages();
};

#endif // PANORAMASTITCHER_H

