//
// Created by Ines Mansour on 25/05/2025.
//
#include "ImageEditorGUI.h"
#include "../include/BrightnessProcessor.h"

#include "../include/EdgeDetector.h"

#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QDebug>

ImageEditorGUI::ImageEditorGUI(QWidget* parent) : QMainWindow(parent) {
    // Main container
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Image display
    imageLabel = new QLabel("No image loaded");
    imageLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(imageLabel);

    // Buttons
    QPushButton* loadButton = new QPushButton("Open Image");
    QPushButton* saveButton = new QPushButton("Save Image");
    QPushButton* edgeButton = new QPushButton("Canny Edge Detection");

    brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setMinimum(-100);
    brightnessSlider->setMaximum(100);
    brightnessSlider->setValue(0);

    // Button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(new QLabel("Brightness:"));
    buttonLayout->addWidget(brightnessSlider);
    buttonLayout->addWidget(edgeButton);
    mainLayout->addLayout(buttonLayout);

    setCentralWidget(centralWidget);

    // Connections
    connect(loadButton, &QPushButton::clicked, this, &ImageEditorGUI::loadImage);
    connect(saveButton, &QPushButton::clicked, this, &ImageEditorGUI::saveImage);
    connect(brightnessSlider, &QSlider::valueChanged, this, &ImageEditorGUI::adjustBrightness);
    connect(edgeButton, &QPushButton::clicked, this, &ImageEditorGUI::detectEdges);
}

void ImageEditorGUI::loadImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.bmp)");
    if (fileName.isEmpty()) return;

    originalImage = cv::imread(fileName.toStdString());
    if (originalImage.empty()) {
        qWarning() << "Failed to load image!";
        return;
    }

    currentImage = originalImage.clone();
    updateDisplay(currentImage);
}

void ImageEditorGUI::saveImage() {
    if (currentImage.empty()) return;

    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg)");
    if (!fileName.isEmpty()) {
        cv::imwrite(fileName.toStdString(), currentImage);
    }
}

void ImageEditorGUI::adjustBrightness() {
    if (originalImage.empty()) return;

    double brightnessValue = brightnessSlider->value();
    currentImage = BrightnessProcessor::adjustBrightness(originalImage, brightnessValue);
    updateDisplay(currentImage);
}

void ImageEditorGUI::detectEdges() {
    if (currentImage.empty()) return;

    cv::Mat edges = EdgeDetector::detectEdges(currentImage, 50, 150);
    updateDisplay(edges);
    currentImage = edges;
}

void ImageEditorGUI::updateDisplay(const cv::Mat& image) {
    QImage qimg = cvMatToQImage(image);
    imageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(
        imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

QImage ImageEditorGUI::cvMatToQImage(const cv::Mat& mat) {
    if (mat.channels() == 3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
    } else if (mat.channels() == 1) {
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8).copy();
    } else {
        return QImage();
    }
}
