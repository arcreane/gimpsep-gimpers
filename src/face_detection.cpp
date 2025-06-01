#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int faceDetectionApp(const string& cascadePath, const string& imagePath) {
    // Chargement du cascade
    CascadeClassifier faceCascade;
    if (!faceCascade.load(cascadePath)) {
        cerr << "Erreur de chargement du cascade: " << cascadePath << endl;
        return -1;
    }

    // Lecture de l’image
    Mat image = imread(imagePath);
    if (image.empty()) {
        cerr << "Erreur de lecture de l'image: " << imagePath << endl;
        return -1;
    }

    // Pré-traitement
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);

    // Détection
    vector<Rect> faces;
    faceCascade.detectMultiScale(gray, faces);

    // Affichage des rectangles
    for (const Rect& r : faces) {
        rectangle(image, r, Scalar(0, 255, 0), 2);
    }

    // Résultats
    imshow("Visages détectés", image);
    imwrite("faces_detected.jpg", image);
    waitKey(0);

    return 0;
}

// Si vous voulez toujours un executable autonome, gardez ce petit wrapper :
int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <cascade.xml> <image>" << endl;
        return -1;
    }
    return faceDetectionApp(argv[1], argv[2]);
}
