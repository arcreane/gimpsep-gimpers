#include <iostream>
#include <string>
#include <limits>
#include <opencv2/opencv.hpp>

#include "../include/BrightnessProcessor.h"
#include "../include/EdgeDetector.h"

#include <QApplication>
#include "../gui/ImageEditorGUI.h"

// === MENU PRINTING ===
void printMenu() {
    std::cout << "\n===== Image Editor CLI =====\n";
    std::cout << "1. Panorama (external)\n";
    std::cout << "2. Face Detection (external)\n";
    std::cout << "3. Adjust Brightness\n";
    std::cout << "4. Canny Edge Detection\n";
    std::cout << "5. Launch GUI\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

// === MENU RETURN + WINDOW CLEANUP ===
void pauseAndReturn() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "\nPress Enter to return to the menu...";
    std::cin.get();
    cv::destroyAllWindows();
}

// === OPTION 1: PANORAMA ===
void runPanorama() {
    std::string img1, img2;
    std::cout << "Path to image 1: ";
    std::cin >> img1;
    std::cout << "Path to image 2: ";
    std::cin >> img2;

    std::string command = "./panorama " + img1 + " " + img2;
    system(command.c_str());
    pauseAndReturn();
}

// === OPTION 2: FACE DETECTION ===
void runFaceDetection() {
    std::string cascadePath, imagePath;
    std::cout << "Path to cascade XML: ";
    std::cin >> cascadePath;
    std::cout << "Path to image: ";
    std::cin >> imagePath;

    std::string command = "./face_recognition " + cascadePath + " " + imagePath;
    system(command.c_str());
    pauseAndReturn();
}

// === OPTION 3: BRIGHTNESS ADJUSTMENT ===
void adjustBrightness() {
    std::string imagePath;
    double brightnessFactor;

    std::cout << "Path to image: ";
    std::cin >> imagePath;
    std::cout << "Brightness factor (-100 to 100): ";
    std::cin >> brightnessFactor;

    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Error: could not load image.\n";
        pauseAndReturn();
        return;
    }

    cv::Mat result = BrightnessProcessor::adjustBrightness(image, brightnessFactor);

    cv::namedWindow("Brightened Image", cv::WINDOW_NORMAL); // allow resizing
    cv::imshow("Brightened Image", result);
    cv::imwrite("images/output/brightness.jpg", result);
    std::cout << "Press any key on the image window to continue...\n";
    cv::waitKey(0);
    pauseAndReturn();
}

// === OPTION 4: CANNY EDGE DETECTION ===
void detectEdges() {
    std::string imagePath;
    double lowerThreshold, upperThreshold;

    std::cout << "Path to image: ";
    std::cin >> imagePath;
    std::cout << "Lower threshold (e.g., 50): ";
    std::cin >> lowerThreshold;
    std::cout << "Upper threshold (e.g., 150): ";
    std::cin >> upperThreshold;

    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Error: could not load image.\n";
        pauseAndReturn();
        return;
    }

    cv::Mat edges = EdgeDetector::detectEdges(image, lowerThreshold, upperThreshold);

    cv::namedWindow("Canny Edges", cv::WINDOW_NORMAL); // allow resizing
    cv::imshow("Canny Edges", edges);
    cv::imwrite("images/output/edges.jpg", edges);
    std::cout << "Press any key on the image window to continue...\n";
    cv::waitKey(0);
    pauseAndReturn();
}

// === GUI RUNNER (OPTION 5) ===
void runGUI(int argc, char* argv[]) {
    QApplication app(argc, argv);
    ImageEditorGUI window;
    window.resize(800, 600);
    window.show();
    app.exec();
}

// === MAIN LOOP ===
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
            case 1:
                runPanorama();
                break;
            case 2:
                runFaceDetection();
                break;
            case 3:
                adjustBrightness();
                break;
            case 4:
                detectEdges();
                break;
            case 5:
                runGUI(argc, argv);
                break;
            case 0:
                std::cout << "Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice. Please enter a valid number from the menu.\n";
        }
    }

    return 0;
}
