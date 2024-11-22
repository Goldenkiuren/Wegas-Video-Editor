#include <opencv2/opencv.hpp>
using namespace cv;

const int slider_max = 50;
int slider = 0;
int slider_value = 0;
int current_operation = -1;
static void on_trackbar(int, void*);

int main(int argc, char** argv)
{
    int camera = 0;
    VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    if(!cap.open(camera))
    {
        return 0;
    }

    namedWindow("Source Live Video", WINDOW_AUTOSIZE);
    namedWindow("Edited Live Video", WINDOW_AUTOSIZE);
    createTrackbar("Slider", "Edited Live Video", &slider, slider_max, on_trackbar);

    for(;;)
    {
        Mat frame, edited_frame;
        cap >> frame;
        if(frame.empty())
        {
            break; // end of video stream
        }
    
        char key = (char)waitKey(1);
        if(key == 27)
        {
            break; // stop capturing by pressing ESC
        }
        else if(key != -1)
        {
            current_operation = key;
        }
            

        int kernel_size;
        switch(current_operation)
        {   //Gaussian Blur
            case 'g':
            case 'G':
                kernel_size = 2 * slider_value + 1;
                GaussianBlur(frame, edited_frame, Size(kernel_size,kernel_size),0);
                break;
            //Canny
            case 'c':
            case 'C':
                Canny(frame, edited_frame, slider_value * 2.5, slider_value*4);
                break;
            default:
                edited_frame = frame;
                break;
        }

        
        imshow("Source Live Video", frame);
        imshow("Edited Live Video", edited_frame);
    }
    cap.release(); // release the VideoCapture object
    destroyAllWindows();
    return 0;
}

static void on_trackbar(int, void*)
{
   slider_value = slider;
}
