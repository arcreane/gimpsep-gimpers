#ifndef FACE_DETECTION_PROCESSOR_H
#define FACE_DETECTION_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <QString>

class FaceDetectionProcessor {
public:
    static cv::Mat detectFaces(const QString& cascadePath, const cv::Mat& image, bool& success);
};

#endif // FACE_DETECTION_PROCESSOR_H
