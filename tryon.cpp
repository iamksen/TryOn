#include <iostream>
#include <dlib/dnn.h> /* shape_predictor */
#include <dlib/data_io.h> /* load_image */
#include <dlib/opencv.h> /* cv_image */
#include <dlib/gui_widgets.h> /* image_window, ... */
#include <dlib/image_processing/frontal_face_detector.h>
#include <opencv2/highgui/highgui.hpp> /* for VideoCapture */
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace dlib;

std::vector<cv::Point2f> left_image;  // frame coords
std::vector<cv::Point2f> right_image; // glass coords

void overlayImage(const cv::Mat &background, const cv::Mat &foreground, cv::Mat &output, cv::Point2i location)
{
    background.copyTo(output);

    // start at the row indicated by location, or at row 0 if location.y is negative.
    for(int y = std::max(location.y , 0); y < background.rows; ++y){
        int fY = y - location.y; // because of the translation

        // we are done of we have processed all rows of the foreground image.
        if(fY >= foreground.rows)
            break;

        // start at the column indicated by location,

        // or at column 0 if location.x is negative.
        for(int x = std::max(location.x, 0); x < background.cols; ++x){
            int fX = x - location.x; // because of the translation.

            // we are done with this row if the column is outside of the foreground image.
            if(fX >= foreground.cols)
                break;

            // determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
            double opacity = 
                ((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3])
                / 255.;


            // and now combine the background and foreground pixel, using the opacity,

            // but only if opacity > 0.
            for(int c = 0; opacity > 0 && c < output.channels(); ++c){
                unsigned char foregroundPx =
                    foreground.data[fY * foreground.step + fX * foreground.channels() + c];
                unsigned char backgroundPx =
                    background.data[y * background.step + x * background.channels() + c];
                output.data[y*output.step + output.channels()*x + c] =
                    backgroundPx * (1.-opacity) + foregroundPx * opacity;
            }
        }
    }
}


