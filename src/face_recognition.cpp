
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <cascade.xml> <image>" << endl;
        return -1;
    }

    string cascadePath = argv[1];
    string imagePath = argv[2];

    CascadeClassifier faceCascade;
    if (!faceCascade.load(cascadePath)) {
        cerr << "Erreur de chargement du cascade: " << cascadePath << endl;
        return -1;
    }

    Mat image = imread(imagePath);
    if (image.empty()) {
        cerr << "Erreur de lecture de l'image: " << imagePath << endl;
        return -1;
    }

    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);

    vector<Rect> faces;
    faceCascade.detectMultiScale(gray, faces);

    for (const Rect& r : faces) {
        rectangle(image, r, Scalar(0, 255, 0), 2);
    }

    imshow("Visages détectés", image);
    imwrite("faces_detected.jpg", image);
    waitKey(0);
    return 0;
}