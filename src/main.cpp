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

#include <QApplication>
#include "../gui/ImageEditorGUI.h"

// === MENU PRINTING ===
void printMenu() {
    std::cout << "\n===== Image Editor CLI =====\n";
    std::cout << "1. Panorama Stitching\n";
    std::cout << "2. Face Detection\n";
    std::cout << "3. Adjust Brightness\n";
    std::cout << "4. Canny Edge Detection\n";
    std::cout << "5. Morphology Operations\n";
    std::cout << "6. Resize Image\n";
    std::cout << "7. Background Subtraction\n";
    std::cout << "8. Launch GUI\n";
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
    std::vector<std::string> paths;
    int numImages;
    std::cout << "How many images? (2+): ";
    std::cin >> numImages;

    if (numImages < 2) {
        std::cerr << "Need at least two images.\n";
        pauseAndReturn();
        return;
    }

    for (int i = 0; i < numImages; ++i) {
        std::string path;
        std::cout << "Path to image " << (i + 1) << ": ";
        std::cin >> path;
        paths.push_back(path);
    }

    std::vector<cv::Mat> images;
    for (const auto& path : paths) {
        cv::Mat img = cv::imread(path);
        if (img.empty()) {
            std::cerr << "Failed to read: " << path << std::endl;
            pauseAndReturn();
            return;
        }
        images.push_back(img);
    }

    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
    cv::Mat pano;
    cv::Stitcher::Status status = stitcher->stitch(images, pano);

    if (status == cv::Stitcher::ERR_NEED_MORE_IMGS) {
        std::cerr << "Not enough overlap. Returning first image only.\n";
        pano = images[0].clone();
    } else if (status != cv::Stitcher::OK) {
        std::cerr << "Stitching failed. Error code: " << int(status) << std::endl;
        pauseAndReturn();
        return;
    }

    cv::imshow("Panorama", pano);
    cv::imwrite("images/output/panorama_result.jpg", pano);
    cv::waitKey(0);
    pauseAndReturn();
}


// === FACE DETECTION ===
void runFaceDetectionIntegrated() {
    std::string cascadePath, imagePath;
    std::cout << "Path to cascade XML: ";
    std::cin >> cascadePath;
    std::cout << "Path to image: ";
    std::cin >> imagePath;

    std::ifstream fs(cascadePath);
    if (!fs.good()) {
        std::cerr << "Cascade file not found: " << cascadePath << std::endl;
        pauseAndReturn();
        return;
    }

    cv::CascadeClassifier faceCascade;
    try {
        if (!faceCascade.load(cascadePath)) {
            std::cerr << "Failed to load cascade file.\n";
            pauseAndReturn();
            return;
        }
    } catch (const cv::Exception& e) {
        std::cerr << "Exception while loading cascade: " << e.what() << std::endl;
        pauseAndReturn();
        return;
    }

    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Failed to load image.\n";
        pauseAndReturn();
        return;
    }

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(gray, faces);

    for (const auto& rect : faces) {
        cv::rectangle(image, rect, cv::Scalar(0, 255, 0), 2);
    }

    cv::imshow("Faces Detected", image);
    cv::imwrite("images/output/faces_detected.jpg", image);
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
            case 3: adjustBrightness(); break;
            case 4: detectEdges(); break;
            case 5: applyMorphology(); break;
            case 6: resizeImage(); break;
            case 7: runBackgroundSubtraction(); break;
            case 8: runGUI(argc, argv); break;
            case 0:
                std::cout << "Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice. Please enter a valid number from the menu.\n";
        }
    }

    return 0;
}
