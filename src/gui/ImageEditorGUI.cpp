#include "ImageEditorGUI.h"

#include "../include/BrightnessProcessor.h"
#include "../include/EdgeDetector.h"
#include "../include/MorphologyProcessor.h"
#include "../include/ResizeProcessor.h"
#include "../include/BackgroundSubtractor.h"
#include "../include/FaceDetectionProcessor.h"
#include "../include/FaceRecognitionProcessor.h"
#include "../include/PanoramaStitcher.h"

#include <QFileDialog>
#include <QPixmap>
#include <QScrollArea>
#include <QStatusBar>
#include <QInputDialog>
#include <QMessageBox>

ImageEditorGUI::ImageEditorGUI(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Image Editor - Gimpsep Gimpers");
    resize(1000, 700);

    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    QVBoxLayout* sidePanel = new QVBoxLayout();
    sidePanel->setSpacing(15);

    imageLabel = new QLabel;
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setMinimumSize(600, 400);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(imageLabel);

    placeholderText = new QLabel("Please import your image using the 'Open' button.");
    placeholderText->setAlignment(Qt::AlignCenter);
    placeholderText->setStyleSheet("color: gray; font-size: 16px;");
    imageLabel->setLayout(new QVBoxLayout);
    imageLabel->layout()->addWidget(placeholderText);

    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(sidePanel);
    setCentralWidget(centralWidget);

    QPushButton* loadButton = new QPushButton("Open");
    saveButton = new QPushButton("Save");
    morphologyButton = new QPushButton("Morphology");
    resizeButton = new QPushButton("Resize");
    backgroundButton = new QPushButton("Background Subtraction");
    faceDetectionButton = new QPushButton("Face Detection");
    panoramaButton = new QPushButton("Panorama Stitching");

    QPushButton* faceRecognitionButton = new QPushButton("Face Recognition");



    faceDetectionButton->setEnabled(true);
    panoramaButton->setEnabled(true);
    saveButton->setEnabled(false);
    morphologyButton->setEnabled(false);
    resizeButton->setEnabled(false);

    brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setRange(-100, 100);
    brightnessSlider->setValue(0);
    brightnessSlider->setEnabled(false);
    QLabel* brightnessLabel = new QLabel("Brightness");

    cannySlider = new QSlider(Qt::Horizontal);
    cannySlider->setRange(0, 255);
    cannySlider->setValue(100);
    cannySlider->setEnabled(false);
    QLabel* cannyLabel = new QLabel("Canny Edge");

    sidePanel->addWidget(loadButton);
    sidePanel->addWidget(saveButton);
    sidePanel->addSpacing(20);
    sidePanel->addWidget(brightnessLabel);
    sidePanel->addWidget(brightnessSlider);
    sidePanel->addWidget(cannyLabel);
    sidePanel->addWidget(cannySlider);
    sidePanel->addSpacing(20);
    sidePanel->addWidget(morphologyButton);
    sidePanel->addWidget(resizeButton);
    sidePanel->addWidget(backgroundButton);
    sidePanel->addStretch();
    sidePanel->addWidget(faceDetectionButton);
    sidePanel->addWidget(panoramaButton);
    sidePanel->addWidget(faceRecognitionButton);

    statusBar()->showMessage("Ready");

    connect(loadButton, &QPushButton::clicked, this, &ImageEditorGUI::loadImage);
    connect(saveButton, &QPushButton::clicked, this, &ImageEditorGUI::saveImage);
    connect(brightnessSlider, &QSlider::valueChanged, this, &ImageEditorGUI::adjustBrightness);
    connect(cannySlider, &QSlider::valueChanged, this, &ImageEditorGUI::detectEdges);
    connect(morphologyButton, &QPushButton::clicked, this, &ImageEditorGUI::applyMorphology);
    connect(resizeButton, &QPushButton::clicked, this, &ImageEditorGUI::resizeImage);
    connect(backgroundButton, &QPushButton::clicked, this, &ImageEditorGUI::runBackgroundSubtraction);
    connect(faceDetectionButton, &QPushButton::clicked, this, &ImageEditorGUI::runFaceDetection);
    connect(panoramaButton, &QPushButton::clicked, this, &ImageEditorGUI::runPanoramaStitching);
    connect(faceRecognitionButton, &QPushButton::clicked, this, &ImageEditorGUI::runFaceRecognition);

}

void ImageEditorGUI::loadImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (fileName.isEmpty()) return;

    originalImage = cv::imread(fileName.toStdString());
    if (originalImage.empty()) {
        statusBar()->showMessage("Failed to load image");
        return;
    }

    currentImage = originalImage.clone();
    brightnessSlider->setValue(0);
    cannySlider->setValue(100);
    updateDisplay(currentImage);
    updateControls(true);
    placeholderText->hide();
    statusBar()->showMessage("Image loaded");
}

void ImageEditorGUI::saveImage() {
    if (currentImage.empty()) return;

    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg)");
    if (!fileName.isEmpty()) {
        cv::imwrite(fileName.toStdString(), currentImage);
        statusBar()->showMessage("Image saved: " + fileName);
    }
}

