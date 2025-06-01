#include "FaceRecognitionProcessor.h"
#include <opencv2/face.hpp>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <map>

using namespace cv;
using namespace cv::face;
using namespace std;

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

FaceRecognitionProcessor::RecognitionResult FaceRecognitionProcessor::recognizeFace(const QString& datasetDirQt, const QString& probePathQt) {
    RecognitionResult result;
    result.success = false;

    string datasetDir = datasetDirQt.toStdString();
    string probePath  = probePathQt.toStdString();

    vector<Mat> images;
    vector<int> labels;
    map<string, int> labelMap;
    map<int, string> labelNames;
    int nextLabel = 0;

    DIR* root = opendir(datasetDir.c_str());
    if (!root) return result;

    struct dirent* entry;
    while ((entry = readdir(root)) != nullptr) {
        string name = entry->d_name;
        if (name == "." || name == "..") continue;
        string fullPath = datasetDir + "/" + name;

        if (isDirectory(fullPath)) {
            string subject = name;
            int lbl = nextLabel++;
            labelMap[subject] = lbl;
            labelNames[lbl] = subject;

            for (auto& f : listFiles(fullPath)) {
                Mat img = imread(f, IMREAD_GRAYSCALE);
                if (!img.empty()) {
                    images.push_back(img);
                    labels.push_back(lbl);
                }
            }
        } else {
            size_t dot = name.find('.');
            string subject = (dot != string::npos ? name.substr(0, dot) : name);

            int lbl;
            auto it = labelMap.find(subject);
            if (it == labelMap.end()) {
                lbl = nextLabel++;
                labelMap[subject] = lbl;
                labelNames[lbl] = subject;
            } else {
                lbl = it->second;
            }

            Mat img = imread(fullPath, IMREAD_GRAYSCALE);
            if (!img.empty()) {
                images.push_back(img);
                labels.push_back(lbl);
            }
        }
    }
    closedir(root);

    if (images.empty()) return result;

    Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
    model->train(images, labels);

    Mat probe = imread(probePath, IMREAD_GRAYSCALE);
    if (probe.empty()) return result;

    int predicted = -1;
    double confidence = 0.0;
    model->predict(probe, predicted, confidence);

    result.subjectName = labelNames[predicted];
    result.confidence = confidence;
    result.image = probe;
    result.success = true;

    return result;
}
