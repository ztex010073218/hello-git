//»²§U¨ç¦¡//
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <time.h>
using namespace std;
using namespace cv;


string index_name(int&);
void read_scv(pair<int, int>**&, string&, string&);
void filter_size_change(pair<int, int>*, bool);
void on_mouse(int, int, int, int, void*);
void show_image_by_filter(pair<int, int>**&);
void show_image(pair<int,int>**&);
//void show_image(int& point);
