#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    VideoCapture cap(0);

    if(cap.isOpened() == false) {
        cout << "Can't open video stream" << endl;
        cin.get();
        return -1;
    }

    namedWindow("CStream");
    while(true) {
        Mat frame;
        bool bSuccess = cap.read(frame);

        if(bSuccess == false) {
            cout << "Failed to get frames" << endl;
            cin.get();
            return -1;
        }

        imshow("CStream", frame);

        if(waitKey(10) == 27) {
            cout << "Esc key pressed" << endl;
            break;
        }
    }

    return 0;
}