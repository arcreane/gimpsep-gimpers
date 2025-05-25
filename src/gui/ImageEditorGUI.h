//
// Created by Ines Mansour on 25/05/2025.
//
#ifndef IMAGEEDITORGUI_H
#define IMAGEEDITORGUI_H

#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QImage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QFileDialog>
#include <QScrollArea>
#include <QStatusBar>
#include <opencv2/opencv.hpp>

class ImageEditorGUI : public QMainWindow {
    Q_OBJECT

public:
    explicit ImageEditorGUI(QWidget* parent = nullptr);

private slots:
    void loadImage();
    void saveImage();
    void adjustBrightness(int value);
    void detectEdges();

private:
    QLabel* imageLabel;
    QSlider* brightnessSlider;
    QPushButton* saveButton;
    QPushButton* edgeButton;

    cv::Mat originalImage;
    cv::Mat currentImage;

    void updateDisplay(const cv::Mat& image);
    QImage cvMatToQImage(const cv::Mat& mat);
    void updateControls(bool enable);
};

#endif // IMAGEEDITORGUI_H


