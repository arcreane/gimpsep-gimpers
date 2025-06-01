#ifndef STITCHING_H
#define STITCHING_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class StitcherSimple {
public:
    StitcherSimple();
    ~StitcherSimple();

    // Charge les images à partir d'un vecteur de chemins
    bool loadImages(const std::vector<std::string>& imagePaths);

    // Execute le processus de "stitching" simple
    bool createPanorama();

    // Affiche le panorama
    void showPanorama(const std::string& windowName = "Panorama") const;

    // Sauvegarde le panorama dans un fichier
    bool savePanorama(const std::string& outputPath) const;

private:
    // Liste des images originales
    std::vector<cv::Mat> images_;

    // Images redimensionnées à la même hauteur
    std::vector<cv::Mat> resizedImages_;

    // Résultat final
    cv::Mat panorama_;

    // Hauteur de référence
    int refHeight_;

    // Méthodes internes
    void computeReferenceHeight();
    void resizeImages();
    void stitchImages();
};

#endif // STITCHING_H