#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <time.h>
#include "sup.h"

using namespace cv;
using namespace std;

#define width 703
#define length 1329
pair<int, int> **data_array = new pair<int, int>*[width*length]();

int main(int argc, char **argv)
{
	time_t start, end;
	start = time(NULL);

	string file_name = "2274_DefectData_Part";
	string path = "../source/";
	

	read_scv(data_array, path, file_name);
	show_image(data_array);
	
	waitKey(0);
	end = time(NULL);
	double diff = difftime(end, start);
	printf("Time = %f\n", diff);
}

