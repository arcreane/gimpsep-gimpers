#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " img1 img2 [img3 ...]" << endl;
        return -1;
    }

    vector<Mat> images;
    for (int i = 1; i < argc; ++i) {
        Mat img = imread(argv[i]);
        if (img.empty()) {
            cerr << "Erreur de lecture de l'image: " << argv[i] << endl;
            return -1;
        }
        images.push_back(img);
    }

    Stitcher::Mode mode = Stitcher::PANORAMA;
    Ptr<Stitcher> stitcher = Stitcher::create(mode);
    Mat pano;
    Stitcher::Status status = stitcher->stitch(images, pano);

    if (status != Stitcher::OK) {
        cerr << "Échec du stitching, code erreur = " << int(status) << endl;
        return -1;
    }

    imshow("Panorama", pano);
    imwrite("panorama_result.jpg", pano);
    waitKey(0);
    return 0;
}