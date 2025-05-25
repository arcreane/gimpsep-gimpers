#include "BackgroundSubtractor.h"

void BackgroundSubtractor::processVideo(const std::string& inputPath, const std::string& outputPath) {
    cv::VideoCapture cap(inputPath);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video file " << inputPath << std::endl;
        return;
    }

    cv::Ptr<cv::BackgroundSubtractor> pBackSub = cv::createBackgroundSubtractorMOG2();

    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(cv::CAP_PROP_FPS);

    cv::VideoWriter writer(outputPath,
        cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
        fps,
        cv::Size(frame_width, frame_height),
        false);

    cv::Mat frame, fgMask;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        pBackSub->apply(frame, fgMask);

        cv::threshold(fgMask, fgMask, 128, 255, cv::THRESH_BINARY);

        cv::imshow("Foreground Mask", fgMask);
        writer.write(fgMask);

        if (cv::waitKey(30) >= 0) break;
    }

    cap.release();
    writer.release();
    cv::destroyAllWindows();
}