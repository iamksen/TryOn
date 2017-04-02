#include <iostream>
#include <dlib/dnn.h> /* shape_predictor */
#include <dlib/data_io.h> /* load_image */
#include <dlib/opencv.h> /* cv_image */
#include <dlib/gui_widgets.h> /* image_window, ... */
#include <dlib/image_processing/frontal_face_detector.h>
#include <opencv2/highgui/highgui.hpp> /* for VideoCapture */
 
using namespace std;
using namespace dlib;
 
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
		const rgb_pixel color(255,0,0); //red color

		while(!win.is_closed()) {
			// Grab a frame from webcam
			cap >> temp;
		    
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

				auto lear  = shape.part(0);//left ear point
				auto leye_left = shape.part(36);//left eye left point
				auto leye_right = shape.part(39);//left eye right point

				auto rear = shape.part(16);// right ear
				auto reye_right = shape.part(45);// right eye right point
				auto reye_left = shape.part(42);//right eye left point

				auto nose = shape.part(33);//nose point

				auto top_left = shape.part(19); //left eyebrow top point
				auto top_right = shape.part(24); //right eyebrow top point


				// Draw the glasses onto the image.
				std::vector<point> from = {2*point(145,25), 2*point(35,25)};
				std::vector<point> to = {reye_right, leye_left};
					
				auto tform = find_similarity_transform(from, to);
				
				/*we have the math, just look for it on the paper*/
				for (long r = 0; r < glasses.nr(); ++r) {
					for (long c = 0; c < glasses.nc(); ++c) {
						point p = tform(point(c,r));
						//point p gives the transformed points , tells where does a point go after transformation and returns its point
						if (get_rect(cimg).contains(p))
							assign_pixel( cimg[p.y()][p.x()], glasses(r,c));
							/*
							if p is a valid point lying inside the face window, then check for it if true, then assign the pixel value of glasses(r,c) pixel i.e. 
							value of its first pixel to the pixel on window(face wali) at posi p.x & p.y. implies that the color of the pixel on window
							at p.x & p.y would be the color of pixel at the glass r,c
							*/
					}
				}

				//Drawing the mesh
				lines.push_back(image_window::overlay_line(lear, leye_left, color));
				lines.push_back(image_window::overlay_line(rear, reye_right, color));
				lines.push_back(image_window::overlay_line(leye_left, leye_right, color));
				lines.push_back(image_window::overlay_line(reye_right, reye_left, color));
				lines.push_back(image_window::overlay_line(leye_left, nose, color));
				lines.push_back(image_window::overlay_line(reye_right, nose, color));
				lines.push_back(image_window::overlay_line(leye_left, top_left, color));
				lines.push_back(image_window::overlay_line(reye_right, top_right, color));
				lines.push_back(image_window::overlay_line(top_left, top_right, color));
			}
				
			win_with_mesh.add_overlay(lines);
			win_with_frame.set_image(cimg);
		}
	}
	catch(std::exception& e) {
		cout << e.what() << endl;
	}
	return 0;
}