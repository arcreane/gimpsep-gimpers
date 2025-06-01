#include <iostream>
#include <string>
#include <limits>
#include <fstream>

#include <opencv2/opencv.hpp>

#include "../include/BrightnessProcessor.h"
#include "../include/EdgeDetector.h"
#include "../include/MorphologyProcessor.h"
#include "../include/ResizeProcessor.h"
#include "../include/BackgroundSubtractor.h"
#include "../include/FaceRecognitionProcessor.h"
#include "../include/FaceDetectionProcessor.h"
#include "../include/ConfigManager.h"



#include <QApplication>

#include "PanoramaStitcher.h"
#include "../gui/ImageEditorGUI.h"

// === MENU PRINTING ===
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


void pauseAndReturn() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "\nPress Enter to return to the menu...";
    std::cin.get();
    cv::destroyAllWindows();
}

// === PANORAMA ===
void runPanoramaIntegrated() {
    int numImages;
    std::cout << "How many images? (2+): ";
    std::cin >> numImages;

    if (numImages < 2) {
        std::cerr << "Need at least two images.\n";
        pauseAndReturn();
        return;
    }

    std::vector<std::string> paths;
    for (int i = 0; i < numImages; ++i) {
        std::string path;
        std::cout << "Path to image " << (i + 1) << ": ";
        std::cin >> path;
        paths.push_back(path);
    }

    PanoramaStitcher stitcher;
    if (!stitcher.loadImages(paths)) {
        std::cerr << "Failed to load one or more images.\n";
        pauseAndReturn();
        return;
    }

    if (!stitcher.createPanorama()) {
        std::cerr << "Panorama creation failed.\n";
        pauseAndReturn();
        return;
    }

    const cv::Mat& result = stitcher.getPanorama();
    cv::imshow("Panorama Result", result);
    cv::imwrite("images/output/panorama_result.jpg", result);
    cv::waitKey(0);
    pauseAndReturn();
}



// === FACE DETECTION ===
void runFaceDetectionIntegrated() {
    QString cascadePath = ConfigManager::instance().getFaceCascadePath();
    if (cascadePath.isEmpty()) {
        std::cerr << "Cascade path not configured.\n";
        pauseAndReturn();
        return;
    }

    std::string imagePath;
    std::cout << "Path to image: ";
    std::cin >> imagePath;

    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Could not load image.\n";
        pauseAndReturn();
        return;
    }

    bool success = false;
    cv::Mat result = FaceDetectionProcessor::detectFaces(cascadePath, image, success);

    if (!success) {
        std::cerr << "Face detection failed.\n";
        pauseAndReturn();
        return;
    }

    cv::imshow("Faces Detected", result);
    cv::imwrite("images/output/faces_detected.jpg", result);
    cv::waitKey(0);
    pauseAndReturn();
}

// === FACE RECOGNITION ===
void runFaceRecognition() {
    QString datasetDir = ConfigManager::instance().getFaceDatasetPath();
    if (datasetDir.isEmpty()) {
        std::cerr << "Dataset path not configured.\n";
        pauseAndReturn();
        return;
    }

    std::string probePath;
    std::cout << "Path to image to recognize: ";
    std::cin >> probePath;

    cv::Mat probeImage = cv::imread(probePath, cv::IMREAD_GRAYSCALE);
    if (probeImage.empty()) {
        std::cerr << "Failed to load probe image.\n";
        pauseAndReturn();
        return;
    }

    auto result = FaceRecognitionProcessor::recognizeFace(datasetDir, probeImage);

    if (!result.success) {
        std::cerr << "Face recognition failed.\n";
        pauseAndReturn();
        return;
    }

    std::cout << "\n=== Face Recognition Result ===\n";
    std::cout << "Recognized Person: " << result.subjectName << "\n";
    std::cout << "Confidence: " << result.confidence << "\n";

    cv::imshow("Recognized Face", result.image);
    cv::imwrite("images/output/face_recognized.jpg", result.image);
    cv::waitKey(0);
    pauseAndReturn();
}
// === BRIGHTNESS ===
void adjustBrightness() {
    std::string imagePath;
    double brightnessFactor;

    std::cout << "Path to image: "; std::cin >> imagePath;
    std::cout << "Brightness factor (-100 to 100): "; std::cin >> brightnessFactor;

    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Error: could not load image.\n";
        pauseAndReturn(); return;
    }

    cv::Mat result = BrightnessProcessor::adjustBrightness(image, brightnessFactor);
    cv::namedWindow("Brightened Image", cv::WINDOW_NORMAL);
    cv::imshow("Brightened Image", result);
    cv::imwrite("images/output/brightness.jpg", result);
    cv::waitKey(0);
    pauseAndReturn();
}

// === CANNY ===
void detectEdges() {
    std::string imagePath;
    double lowerThreshold, upperThreshold;

    std::cout << "Path to image: "; std::cin >> imagePath;
    std::cout << "Lower threshold: "; std::cin >> lowerThreshold;
    std::cout << "Upper threshold: "; std::cin >> upperThreshold;

    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Error: could not load image.\n";
        pauseAndReturn(); return;
    }

    cv::Mat edges = EdgeDetector::detectEdges(image, lowerThreshold, upperThreshold);
    cv::namedWindow("Canny Edges", cv::WINDOW_NORMAL);
    cv::imshow("Canny Edges", edges);
    cv::imwrite("images/output/edges.jpg", edges);
    cv::waitKey(0);
    pauseAndReturn();
}

// === MORPHOLOGY ===
void applyMorphology() {
    std::string imagePath;
    int operation, kernelSize;

    std::cout << "Path to image: "; std::cin >> imagePath;
    std::cout << "Operation (1: Dilation, 2: Erosion): "; std::cin >> operation;
    std::cout << "Kernel size (odd number): "; std::cin >> kernelSize;

    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Error: could not load image.\n";
        pauseAndReturn(); return;
    }

    cv::Mat result = (operation == 1)
        ? MorphologyProcessor::applyDilation(image, kernelSize)
        : MorphologyProcessor::applyErosion(image, kernelSize);

    cv::namedWindow("Morphology Result", cv::WINDOW_NORMAL);
    cv::imshow("Morphology Result", result);
    cv::imwrite("images/output/morphology.jpg", result);
    cv::waitKey(0);
    pauseAndReturn();
}

// === RESIZE ===
void resizeImage() {
    std::string imagePath;
    int width, height;

    std::cout << "Path to image: "; std::cin >> imagePath;
    std::cout << "New width: "; std::cin >> width;
    std::cout << "New height: "; std::cin >> height;

    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Error: could not load image.\n";
        pauseAndReturn(); return;
    }

    cv::Mat result = ResizeProcessor::resizeImage(image, width, height);
    cv::namedWindow("Resized Image", cv::WINDOW_NORMAL);
    cv::resizeWindow("Resized Image", width, height);
    cv::imshow("Resized Image", result);
    cv::imwrite("images/output/resized.jpg", result);
    cv::waitKey(0);
    pauseAndReturn();
}

// === BACKGROUND SUBTRACTION ===
void runBackgroundSubtraction() {
    std::string inputVideoPath, outputVideoPath;

    std::cout << "Path to input video: "; std::cin >> inputVideoPath;
    std::cout << "Path to output video: "; std::cin >> outputVideoPath;

    BackgroundSubtractor::processVideo(inputVideoPath, outputVideoPath);
    pauseAndReturn();
}




// === GUI ===
void runGUI(int argc, char* argv[]) {
    QApplication app(argc, argv);
    ImageEditorGUI window;
    window.resize(800, 600);


    window.show();
    app.exec();
}

// === MAIN ===
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
