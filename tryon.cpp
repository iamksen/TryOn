#include <iostream>
#include <dlib/dnn.h> /* shape_predictor */
#include <dlib/data_io.h> /* load_image */
#include <dlib/opencv.h> /* cv_image */
#include <dlib/gui_widgets.h> /* image_window, ... */
#include <dlib/image_processing/frontal_face_detector.h>
#include <opencv2/highgui/highgui.hpp> /* for VideoCapture */
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <string>

using namespace std;
using namespace dlib;

std::vector<cv::Point2f> source_image;  // frame coords
std::vector<cv::Point2f> destination_image; // glass coords

void overlayImage(const cv::Mat &background, const cv::Mat &foreground, cv::Mat &output)
{
    background.copyTo(output);

    // start at the row indicated by location, or at row 0 if location.y is negative.
    for(int y = 0; y < background.rows; ++y){
        
        // we are done of we have processed all rows of the foreground image.
        if(y >= foreground.rows)
            break;

        // start at the column indicated by location,
        // or at column 0 if location.x is negative.
        for(int x = 0; x < background.cols; ++x){
            
            // we are done with this row if the column is outside of the foreground image.
            if(x >= foreground.cols)
                break;

            // determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
            double opacity = ((double)foreground.data[y * foreground.step + x * foreground.channels() + 3]) / 255.;


            // and now combine the background and foreground pixel, using the opacity,
            // but only if opacity > 0.
            for(int c = 0; opacity > 0 && c < output.channels(); ++c){
                unsigned char foregroundPx =
                    foreground.data[y * foreground.step + x * foreground.channels() + c];
                unsigned char backgroundPx =
                    background.data[y * background.step + x * background.channels() + c];
                output.data[y*output.step + output.channels()*x + c] = backgroundPx * (1.-opacity) + foregroundPx * opacity;
            }
        }
    }
}

/* function to return the path of the glass frame */
string read_frame_path()
{
    char temp[100];
    FILE *file;
    file = fopen("path.txt", "r");
    int k = fscanf(file, "%s", temp);
    fclose(file);
    return temp;;
}

/* function to return the path of the glass frame */
float read_gamma_value()
{
	float temp;
    FILE *file;
    file = fopen("gamma.txt", "r");
    int k = fscanf(file, "%f", &temp);
    fclose(file);
    return temp;
}

// function to adjust brightness of src image
void gamma_correction(cv::Mat& src, cv::Mat& dst, float gamma)
{
	// build look up table
	unsigned char LUT[256];
	for (int i = 0; i < 256; i++) {
		LUT[i] = cv::saturate_cast<uchar>( pow((float)(i / 255.0), gamma) * 255.0f);
	}

	dst = src.clone();
    cv::MatIterator_<uchar> it1, end1;
    cv::MatIterator_<cv::Vec3b> it2, end2;

    if( dst.channels() == 1 ){
        for (it1 = dst.begin<uchar>(), end1 = dst.end<uchar>(); it1 != end1; it1++)
               *it1 = LUT[(*it1)];
    } else if( dst.channels() == 3 ) {
        for (it2 = dst.begin<cv::Vec3b>(), end2 = dst.end<cv::Vec3b>(); it2 != end2; it2++) {
            (*it2)[0] = LUT[((*it2)[0])];
            (*it2)[1] = LUT[((*it2)[1])];
            (*it2)[2] = LUT[((*it2)[2])];
        }
    }
}

