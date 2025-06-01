#ifndef FACE_DETECTION_PROCESSOR_H
#define FACE_DETECTION_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <QString>

class FaceDetectionProcessor {
public:
    // Renvoie une image annotée avec les visages détectés, ou une image vide en cas d'échec
    static cv::Mat detectFaces(const QString& cascadePath, const QString& imagePath, bool& success);
};

#endif // FACE_DETECTION_PROCESSOR_H