int main(int argc, char** argv)
{
    try {
        cv::VideoCapture cap(0);
        if (!cap.isOpened()) {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }

        frontal_face_detector detector = get_frontal_face_detector();
        /*
           based on 5 HOG filters, trained on front face, total leftface and rightface, partial left and rightface
           block size of 80x80 is taken to take samples for HOG.
           this generates a frontal and sideways face detector.
        */


        shape_predictor sp2;
        /*
           takes in an image of face and outputs a set of points that define the pose of the object .
           trained by shape_predictor_trainer using the one millisecond face alignment algo.
        */

        matrix<rgb_alpha_pixel> glasses;
        /*
        rgb_alpha_pixel: __alpha compositing__ is the process of combining an image with a background to create
        the appearance of partial or full transparency.
        additional data is stored in the alpha channel with a value between 0 and 1. A value of 0 means that the
        pixel does not have any coverage information and is transparent; i.e. there was no color contribution from
        any geometry because the geometry did not overlap this pixel. A value of 1 means that the pixel is opaque
        because the geometry completely overlapped the pixel.
        For example, given two image elements A and B, the most common compositing operation is to combine the images
        such that A appears in the foreground and B appears in the background.

        since the glasses have gone through photoshop, which does the alpha transparency of the background automatically
        with the checking of the transparency checkbox while saving the image, neglecting the background.

        glasses stores the pixel values of a particular frame selected
        */

        deserialize("shape_predictor_68_face_landmarks.dat") >> sp2;
        /*
           training data for the shape predictor
        */

        cv::Mat gll = cv::imread("../frame/frame1.png", -1 );

        load_image(glasses, "../frame/frame1.png");
        /*
           stores images of all png glasses frame in glasses matrix and alpha composites them
        */

        image_window win_with_mesh, win_with_frame, win;
        win.set_title("Webcame output");
        win_with_frame.set_title("Output with frame");
        win_with_mesh.set_title("Output with mesh");
        /*image window for normal, with mesh, with frame with titles for the respective windows*/

        /*cap captures the image from webcam and stores it in a matrix form of name temp*/
        dlib::array2d<rgb_pixel> cimg;

        //you can comment out the below line
        array2d<rgb_pixel> cam_image;
        /*store cam image that is obtained in a 2d vector which has only rgb channels and no alpha channel*/

        cv::Mat temp;
        cv::Mat finalImage;
        cv::Mat result;
        const rgb_pixel color(255,0,0); //red color
        float l1, l2, l3, deltal1, deltal2, deltal3, X, Y;
        while(!win.is_closed()) {
            // Grab a frame from webcam
            cap >> temp;
            result = temp;
            dlib::assign_image(cimg, dlib::cv_image<bgr_pixel>(temp));
            /*
            cv_image: a wrapper class which binds itself over a Mat object, converts opencv images to dlib images
            here it converts the opencv type image temp to dlib <bgr_pixel> type image
            and assign_image converts the above to the suitable form and copies it to another image 'cimg'
            bgr simply lays down in memory in the order B G R rather tan R G B in rgb.. images in opencv are stored in bgr format
            */

            // Detect faces
            std::vector<rectangle> dets = detector(cimg);
            /*
               vector of faces detected
            */

            win_with_mesh.clear_overlay();
            win_with_frame.clear_overlay();
            win_with_mesh.set_image(cimg);

            // Display it all on the screen
            //win.clear_overlay();
            win.set_image(cimg);


            std::vector<image_window::overlay_line> lines;
            /*lines stores the lines used to draw the mesh; basically a vector of lines*/

            for(int i = 0 ; i < (int)dets.size() ; i++) {
                auto shape = sp2(cimg, dets[i]);
                /*
                   for this face dets[i] find the pose of the face and stores the shape in shape variable
                   in the form of points that define the pose of the face
                   this shape is a vector of 68 points(pairs) defining the pose of the face
                */

                auto top_left  = shape.part(17);
                auto top_right  = shape.part(26);
                auto mid  = shape.part(27);
                auto left_eye  = shape.part(36);
                auto right_eye  = shape.part(45);
                auto pt31 = shape.part(31);
                auto pt35 = shape.part(35);
                auto pt29 = shape.part(29);
                auto pt0 = shape.part(0);
                auto pt16 = shape.part(16);

                left_image.clear();
                right_image.clear();

                // points on the face in anticlockwise direction
                right_image.push_back(cv::Point2f(top_left.x(), top_left.y()));
                right_image.push_back(cv::Point2f(left_eye.x(), left_eye.y()));
                right_image.push_back(cv::Point2f((left_eye.x()+pt31.x())/2, (left_eye.y()+pt31.y())/2));
                right_image.push_back(cv::Point2f(mid.x(), mid.y()));
                right_image.push_back(cv::Point2f((right_eye.x()+pt35.x())/2, (right_eye.y()+pt35.y())/2));
                right_image.push_back(cv::Point2f(right_eye.x(), right_eye.y()));
                right_image.push_back(cv::Point2f(top_right.x(), top_right.y()));
                
                // points on the glass in anticlockwise direction
                left_image.push_back(cv::Point2f(0,11)); //top left
                left_image.push_back(cv::Point2f(44, 60)); //left eye
                left_image.push_back(cv::Point2f(88, 110)); //middle left
                left_image.push_back(cv::Point2f(175,20)); //middle point
                left_image.push_back(cv::Point2f(265, 110)); //middle right
                left_image.push_back(cv::Point2f(300,66)); //right eye
                left_image.push_back(cv::Point2f(349,11)); //top right

                cv::Mat H = findHomography(left_image, right_image, 0);
                cv::Mat glassWarped;

                warpPerspective(gll,glassWarped,H, temp.size() );

                overlayImage(temp, glassWarped, result, cv::Point(0,0));
                
                //Drawing the mesh
                lines.push_back(image_window::overlay_line(top_left, pt0, color));
                lines.push_back(image_window::overlay_line(top_right, pt16, color));
            }

            dlib::assign_image(cimg, dlib::cv_image<bgr_pixel>(temp));
            win_with_frame.add_overlay(lines);
            win_with_frame.set_image(cimg);
        }
    }
    catch(std::exception& e) {
        cout << e.what() << endl;
    }
    return 0;
}