int main()
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

        deserialize("shape_predictor_68_face_landmarks.dat") >> sp2;
        /*
           training data for the shape predictor
        */

        /* string frame_path will store the path of the glass frame */
        string frame_path = "../frame/frame1.png";
        float value_of_gamma = 1.0;

        image_window win_with_frame;
        win_with_frame.set_title("Output with frame");
        /*image window for frame with titles on window*/

        /*cap captures the image from webcam and stores it in a matrix form of name temp*/
        dlib::array2d<rgb_pixel> cimg;

        //you can comment out the below line
        array2d<rgb_pixel> cam_image;
        /*store cam image that is obtained in a 2d vector which has only rgb channels and no alpha channel*/

        cv::Mat temp, glass, result;
        rgb_pixel color;
        const rgb_pixel black(0,0,0); //red color
        const rgb_pixel sunglass_color(212,175,55);
        
        while(!win_with_frame.is_closed()) {

            frame_path = read_frame_path();
            value_of_gamma =  read_gamma_value();
            
            if( (int)frame_path.length() <= 0 ) // in case if path is not selected
                continue;
            
            /* stores image of png glasse frame in glasses matrix and alpha composites them */
            glass = cv::imread(frame_path, -1 );
                        
            // Grab a frame from webcam
            cap >> temp;
            cv::Mat check_image(temp.rows, temp.cols, temp.type());
            
            // function to adjust brightness of frames
            gamma_correction(temp, temp, value_of_gamma);

            result = temp;
            
            if ( frame_path.find("lips") != string::npos ){ // if lips frame is choosed

            } else if( frame_path.find("lensframe1") != string::npos ) // lenglass frames have smaller height
                resize(glass, glass, cv::Size(350, 95), 0, 0, cv::INTER_CUBIC);
            else 
                resize(glass, glass, cv::Size(350, 120), 0, 0, cv::INTER_CUBIC);

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

            win_with_frame.clear_overlay();
            
            std::vector<image_window::overlay_line> lines;
            /*lines stores the lines used to draw the mesh; basically a vector of lines*/

            for(int i = 0 ; i < (int)dets.size() ; i++) {
                auto shape = sp2(cimg, dets[i]);
                /*
                   for this face dets[i] find the pose of the face and stores the shape in shape variable
                   in the form of points that define the pose of the face
                   this shape is a vector of 68 points(pairs) defining the pose of the face
                */

                auto top_left    = shape.part(17);
                auto top_right   = shape.part(26);
                auto mid         = shape.part(27);
                auto left_eye    = shape.part(36);
                auto right_eye   = shape.part(45);
                auto left_nasal  = shape.part(31);
                auto right_nasal = shape.part(35);
                auto left_ear    = shape.part(0);
                auto right_ear   = shape.part(16);

                source_image.clear();
                destination_image.clear();

                // points on the face in anticlockwise direction
                destination_image.push_back(cv::Point2f(top_left.x(), top_left.y()));
                destination_image.push_back(cv::Point2f(left_eye.x(), left_eye.y()));
                destination_image.push_back(cv::Point2f((left_eye.x() + left_nasal.x())/2, (left_eye.y() + left_nasal.y())/2));
                destination_image.push_back(cv::Point2f(mid.x(), mid.y()));
                destination_image.push_back(cv::Point2f((right_eye.x() + right_nasal.x())/2, (right_eye.y() + right_nasal.y())/2));
                destination_image.push_back(cv::Point2f(right_eye.x(), right_eye.y()));
                destination_image.push_back(cv::Point2f(top_right.x(), top_right.y()));
                
                // points on the glass in anticlockwise direction
                source_image.push_back(cv::Point2f(0,11));     //top left
                source_image.push_back(cv::Point2f(44, 60));   //left eye
                source_image.push_back(cv::Point2f(88, 110));  //middle left
                source_image.push_back(cv::Point2f(175,20));   //middle point
                source_image.push_back(cv::Point2f(265, 110)); //middle right
                source_image.push_back(cv::Point2f(300,66));   //right eye
                source_image.push_back(cv::Point2f(349,11));   //top right

                
                cv::Mat H = findHomography(source_image, destination_image, 0);
                cv::Mat glassWarped;
                warpPerspective(glass, glassWarped, H, temp.size());
                overlayImage(temp, glassWarped, result);


                // finding perspective transform of hinge points of frame
                std::vector<cv::Point2f> src;
                src.push_back(cv::Point2f(0,50));   //left hinge of frame
                src.push_back(cv::Point2f(349,50)); //right hinge of frame
                std::vector<cv::Point2f> dst2;
                perspectiveTransform(src, dst2, H);


                // finding final hinge points
                point right_hinge_pt, left_hinge_pt;                
                if( frame_path.find("sunglass") != string::npos ){ //hinge point is different for sunglasses
                    color = sunglass_color;
                    right_hinge_pt.x() = (int)dst2[1].x; //right hinge points
                    right_hinge_pt.y() = (int)dst2[1].y;

                    left_hinge_pt.x()  = (int)dst2[0].x; //left hinge points
                    left_hinge_pt.y()  = (int)dst2[0].y;
                } else {
                    color = black;
                    right_hinge_pt     = top_right;
                    left_hinge_pt      = top_left;    
                    right_hinge_pt.y() = right_hinge_pt.y() + 5; //right hinge points
                    left_hinge_pt.y()  = left_hinge_pt.y() + 5;  //left hinge points
                }
                

                //Drawing the mesh
				lines.push_back(image_window::overlay_line(left_hinge_pt, left_ear, color));
                lines.push_back(image_window::overlay_line(point(left_hinge_pt.x(), left_hinge_pt.y()+2), left_ear, color));
                lines.push_back(image_window::overlay_line(point(left_hinge_pt.x(), left_hinge_pt.y()+1), left_ear, color));
                lines.push_back(image_window::overlay_line(point(left_hinge_pt.x(), left_hinge_pt.y()-1), left_ear, color));
                lines.push_back(image_window::overlay_line(point(left_hinge_pt.x(), left_hinge_pt.y()-2), left_ear, color));
                lines.push_back(image_window::overlay_line(point(left_hinge_pt.x(), left_hinge_pt.y()+3), left_ear, color));

                lines.push_back(image_window::overlay_line(right_hinge_pt, right_ear, color));
                lines.push_back(image_window::overlay_line(point(right_hinge_pt.x(), right_hinge_pt.y()+2), right_ear, color));
                lines.push_back(image_window::overlay_line(point(right_hinge_pt.x(), right_hinge_pt.y()+1), right_ear, color));
                lines.push_back(image_window::overlay_line(point(right_hinge_pt.x(), right_hinge_pt.y()-1), right_ear, color));
                lines.push_back(image_window::overlay_line(point(right_hinge_pt.x(), right_hinge_pt.y()-2), right_ear, color));
                lines.push_back(image_window::overlay_line(point(right_hinge_pt.x(), right_hinge_pt.y()+3), right_ear, color));

                /*
                float min_dist = sqrt( pow((left_eye.x() - right_eye.x()), 2) + pow((left_eye.y() - right_eye.y()), 2) );
                
                if( min_dist <= 100 )
                	cout<<"Please move forward, toward camera"<<endl;
                else
                	cout<<"Perfect!"<<endl;
                */
            }

            dlib::assign_image(cimg, dlib::cv_image<bgr_pixel>(temp));
            
            //draw side frames
            win_with_frame.add_overlay(lines);
            
            win_with_frame.set_image(cimg);
        }
    }
    catch(std::exception& e) {
        cout << e.what() << endl;
    }
    return 0;
}