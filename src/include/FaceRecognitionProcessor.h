//
// Created by Ines Mansour on 01/06/2025.
//

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

    static RecognitionResult recognizeFace(const QString& datasetDir, const QString& probePath);
};



#endif //FACERECOGNITIONPROCESSOR_H
