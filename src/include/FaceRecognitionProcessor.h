#ifndef FACERECOGNITIONPROCESSOR_H
#define FACERECOGNITIONPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <QString>
#include <string>

class FaceRecognitionProcessor {
public:
    struct RecognitionResult {
        std::string subjectName;
        double confidence;
        cv::Mat image;
        bool success;
    };

    // Original version
    static RecognitionResult recognizeFace(const QString& datasetDir, const QString& probePath);

    // New overload for GUI (uses current image)
    static RecognitionResult recognizeFace(const QString& datasetDir, const cv::Mat& image);

};

#endif // FACERECOGNITIONPROCESSOR_H
