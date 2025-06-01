#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " img1 img2 [img3 ...]" << endl;
        return -1;
    }

    // Lire toutes les images et vérifier qu'elles sont valides
    vector<Mat> images;
    for (int i = 1; i < argc; ++i) {
        Mat img = imread(argv[i]);
        if (img.empty()) {
            cerr << "Erreur de lecture de l'image: " << argv[i] << endl;
            return -1;
        }
        images.push_back(img);
    }

    // Déterminer la hauteur de référence : on prend la plus petite hauteur pour éviter de grossir
    int minHeight = images[0].rows;
    for (size_t i = 1; i < images.size(); ++i) {
        minHeight = min(minHeight, images[i].rows);
    }

    // Redimensionner chaque image pour qu'elle ait la même hauteur (minHeight),
    // tout en conservant le ratio largeur/hauteur
    vector<Mat> resized;
    resized.reserve(images.size());
    for (const Mat& img : images) {
        if (img.rows != minHeight) {
            double scale = static_cast<double>(minHeight) / img.rows;
            int newWidth = static_cast<int>(img.cols * scale);
            Mat tmp;
            resize(img, tmp, Size(newWidth, minHeight));
            resized.push_back(tmp);
        } else {
            resized.push_back(img);
        }
    }

    // Calculer la largeur totale du panorama
    int totalWidth = 0;
    for (const Mat& img : resized) {
        totalWidth += img.cols;
    }

    // Créer une image vide (noire) pour y coller toutes les images côte à côte
    Mat panorama(minHeight, totalWidth, resized[0].type(), Scalar::all(0));

    // Copier chaque image dans le panorama à la bonne position
    int offsetX = 0;
    for (const Mat& img : resized) {
        Mat roi = panorama(Rect(offsetX, 0, img.cols, img.rows));
        img.copyTo(roi);
        offsetX += img.cols;
    }

    // Afficher et sauvegarder le résultat
    imshow("Panorama brut", panorama);
    imwrite("panorama_simple.jpg", panorama);

    waitKey(0);
    return 0;
}