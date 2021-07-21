#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace std;
using namespace cv;

string filename;
Mat image;
string winName = "show";
enum{NOT_SET = 0, IN_PROCESS = 1, SET = 2};
uchar rectState;
Rect rect;
Mat mask;
Mat Mask;
Mat spec_mask;
const Scalar GREEN = Scalar(0,255,0);
Mat bgdModel, fgdModel;

void setRectInMask(){
    rect.x = max(0, rect.x);
    rect.y = max(0, rect.y);
    rect.width = min(rect.width, image.cols-rect.x);
    rect.height = min(rect.height, image.rows-rect.y);

}

static void getBinMask( const Mat& comMask, Mat& binMask ){
    binMask.create( comMask.size(), CV_8UC1 );
    binMask = comMask & 1;
}

void on_mouse( int event, int x, int y, int flags, void* )
{
    Mat res;
    Mat binMask;

    switch( event ){
        case CV_EVENT_LBUTTONDOWN:
            if( rectState == NOT_SET){
                rectState = IN_PROCESS;
                rect = Rect( x, y, 1, 1 );
            }
            break;
        case CV_EVENT_LBUTTONUP:
            if( rectState == IN_PROCESS ){
                rect = Rect( Point(rect.x, rect.y), Point(x,y) );
                rectState = SET;
                (mask(rect)).setTo( Scalar(GC_PR_FGD));
                image = imread( filename, 1);
                grabCut(image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);
                getBinMask( mask, binMask );
                image.copyTo(res, binMask );
                Mat res_gray;
                cvtColor(res,res_gray,cv::COLOR_RGB2GRAY);
                cv::threshold(res_gray,Mask,15,255,cv::THRESH_BINARY);
                imshow("Mask", Mask);
                cv::threshold(res_gray,spec_mask,150,255,cv::THRESH_BINARY);
                imshow("spec_mask", spec_mask);

            }
            break;
        case CV_EVENT_MOUSEMOVE:
            if( rectState == IN_PROCESS ){
                rect = Rect( Point(rect.x, rect.y), Point(x,y) );
                image = imread( filename, 1 );
                rectangle(image, Point( rect.x, rect.y ), Point(rect.x + rect.width, rect.y + rect.height ), GREEN, 2);
                imshow(winName, image);
            }
            break;
    }
}

int main(int argc, char* argv[]){

    string path="/home/wya/Project/NfP_test/data/headset/Mono/1";
    filename =path+ "/000000I0.png";
    image = imread( filename, 1 );
    imshow(winName, image);
    mask.create(image.size(), CV_8UC1);
    rectState = NOT_SET;
    mask.setTo(GC_BGD);

    setMouseCallback(winName, on_mouse, 0);



//     imshow("Spec Mask", spec_mask);
    waitKey(0);
    int32_t count=0;
    for(int i=0; i < Mask.total();i++){
        if(Mask.data[i]>0) count++;
    }
    cout<<"Mask counts= "<< count<<endl;


    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(0);    // 无压缩png.
    compression_params.push_back(IMWRITE_PNG_STRATEGY);
    compression_params.push_back(IMWRITE_PNG_STRATEGY_DEFAULT);

    imwrite(path+"/mask.png",Mask, compression_params);
    imwrite(path+"/spec.png",spec_mask);
    return 0;
}
