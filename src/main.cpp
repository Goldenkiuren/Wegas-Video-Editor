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

    VideoWriter video;

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
            //ESC
            break; // stop capturing by pressing ESC
        }
        else if(key == 32 && !video.isOpened())
        {
            //SPACE
            video.open("output.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), cap.get(CAP_PROP_FPS), Size((int)cap.get(CAP_PROP_FRAME_WIDTH), (int)cap.get(CAP_PROP_FRAME_HEIGHT)));
        }
        else if(key == 13)
        {
            //ENTER
            video.release();
        }
        else if(key != -1)
        {
            current_operation = key;
        }

        switch(current_operation)
        {   //Gaussian Blur
            case 'g':
            case 'G':
                GaussianBlur(frame, edited_frame, Size(2 * slider_value + 1,2 * slider_value + 1),0);
                break;
            //Canny
            case 'k':
            case 'K':
                Canny(frame, edited_frame, slider_value * 2.5, slider_value*4);
                break;
            //Sobel
            case 's':
            case 'S':
                Sobel(frame, edited_frame, CV_8U, 1, 1, min(slider_value * 2 + 1, 31));
                break;
            //Brightness
            case 'b':
            case 'B':
                frame.convertTo(edited_frame, -1, 1, slider_value * 4);
                break;
            //Contrast
            case 'c':
            case 'C':
                frame.convertTo(edited_frame, -1, slider_value * 0.1, 0);
                break;
            //Negative
            case 'n':
            case 'N':
                frame.convertTo(edited_frame, -1, -1, 255);
                break;
            //Grayscale
            case 'y':
            case 'Y':
                cvtColor(frame, edited_frame, COLOR_BGR2GRAY);
                break;
            //Resize
            case 'r':
            case 'R':
                resize(frame, edited_frame, Size(), 0.5 + slider_value * 0.01, 0.5 + slider_value * 0.01);
                break;
            //Rotate Left
            case '-':
            case '_':
                rotate(frame, edited_frame, ROTATE_90_COUNTERCLOCKWISE);
                break;
            //Rotate Right
            case '=':
            case '+':
                rotate(frame, edited_frame, ROTATE_90_CLOCKWISE);
                break;
            //Vertical Flip
            case 'v':
            case 'V':
                flip(frame, edited_frame, 0);
                break;
            //Horizontal Flip
            case 'h':
            case 'H':
                flip(frame, edited_frame, 1);
                break;
            default:
                frame.copyTo(edited_frame);
                break;
        }

        
        imshow("Source Live Video", frame);
        imshow("Edited Live Video", edited_frame);
        if(video.isOpened())
        {
            if(current_operation == 'r' || current_operation == 'R' || current_operation == 'y' || current_operation == 'Y' ||
            current_operation == '-' || current_operation == '_' || current_operation == '+' || current_operation == '=')
            {
                video.write(frame);
            }
            else
            {
                video.write(edited_frame);
            }
        }
    }
    cap.release(); // release the VideoCapture object
    destroyAllWindows();
    return 0;
}

static void on_trackbar(int, void*)
{
   slider_value = slider;
}
