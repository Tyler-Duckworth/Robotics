#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    Mat img = imread("/Users/s848387/Desktop/pixLab/images/beach.jpg");
    
    if(img.empty()) {
        cout << "Couldn't find image" << endl;
        cin.get();
        return -1;
    }
    String name = "Beach";
    namedWindow(name);
    imshow(name, img);
    waitKey(0);
    destroyWindow(name);
    return 0;
}