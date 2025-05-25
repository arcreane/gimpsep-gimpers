//
// Created by Ines Mansour on 25/05/2025.
//
#ifndef IMAGEEDITORGUI_H
#define IMAGEEDITORGUI_H

#include <QMainWindow>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <opencv2/opencv.hpp>

class ImageEditorGUI : public QMainWindow {
    Q_OBJECT

public:
    explicit ImageEditorGUI(QWidget* parent = nullptr);

private slots:
    void loadImage();
    void saveImage();
    void adjustBrightness(int value);
    void detectEdges(int value);
    void applyMorphology();
    void resizeImage();
    void runBackgroundSubtraction();


private:
    void updateDisplay(const cv::Mat& image);
    QImage cvMatToQImage(const cv::Mat& mat);
    void updateControls(bool enable);

    QLabel* imageLabel;
    QLabel* placeholderText;
    QPushButton* saveButton;
    QPushButton* morphologyButton;
    QPushButton* resizeButton;
    QSlider* brightnessSlider;
    QSlider* cannySlider;
    QPushButton* backgroundButton;


    cv::Mat originalImage;
    cv::Mat currentImage;
};

#endif // IMAGEEDITORGUI_H



