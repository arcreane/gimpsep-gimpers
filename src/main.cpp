#include <iostream>
#include <string>
#include <limits>
#include <opencv2/opencv.hpp>
#include "BrightnessProcessor.h"
#include "EdgeDetector.h"

using namespace std;

// === MENU PRINTING ===
void printMenu() {
    cout << "\n===== Image Editor CLI =====\n"
         << "1. Panorama (external)\n"
         << "2. Face Detection (external)\n"
         << "3. Face Recognition (external)\n"
         << "4. Adjust Brightness\n"
         << "5. Canny Edge Detection\n"
         << "0. Exit\n"
         << "Choice: ";
}

// === MENU RETURN + WINDOW CLEANUP ===
void pauseAndReturn() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nPress Enter to return to the menu...";
    cin.get();
    cv::destroyAllWindows();
}

// === OPTION 1: PANORAMA ===
void runPanorama() {
    int n;
    cout << "How many images for the panorama? ";
    cin >> n;
    if (!cin || n < 2) {
        cout << "You must enter at least 2 images." << endl;
        pauseAndReturn();
        return;
    }
    vector<string> paths(n);
    for (int i = 0; i < n; ++i) {
        cout << "Path to image " << (i + 1) << ": ";
        cin >> paths[i];
    }
    string cmd = "./panorama";
    for (auto& p : paths) {
        cmd += " " + p;
    }
    system(cmd.c_str());
    pauseAndReturn();
}

// === OPTION 2: FACE DETECTION ===
void runFaceDetection() {
    string cascadePath, imagePath;
    cout << "Path to cascade XML: ";
    cin >> cascadePath;
    cout << "Path to image: ";
    cin >> imagePath;
    string cmd = "./face_detection "
               + cascadePath + " "
               + imagePath
               + " 2> err_detect.log";
    int status = system(cmd.c_str());
    cout << "Return code: " << status
         << ". See err_detect.log for details.\n";
    pauseAndReturn();
}

// === OPTION 3: FACE RECOGNITION ===
void runFaceRecognition() {
    string datasetDir, imagePath;
    cout << "Path to dataset directory: ";
    cin >> datasetDir;
    cout << "Path to image to recognize: ";
    cin >> imagePath;

    // On appelle directement et laissons stdout/stderr s'afficher dans la console
    string cmd = "./face_recognition " + datasetDir + " " + imagePath;
    int status = system(cmd.c_str());
    cout << "Return code: " << status << endl;
    pauseAndReturn();
}

// === OPTION 4: BRIGHTNESS ADJUSTMENT ===
void adjustBrightness() {
    string imagePath;
    double brightnessFactor;

    cout << "Path to image: ";
    cin >> imagePath;
    cout << "Brightness factor (-100 to 100): ";
    cin >> brightnessFactor;

    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        cerr << "Error: could not load image.\n";
        pauseAndReturn();
        return;
    }

    cv::Mat result = BrightnessProcessor::adjustBrightness(image, brightnessFactor);
    cv::namedWindow("Brightened Image", cv::WINDOW_NORMAL);
    cv::imshow("Brightened Image", result);
    cv::imwrite("images/output/brightness.jpg", result);
    cout << "Press any key on the image window to continue...\n";
    cv::waitKey(0);
    pauseAndReturn();
}

// === OPTION 5: CANNY EDGE DETECTION ===
void detectEdges() {
    string imagePath;
    double lowerThreshold, upperThreshold;

    cout << "Path to image: ";
    cin >> imagePath;
    cout << "Lower threshold (e.g., 50): ";
    cin >> lowerThreshold;
    cout << "Upper threshold (e.g., 150): ";
    cin >> upperThreshold;

    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        cerr << "Error: could not load image.\n";
        pauseAndReturn();
        return;
    }

    cv::Mat edges = EdgeDetector::detectEdges(image, lowerThreshold, upperThreshold);
    cv::namedWindow("Canny Edges", cv::WINDOW_NORMAL);
    cv::imshow("Canny Edges", edges);
    cv::imwrite("images/output/edges.jpg", edges);
    cout << "Press any key on the image window to continue...\n";
    cv::waitKey(0);
    pauseAndReturn();
}

// === MAIN LOOP ===
int main() {
    int choice = -1;
    while (true) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printMenu();
        cin >> choice;
        if (!cin) break;

        switch (choice) {
            case 1: runPanorama();      break;
            case 2: runFaceDetection(); break;
            case 3: runFaceRecognition(); break;
            case 4: adjustBrightness(); break;
            case 5: detectEdges();      break;
            case 0:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please enter a valid number.\n";
        }
    }
    return 0;
}
