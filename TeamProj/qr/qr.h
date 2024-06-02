#ifndef QR_H
#define QR_H

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <string>

extern "C" {
    struct QRCodeResult {
        int digit1;
        int digit2;
        bool success;
    };

    class QRCodeScanner {
    public:
        QRCodeScanner();
        ~QRCodeScanner();
        
        bool initializeCamera();
        QRCodeResult scanQRCode();
        void releaseCamera();

    private:
        cv::VideoCapture cap;
        cv::Mat frame;
        cv::QRCodeDetector qrDecoder;
    };

    QRCodeScanner* createQRCodeScanner();
    void destroyQRCodeScanner(QRCodeScanner* scanner);
    bool initializeQRCodeScanner(QRCodeScanner* scanner);
    QRCodeResult scanQRCode(QRCodeScanner* scanner);
    void releaseQRCodeScanner(QRCodeScanner* scanner);
}

#endif // QR_H
