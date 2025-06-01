#include "FaceDetectionProcessor.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

cv::Mat FaceDetectionProcessor::detectFaces(const QString& cascadePath, const QString& imagePath, bool& success) {
    success = false;

    // Chargement du classifieur
    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load(cascadePath.toStdString())) {
        std::cerr << "Erreur de chargement du cascade: " << cascadePath.toStdString() << std::endl;
        return cv::Mat();
    }

    // Lecture de l'image
    cv::Mat image = cv::imread(imagePath.toStdString());
    if (image.empty()) {
        std::cerr << "Erreur de lecture de l'image: " << imagePath.toStdString() << std::endl;
        return cv::Mat();
    }

    // Prétraitement
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    // Détection
    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(gray, faces);

    // Dessin des rectangles
    for (const auto& face : faces) {
        cv::rectangle(image, face, cv::Scalar(0, 255, 0), 2);
    }

    success = true;
    return image;
}
