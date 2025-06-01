//
// Created by Ines Mansour on 02/06/2025.
//
#include "../include/CommonFunctions.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <limits>



namespace CommonFunctions {

    void pauseAndReturn() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nPress Enter to return to the menu...";
        std::cin.get();
        cv::destroyAllWindows();
    }

    bool loadImage(const std::string& path, cv::Mat& image, bool grayscale) {
        image = cv::imread(path, grayscale ? cv::IMREAD_GRAYSCALE : cv::IMREAD_COLOR);
        if (image.empty()) {
            std::cerr << "Error: Could not load image from " << path << "\n";
            pauseAndReturn();
            return false;
        }
        return true;
    }

    void showImageAndSave(const std::string& windowName, const cv::Mat& image,
                          const std::string& savePath, int width, int height) {
        cv::namedWindow(windowName, cv::WINDOW_NORMAL);
        cv::resizeWindow(windowName, width, height);
        cv::imshow(windowName, image);
        cv::imwrite(savePath, image);
        cv::waitKey(0);
        cv::destroyWindow(windowName);
    }

}
