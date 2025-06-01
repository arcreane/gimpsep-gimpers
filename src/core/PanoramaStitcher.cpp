#include "PanoramaStitcher.h"
#include <opencv2/opencv.hpp>
#include <iostream>

PanoramaStitcher::PanoramaStitcher() : refHeight_(0) {}
PanoramaStitcher::~PanoramaStitcher() {}

bool PanoramaStitcher::loadImages(const std::vector<std::string>& imagePaths) {
    images_.clear();
    for (const auto& path : imagePaths) {
        cv::Mat img = cv::imread(path);
        if (img.empty()) {
            std::cerr << "Erreur de lecture de l'image: " << path << std::endl;
            return false;
        }
        images_.push_back(img);
    }
    return true;
}

void PanoramaStitcher::computeReferenceHeight() {
    if (images_.empty()) {
        refHeight_ = 0;
        return;
    }
    refHeight_ = images_[0].rows;
    for (const auto& img : images_) {
        refHeight_ = std::min(refHeight_, img.rows);
    }
}

void PanoramaStitcher::resizeImages() {
    resizedImages_.clear();
    for (const auto& img : images_) {
        if (img.rows != refHeight_) {
            double scale = static_cast<double>(refHeight_) / img.rows;
            int newWidth = static_cast<int>(img.cols * scale);
            cv::Mat tmp;
            cv::resize(img, tmp, cv::Size(newWidth, refHeight_));
            resizedImages_.push_back(tmp);
        } else {
            resizedImages_.push_back(img);
        }
    }
}

void PanoramaStitcher::stitchImages() {
    if (resizedImages_.empty()) {
        panorama_ = cv::Mat();
        return;
    }
    int totalWidth = 0;
    for (const auto& img : resizedImages_) {
        totalWidth += img.cols;
    }
    panorama_ = cv::Mat(refHeight_, totalWidth, resizedImages_[0].type(), cv::Scalar::all(0));
    int offsetX = 0;
    for (const auto& img : resizedImages_) {
        cv::Mat roi = panorama_(cv::Rect(offsetX, 0, img.cols, img.rows));
        img.copyTo(roi);
        offsetX += img.cols;
    }
}

bool PanoramaStitcher::createPanorama() {
    if (images_.size() < 2) {
        std::cerr << "Besoin d'au moins deux images pour créer un panorama." << std::endl;
        return false;
    }
    computeReferenceHeight();
    resizeImages();
    stitchImages();
    return !panorama_.empty();
}

void PanoramaStitcher::showPanorama(const std::string& windowName) const {
    if (!panorama_.empty()) {
        cv::imshow(windowName, panorama_);
        cv::waitKey(0);
    }
}

bool PanoramaStitcher::savePanorama(const std::string& outputPath) const {
    if (panorama_.empty()) {
        std::cerr << "Panorama vide, rien à sauvegarder." << std::endl;
        return false;
    }
    return cv::imwrite(outputPath, panorama_);
}

const cv::Mat& PanoramaStitcher::getPanorama() const {
    return panorama_;
}
