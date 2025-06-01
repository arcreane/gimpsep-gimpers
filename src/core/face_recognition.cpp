// src/face_recognition.cpp
// Reconnaissance faciale C++11 inspirée du tutoriel OpenCV

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/face.hpp>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace cv;
using namespace cv::face;
using namespace std;

/// vérifie si path est un répertoire
static bool isDirectory(const string &path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && (st.st_mode & S_IFMT) == S_IFDIR;
}

/// liste tous les fichiers simples (non-répertoires) dans dirPath
static vector<string> listFiles(const string &dirPath) {
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

int main(int argc, char** argv)
{
    if (argc != 3) {
        cout << "Usage: " << argv[0]
             << " <dataset_dir> <image_to_recognize>\n";
        return -1;
    }

    string datasetDir = argv[1];
    string probePath   = argv[2];

    // Containers pour images & labels
    vector<Mat> images;
    vector<int> labels;
    map<string,int> labelMap;    // subject -> label int
    map<int,string> labelNames;  // label int -> subject
    int nextLabel = 0;

    // Lecture du dossier racine
    DIR* root = opendir(datasetDir.c_str());
    if (!root) {
        cerr << "Erreur : impossible d'ouvrir « " << datasetDir << " »\n";
        return -1;
    }
    struct dirent* entry;
    while ((entry = readdir(root)) != nullptr) {
        string name = entry->d_name;
        if (name == "." || name == "..") continue;
        string fullPath = datasetDir + "/" + name;

        if (isDirectory(fullPath)) {
            // CAS 1: sous-dossier par personne
            string subject = name;
            int lbl = nextLabel++;
            labelMap[subject]   = lbl;
            labelNames[lbl]     = subject;

            // importer toutes les images de ce sous-dossier
            for (auto& f : listFiles(fullPath)) {
                Mat img = imread(f, IMREAD_GRAYSCALE);
                if (!img.empty()) {
                    images.push_back(img);
                    labels.push_back(lbl);
                }
            }
        }
        else {
            // CAS 2: fichier plat subjectXX.xxx
            // on récupère le préfixe jusqu'au premier '.'
            size_t dot = name.find('.');
            string subject = (dot != string::npos ? name.substr(0, dot) : name);

            int lbl;
            auto it = labelMap.find(subject);
            if (it == labelMap.end()) {
                lbl = nextLabel;
                labelMap[subject]   = lbl;
                labelNames[lbl]     = subject;
                nextLabel++;
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

    if (images.empty()) {
        cerr << "Erreur : aucun fichier image chargé depuis « "
             << datasetDir << " »\n";
        return -1;
    }

    // Entraînement du modèle LBPH
    Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
    model->train(images, labels);
    model->save("face_model.yml");

    // Chargement et prédiction sur l'image cible
    Mat probe = imread(probePath, IMREAD_GRAYSCALE);
    if (probe.empty()) {
        cerr << "Erreur : impossible de charger l'image « "
             << probePath << " »\n";
        return -1;
    }

    int predicted = -1;
    double confidence = 0.0;
    model->predict(probe, predicted, confidence);


    cout << "\n=== Résultat reconnaissance faciale ===\n"
         << "Personne reconnue : " << labelNames[predicted] << "\n"
         << "Confiance (plus bas = meilleure) : " << confidence << "\n\n";


    imshow("Test", probe);
    waitKey(0);

    return 0;
}
