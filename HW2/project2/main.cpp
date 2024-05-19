#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

int main() {
    // Open the default camera
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open the camera" << std::endl;
        return -1;
    }

    cv::QRCodeDetector qrDecoder;
    cv::Mat frame;

    while (true) {
        // Capture frame-by-frame
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Error: Unable to capture frame" << std::endl;
            break;
        }

        // Detect and decode the QR code
        std::string data = qrDecoder.detectAndDecode(frame);
        if (!data.empty()) {
            std::cout << "Decoded Data: " << data << std::endl;
        }

        // Press 'q' to exit the loop
        if (cv::waitKey(30) == 'q') {
            break;
        }
    }

    // When everything done, release the video capture object
    cap.release();
    return 0;
}

