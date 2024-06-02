// qr.cpp
#include "qr.h"
#include <iostream>
#include <string>

QRCodeScanner::QRCodeScanner() {}

QRCodeScanner::~QRCodeScanner() {
    releaseCamera();
}

bool QRCodeScanner::initializeCamera() {
    cap.open(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open the camera" << std::endl;
        return false;
    }
    return true;
}

QRCodeResult QRCodeScanner::scanQRCode() {
    QRCodeResult result = {0, 0, false};
    cap >> frame;
    if (frame.empty()) {
        std::cerr << "Error: Unable to capture frame" << std::endl;
        return result;
    }

    std::string data = qrDecoder.detectAndDecode(frame);
    if (data.length() == 2 && isdigit(data[0]) && isdigit(data[1])) {
        result.digit1 = data[0] - '0';
        result.digit2 = data[1] - '0';
        result.success = true;
    }
    return result;
}

void QRCodeScanner::releaseCamera() {
    if (cap.isOpened()) {
        cap.release();
    }
}

// C API functions
extern "C" {

QRCodeScanner* createQRCodeScanner() {
    return new QRCodeScanner();
}

void destroyQRCodeScanner(QRCodeScanner* scanner) {
    delete scanner;
}

bool initializeQRCodeScanner(QRCodeScanner* scanner) {
    return scanner->initializeCamera();
}

QRCodeResult scanQRCode(QRCodeScanner* scanner) {
    return scanner->scanQRCode();
}

void releaseQRCodeScanner(QRCodeScanner* scanner) {
    scanner->releaseCamera();
}

}
