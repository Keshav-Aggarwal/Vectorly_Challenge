
#include <algorithm>
#include <vector>
#include <map>
#include <iterator>
#include <string>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int trackBarPos = 0;
int g_run = 1, g_dontset = 0;
cv::VideoCapture g_cap;

class FuncContainer{
    private:
        cv::Mat image;
        cv::Mat tempImage;
        int morph_size = 9; 
    public:
        FuncContainer(string filePath){
            image = cv::imread(filePath);
            if(image.empty()){
                cout << "Image doesn't exists" << endl;
                exit(1);
            }
            tempImage = image.clone();
        }

        FuncContainer& pyrDown(){
            cv::pyrDown(tempImage, tempImage);
            return *this;
        }

        void FilterText(){
            // Convert the image to HSV color space as check the black color is easies in the HSV space with only V value.
            cv::cvtColor(tempImage, tempImage, cv::COLOR_BGR2HSV);

            // Then Filter the color values between (0,0,0) and (255,255,30) in HSV color space. 
            // This gives a very good approximation of the black color and filter the image.
            cv::inRange(tempImage, cv::Scalar(0, 0, 0), cv::Scalar(255, 2255,30), tempImage);

            // Create a Kernel for the morphological operation. The size is 7*7 with Rectangular shape
            Mat element = cv::getStructuringElement(cv::MORPH_RECT, Size( morph_size, morph_size));

            //Perform the Morphological operation Opening to remove the noise. Opening is done by applying erosion followed by dilation. 
            //Erosion removes the small patches and then dilation helps in filling the patches or noise present. Only erosion can also remove the noise.
            cv::morphologyEx( tempImage, tempImage, cv::MORPH_OPEN, element );

            //Invert the image as background is required in white color.
            cv::bitwise_not(tempImage, tempImage);
            
            //optional call to scale the image down
            //pyrDown();

            //Visualize the result
            cv::namedWindow("Extracted_text", cv::WINDOW_NORMAL);
            cv::imshow("Extracted_text", tempImage);
            cv::waitKey(0);
        }
};

int main(int argc, char** argv){
    if(argc == 2){
        FuncContainer objFunc = FuncContainer(string(argv[1]));
        objFunc.FilterText();
    }
        
    cv::destroyAllWindows();
    return 0;
}