#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <fstream>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <cascade.xml> <image>" << endl;
        return -1;
    }

    string cascadePath = argv[1];
    string imagePath   = argv[2];

    // Vérification de l'existence du fichier cascade
    ifstream fs(cascadePath);
    if (!fs.good()) {
        cerr << "Fichier cascade introuvable: " << cascadePath << endl;
        return -1;
    }

    CascadeClassifier faceCascade;
    try {
        if (!faceCascade.load(cascadePath)) {
            cerr << "Erreur de chargement du cascade (format invalide?): " << cascadePath << endl;
            return -1;
        }
    } catch (const cv::Exception& e) {
        cerr << "Exception lors du chargement du cascade: " << e.what() << endl;
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
