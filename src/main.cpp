#include <iostream>
#include <string>
#include <limits>
#include <opencv2/opencv.hpp>

#include "../include/BrightnessProcessor.h"
#include "../include/EdgeDetector.h"
#include "../include/MorphologyProcessor.h"
#include "../include/ResizeProcessor.h"
#include "../include/BackgroundSubtractor.h"
#include "../include/FaceRecognitionProcessor.h"
#include "../include/FaceDetectionProcessor.h"
#include "../include/ConfigManager.h"
#include "../include/CommonFunctions.h"

#include <QApplication>
#include "PanoramaStitcher.h"
#include "../gui/ImageEditorGUI.h"

void printMenu() {
    std::cout << "\n===== Image Editor CLI =====\n";
    std::cout << "1. Panorama \n";
    std::cout << "2. Face Detection \n";
    std::cout << "3. Face Recognition \n";
    std::cout << "4. Adjust Brightness\n";
    std::cout << "5. Canny Edge Detection\n";
    std::cout << "6. Morphology Operations\n";
    std::cout << "7. Resize Image\n";
    std::cout << "8. Background Subtraction\n";
    std::cout << "9. Launch GUI\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

void runPanoramaIntegrated() {
    int numImages;
    std::cout << "How many images? (2+): ";
    std::cin >> numImages;
    if (numImages < 2) {
        std::cerr << "Need at least two images.\n";
        CommonFunctions::pauseAndReturn();
        return;
    }

    std::vector<std::string> paths(numImages);
    for (int i = 0; i < numImages; ++i) {
        std::cout << "Path to image " << (i + 1) << ": ";
        std::cin >> paths[i];
    }

    PanoramaStitcher stitcher;
    if (!stitcher.loadImages(paths) || !stitcher.createPanorama()) {
        std::cerr << "Panorama creation failed.\n";
        CommonFunctions::pauseAndReturn();
        return;
    }

    CommonFunctions::showImageAndSave("Panorama Result", stitcher.getPanorama(), "images/output/panorama_result.jpg");
}

void runFaceDetectionIntegrated() {
    QString cascadePath = ConfigManager::instance().getFaceCascadePath();
    if (cascadePath.isEmpty()) {
        std::cerr << "Cascade path not configured.\n";
        CommonFunctions::pauseAndReturn();
        return;
    }

    std::string imagePath;
    std::cout << "Path to image: ";
    std::cin >> imagePath;

    cv::Mat image;
    if (!CommonFunctions::loadImage(imagePath, image)) return;

    bool success = false;
    cv::Mat result = FaceDetectionProcessor::detectFaces(cascadePath, image, success);
    if (!success) {
        std::cerr << "Face detection failed.\n";
        CommonFunctions::pauseAndReturn();
        return;
    }

    CommonFunctions::showImageAndSave("Faces Detected", result, "images/output/faces_detected.jpg");
}

void runFaceRecognition() {
    QString datasetDir = ConfigManager::instance().getFaceDatasetPath();
    if (datasetDir.isEmpty()) {
        std::cerr << "Dataset path not configured.\n";
        CommonFunctions::pauseAndReturn();
        return;
    }

    std::string probePath;
    std::cout << "Path to image to recognize: ";
    std::cin >> probePath;

    cv::Mat probeImage;
    if (!CommonFunctions::loadImage(probePath, probeImage, true)) return;

    auto result = FaceRecognitionProcessor::recognizeFace(datasetDir, probeImage);
    if (!result.success) {
        std::cerr << "Face recognition failed.\n";
        CommonFunctions::pauseAndReturn();
        return;
    }

    std::cout << "\n=== Face Recognition Result ===\n";
    std::cout << "Recognized Person: " << result.subjectName << "\n";
    std::cout << "Confidence: " << result.confidence << "\n";
    CommonFunctions::showImageAndSave("Recognized Face", result.image, "images/output/face_recognized.jpg");
}

void adjustBrightness() {
    std::string imagePath;
    double brightnessFactor;
    std::cout << "Path to image: "; std::cin >> imagePath;
    std::cout << "Brightness factor (-100 to 100): "; std::cin >> brightnessFactor;

    cv::Mat image;
    if (!CommonFunctions::loadImage(imagePath, image)) return;

    cv::Mat result = BrightnessProcessor::adjustBrightness(image, brightnessFactor);
    CommonFunctions::showImageAndSave("Brightened Image", result, "images/output/brightness.jpg");
}

void detectEdges() {
    std::string imagePath;
    double lowerThreshold, upperThreshold;
    std::cout << "Path to image: "; std::cin >> imagePath;
    std::cout << "Lower threshold: "; std::cin >> lowerThreshold;
    std::cout << "Upper threshold: "; std::cin >> upperThreshold;

    cv::Mat image;
    if (!CommonFunctions::loadImage(imagePath, image)) return;

    cv::Mat edges = EdgeDetector::detectEdges(image, lowerThreshold, upperThreshold);
    CommonFunctions::showImageAndSave("Canny Edges", edges, "images/output/edges.jpg");
}

void applyMorphology() {
    std::string imagePath;
    int operation, kernelSize;
    std::cout << "Path to image: "; std::cin >> imagePath;
    std::cout << "Operation (1: Dilation, 2: Erosion): "; std::cin >> operation;
    std::cout << "Kernel size (odd number): "; std::cin >> kernelSize;

    cv::Mat image;
    if (!CommonFunctions::loadImage(imagePath, image)) return;

    cv::Mat result = (operation == 1)
        ? MorphologyProcessor::applyDilation(image, kernelSize)
        : MorphologyProcessor::applyErosion(image, kernelSize);

    CommonFunctions::showImageAndSave("Morphology Result", result, "images/output/morphology.jpg");
}

void resizeImage() {
    std::string imagePath;
    int width, height;
    std::cout << "Path to image: "; std::cin >> imagePath;
    std::cout << "New width: "; std::cin >> width;
    std::cout << "New height: "; std::cin >> height;

    cv::Mat image;
    if (!CommonFunctions::loadImage(imagePath, image)) return;

    cv::Mat result = ResizeProcessor::resizeImage(image, width, height);
    CommonFunctions::showImageAndSave("Resized Image", result, "images/output/resized.jpg", width, height);
}

void runBackgroundSubtraction() {
    std::string inputVideoPath, outputVideoPath;
    std::cout << "Path to input video: "; std::cin >> inputVideoPath;
    std::cout << "Path to output video: "; std::cin >> outputVideoPath;
    BackgroundSubtractor::processVideo(inputVideoPath, outputVideoPath);
    CommonFunctions::pauseAndReturn();
}

void runGUI(int argc, char* argv[]) {
    QApplication app(argc, argv);
    ImageEditorGUI window;
    window.resize(800, 600);
    window.show();
    app.exec();
}

int main(int argc, char* argv[]) {
    if (!ConfigManager::instance().loadConfig("config.json")) {
        std::cerr << "Failed to load configuration file (config.json).\n";
        return 1;
    }

    int choice = -1;
    while (true) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printMenu();
        std::cin >> choice;
        if (!std::cin) break;

        switch (choice) {
            case 1: runPanoramaIntegrated(); break;
            case 2: runFaceDetectionIntegrated(); break;
            case 3: runFaceRecognition(); break;
            case 4: adjustBrightness(); break;
            case 5: detectEdges(); break;
            case 6: applyMorphology(); break;
            case 7: resizeImage(); break;
            case 8: runBackgroundSubtraction(); break;
            case 9: runGUI(argc, argv); break;
            case 0:
                std::cout << "Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice. Please enter a valid number.\n";
        }
    }

    return 0;
}
