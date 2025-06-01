#include "FaceRecognitionProcessor.h"
#include <opencv2/face.hpp>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <map>
#include <filesystem>  // C++17

using namespace cv;
using namespace cv::face;
using namespace std;
namespace fs = std::filesystem;

static bool isDirectory(const string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && (st.st_mode & S_IFMT) == S_IFDIR;
}

static vector<string> listFiles(const string& dirPath) {
    vector<string> files;
    DIR* d = opendir(dirPath.c_str());
    if (!d) return files;
    struct dirent* e;
    while ((e = readdir(d)) != nullptr) {
        string name = e->d_name;
        if (name == "." || name == "..") continue;
        string full = dirPath + "/" + name;
        struct stat st;
        if (stat(full.c_str(), &st) == 0 && (st.st_mode & S_IFMT) != S_IFDIR) {
            files.push_back(full);
        }
    }
    closedir(d);
    return files;
}

FaceRecognitionProcessor::RecognitionResult FaceRecognitionProcessor::recognizeFace(const QString& datasetDirQt, const cv::Mat& probeImage) {
    RecognitionResult result;
    result.success = false;

    string datasetDir = datasetDirQt.toStdString();

    vector<Mat> images;
    vector<int> labels;
    map<string, int> labelMap;
    map<int, string> labelNames;
    int nextLabel = 0;

    for (const auto& filePath : listFiles(datasetDir)) {
        string filename = fs::path(filePath).filename().string();             // e.g. 000008.jpg
        string stem = fs::path(filePath).stem().string();                    // e.g. 000008 (no extension)

        int lbl;
        if (labelMap.find(stem) == labelMap.end()) {
            lbl = nextLabel++;
            labelMap[stem] = lbl;
            labelNames[lbl] = stem;
        } else {
            lbl = labelMap[stem];
        }

        Mat img = imread(filePath, IMREAD_GRAYSCALE);
        if (!img.empty()) {
            images.push_back(img);
            labels.push_back(lbl);
        }
    }

    if (images.empty()) {
        cerr << "No training images found." << endl;
        return result;
    }

    Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
    model->train(images, labels);

    Mat probeGray;
    if (probeImage.channels() == 3) {
        cvtColor(probeImage, probeGray, COLOR_BGR2GRAY);
    } else {
        probeGray = probeImage.clone();
    }

    int predicted = -1;
    double confidence = 0.0;
    model->predict(probeGray, predicted, confidence);

    result.subjectName = labelNames[predicted];  // e.g. "000008"
    result.confidence = confidence;
    result.image = probeImage.clone();
    result.success = true;

    return result;
}
