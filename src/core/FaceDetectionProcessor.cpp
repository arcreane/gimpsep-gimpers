#include "FaceDetectionProcessor.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

cv::Mat FaceDetectionProcessor::detectFaces(const QString& cascadePath, const cv::Mat& image, bool& success) {
    success = false;

    if (image.empty()) {
        std::cerr << "Input image is empty." << std::endl;
        return {};
    }

    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load(cascadePath.toStdString())) {
        std::cerr << "Failed to load cascade: " << cascadePath.toStdString() << std::endl;
        return {};
    }

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(gray, faces);

    if (faces.empty()) {
        return image.clone();
    }

    cv::Mat result = image.clone();
    for (const auto& face : faces) {
        cv::rectangle(result, face, cv::Scalar(0, 255, 0), 2);
    }

    success = true;
    return result;
}
