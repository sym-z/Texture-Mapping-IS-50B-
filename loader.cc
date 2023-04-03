/*
Usage: call the function load_image("/public/kyoto.jpg") and it will open the file kyoto.jpg and put the RGB values into a 3D vector of ints. The size of the vector is the size of the image x 3 deep because there are three colors, Red Green and Blue. 
So if you want to get the green value at Row 100, Col 20, you do a vec.at(100).at(20).at(1);
Red = 0, Green = 1, Blue = 2
*/

#include <CImg.h>
#include <vector>
#include <iostream>
using namespace cimg_library;
using namespace std;

//Copy values from image into a 3D vector
//Its entirely this is buggy and has rows and columns mixed up
const int COLORS = 3;
void image_to_vec(const CImg<unsigned char> &image, vector<vector<vector<int>>> &vec, int cols, int rows) {
	const int stride = cols * rows;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < COLORS; k++) {
				vec.at(i).at(j).at(k) = image[k*stride+i*cols+j];
			}
		}
	}
}

vector<vector<vector<int>>> load_image(string filename) {
	try {
		CImg<unsigned char> image (filename.c_str());
		int cols = image.width(); 
		int rows = image.height();

		//Create a new 3D vector to pass to the students' image filter code
		//Even though it is of ints, it is really uint8_t's, any value over 255 will cap at 255
		vector<vector<vector<int>>> vec(rows,vector<vector<int>>(cols,vector<int>(COLORS)));
		image_to_vec(image,vec,cols,rows); //Copy data from image to vec to make it easier on students
		return vec;
	} catch (...) {
		cout << "Error opening file\n";
		exit(EXIT_FAILURE);
	}
}