void ImageEditorGUI::adjustBrightness(int value) {
    if (originalImage.empty()) return;
    currentImage = BrightnessProcessor::adjustBrightness(originalImage, value);
    updateDisplay(currentImage);
}

void ImageEditorGUI::detectEdges(int threshold) {
    if (originalImage.empty()) return;
    cv::Mat result = EdgeDetector::detectEdges(originalImage, threshold, threshold * 3);
    currentImage = result.clone();
    updateDisplay(result);
    statusBar()->showMessage("Canny edge detection applied");
}

void ImageEditorGUI::applyMorphology() {
    if (currentImage.empty()) return;

    bool ok;
    int op = QInputDialog::getInt(this, "Morphology", "Operation (1: Dilation, 2: Erosion):", 1, 1, 2, 1, &ok);
    if (!ok) return;

    int size = QInputDialog::getInt(this, "Kernel Size", "Enter odd kernel size (e.g., 3, 5, 7):", 3, 1, 99, 2, &ok);
    if (!ok) return;

    cv::Mat result = (op == 1)
        ? MorphologyProcessor::applyDilation(currentImage, size)
        : MorphologyProcessor::applyErosion(currentImage, size);

    currentImage = result;
    updateDisplay(result);
    statusBar()->showMessage("Morphological operation applied");
}

void ImageEditorGUI::resizeImage() {
    if (currentImage.empty()) return;

    bool ok;
    int width = QInputDialog::getInt(this, "Resize", "New width:", currentImage.cols, 1, 10000, 1, &ok);
    if (!ok) return;
    int height = QInputDialog::getInt(this, "Resize", "New height:", currentImage.rows, 1, 10000, 1, &ok);
    if (!ok) return;

    cv::Mat result = ResizeProcessor::resizeImage(currentImage, width, height);
    currentImage = result;
    updateDisplay(result);
    statusBar()->showMessage("Image resized");
}

void ImageEditorGUI::runBackgroundSubtraction() {
    QString inputPath = QFileDialog::getOpenFileName(this, "Open Video", "", "Videos (*.mp4 *.avi *.mov)");
    if (inputPath.isEmpty()) return;

    QString outputPath = QFileDialog::getSaveFileName(this, "Save Output Video", "", "AVI Video (*.avi)");
    if (outputPath.isEmpty()) return;

    BackgroundSubtractor::processVideo(inputPath.toStdString(), outputPath.toStdString());
    statusBar()->showMessage("Background subtraction completed. Output saved.");
}

void ImageEditorGUI::runFaceDetection() {
    QString cascadePath = QFileDialog::getOpenFileName(this, "Select Haar Cascade XML", "", "XML Files (*.xml)");
    if (cascadePath.isEmpty()) return;

    QString imagePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (imagePath.isEmpty()) return;

    bool success = false;
    cv::Mat result = FaceDetectionProcessor::detectFaces(cascadePath, imagePath, success);
    if (!success) {
        QMessageBox::warning(this, "Error", "Face detection failed.");
        return;
    }

    currentImage = result;
    updateDisplay(currentImage);
    statusBar()->showMessage("Face detection completed.");
}
void ImageEditorGUI::runFaceRecognition() {
    QString datasetDir = QFileDialog::getExistingDirectory(this, "Select Dataset Directory");
    if (datasetDir.isEmpty()) return;

    QString probePath = QFileDialog::getOpenFileName(this, "Select Image to Recognize", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (probePath.isEmpty()) return;

    auto result = FaceRecognitionProcessor::recognizeFace(datasetDir, probePath);
    if (!result.success) {
        QMessageBox::warning(this, "Error", "Face recognition failed.");
        return;
    }

    currentImage = result.image;
    updateDisplay(currentImage);

    QString message = QString("Recognized: %1\nConfidence: %2")
                      .arg(QString::fromStdString(result.subjectName))
                      .arg(result.confidence);
    QMessageBox::information(this, "Recognition Result", message);
    statusBar()->showMessage("Face recognition completed.");
}


void ImageEditorGUI::runPanoramaStitching() {
    bool ok;
    int numImages = QInputDialog::getInt(this, "Panorama", "Number of images:", 2, 2, 10, 1, &ok);
    if (!ok) return;

    std::vector<std::string> paths;
    for (int i = 0; i < numImages; ++i) {
        QString path = QFileDialog::getOpenFileName(this, QString("Select Image %1").arg(i + 1), "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (path.isEmpty()) return;
        paths.push_back(path.toStdString());
    }

    PanoramaStitcher stitcher;
    if (!stitcher.loadImages(paths) || !stitcher.createPanorama()) {
        QMessageBox::warning(this, "Error", "Panorama stitching failed.");
        return;
    }

    currentImage = stitcher.getPanorama().clone();
    updateDisplay(currentImage);
    statusBar()->showMessage("Panorama created.");
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

void ImageEditorGUI::updateControls(bool enable) {
    saveButton->setEnabled(enable);
    brightnessSlider->setEnabled(enable);
    cannySlider->setEnabled(enable);
    morphologyButton->setEnabled(enable);
    resizeButton->setEnabled(enable);
}
