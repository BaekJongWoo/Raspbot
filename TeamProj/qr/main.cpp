#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

int main() {
    
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open the camera" << std::endl;
        return -1;
    }

    cv::QRCodeDetector qrDecoder;
    cv::Mat frame;

    while (true) {
        
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Error: Unable to capture frame" << std::endl;
            break;
        }

        std::string data = qrDecoder.detectAndDecode(frame);
        if (!data.empty()) {
            std::cout << data << std::endl;
        }

        if (cv::waitKey(30) == 'q') {
            break;
        }
    }
 
    cap.release();
    return 0;
}

