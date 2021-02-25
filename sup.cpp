#include "sup.h"
#define CV_EVENT_LBUTTONDOWN 0
#define CV_EVENT_RBUTTONDOWN 2
#define CV_EVENT_LBUTTONUP 4
#define CV_EVENT_RBUTTONUP 5

#define CV_EVENT_FLAG_LBUTTON 1
#define screen_width 703
#define screen_length 1329      
#define filter_size_delta 100

extern pair<int, int> **data_array;
Point Pre_pt;
Point End_pt;
Point filter_position(screen_width/2,screen_length/2);
Rect r;
bool level_up_filter = false;
bool level_down_filter = false;
bool move_filter = false;
int filter_level = 0;
pair<int, int> filter_size(screen_width,screen_length);

string index_name(int &index)
{
	string output;
	if (index >= 1000)
	{
		output = to_string(index);
	}
	else if(index >= 100)
	{
		output = "0" + to_string(index);
	}
	else if (index >= 10)
	{
		output = "00" + to_string(index);
	}
	else
	{
		output = "000" + to_string(index);
	}
	return output;
}

void read_scv(pair<int, int>**& array, string& path, string& file_name)
{
	ifstream input_csv;
	string line;
	string cell;
	pair<int, int> *location;
	for (int i = 1; i <= 25; i++)
	{
		input_csv.open(path + file_name + index_name(i) + ".csv", ios::in);
		while (!input_csv.good())
		{
			cout << "open file fail at " << file_name << index_name(i) << ".csv\n";
			exit(1);
		}
		getline(input_csv, line);
		while (getline(input_csv, line, ','))
		{
			location = new pair<int, int>;
			for (int j = 1; j < 34; j++)
			{
				if (j < 33)
					getline(input_csv, line, ',');
				else
					getline(input_csv, line, '\n');
				if (j == 7)
					location->first = stoi(line);
				else if (j == 8)
					location->second = stoi(line);
			}
			array[(1328 - (location->second + 743))*screen_width + (location->first - 166)] = location;
		}
		input_csv.close();
	}
}

void filter_size_change(pair<int, int>* filter, bool enlarge)
{
	if (enlarge)
	{
		filter->first += filter_size_delta;
		filter->second += filter_size_delta * 2;
		if (filter_position.x + (int)(0.5 * filter_size.first) > screen_width)
		{
			filter_position.x = screen_width - (int)(0.5 * filter_size.first);
		}
		else if(filter_position.x + (int)(0.5 * filter_size.first) < 0)
		{
			filter_position.x = (int)(0.5 * filter_size.first);
		}
		if (filter_position.y + (int)(0.5 * filter_size.second) > screen_width)
		{
			filter_position.y = screen_width - (int)(0.5 * filter_size.second);
		}
		else if (filter_position.y + (int)(0.5 * filter_size.second) < 0)
		{
			filter_position.y = (int)(0.5 * filter_size.second);
		}
	}
	else
	{
		filter->first -= filter_size_delta;
		filter->second -= filter_size_delta * 2;
	}
	//cout << "filter size x:" << filter->first << " y:" << filter->second << '\n';
}

void on_mouse(int EVENT, int x, int y, int flags, void* ustc)
{
	if (!move_filter)
	{
		Pre_pt.x = x;
		Pre_pt.y = y;
	}
	else
	{
		End_pt.x = x;
		End_pt.y = y;
	}
	if (EVENT == CV_EVENT_LBUTTONDOWN)
	{
		if (flags == CV_EVENT_FLAG_LBUTTON)
			move_filter = true;
		else
			level_up_filter = true;
	}
	else if (EVENT == CV_EVENT_RBUTTONDOWN)
	{
		level_down_filter = true;
	}
	else if (EVENT == CV_EVENT_LBUTTONUP )
	{
		if (level_up_filter && !move_filter)
		{
			filter_level++;
			filter_size_change(&filter_size, false);
			cout << "左鍵點擊" << '\n';
			cout << "filter_level: " << filter_level << '\n';
		}
		move_filter = false;
		level_up_filter = false;
	}
	else if (EVENT == CV_EVENT_RBUTTONUP)
	{
		if (level_down_filter && filter_level > 0)
		{
			filter_level--;
			filter_size_change(&filter_size, true);
			cout << "filter_level: " << filter_level << '\n';
		}
		level_down_filter = false;
	}
	if (move_filter)
	{
		int move_distance_x, move_distance_y;
		move_distance_x = End_pt.x - Pre_pt.x;
		move_distance_y = End_pt.y - Pre_pt.y;
		if (filter_position.x - move_distance_x + 0.5 * filter_size.first <= screen_width && filter_position.x - move_distance_x - 0.5 * filter_size.first >= 0)
		{
			filter_position.x -= move_distance_x;
		}
		/*else if (filter_position.x - move_distance_x + 0.5 * filter_size.first > screen_width)
		{
			filter_position.x = screen_width - 0.5 * filter_size.first;
		}
		else
		{
			filter_position.x = 0.5 * filter_size.first;
		}*/
		if (filter_position.y - move_distance_y + 0.5 * filter_size.second <= screen_length && filter_position.y - move_distance_y - 0.5 * filter_size.second >= 0)
		{
			filter_position.y -= move_distance_y;
		}
		/*else if (filter_position.y - move_distance_y + 0.5 * filter_size.second > screen_length)
		{
			filter_position.y = screen_length - 0.5 * filter_size.second;
		}
		else
		{
			filter_position.y = 0.5 * filter_size.second;
		}*/
		//cout << "distance x:" << move_distance_x << " y:" << move_distance_y << '\n';
		//cout << "filter position x:" << filter_position.x << " y:" << filter_position.y << '\n';
		//cout << "begin x:" << Pre_pt.x << " y:" << Pre_pt.y << '\n';
		//cout << "end   x:" << End_pt.x << " y:" << End_pt.y << '\n';
	}
	show_image_by_filter(data_array);
	//cout << Pre_pt.x << ' ' << Pre_pt.y << '\n';
}

void show_image_by_filter(pair<int, int>**& array)
{
	Mat img = Mat::zeros(Size(screen_width, screen_length), CV_8UC3);
	img.setTo(Scalar(0, 252, 124));
	int sell_width = screen_width/filter_size.first, sell_length = screen_length/filter_size.second;
	for (int i = 0; i < filter_size.second; i++)
	{
		for (int j = 0; j < filter_size.first; j++)
		{
			if (array[(filter_position.y - (int)(0.5 * filter_size.second) + i) * screen_width + (filter_position.x - (int)( 0.5 * filter_size.first) + j)] != nullptr)
			{
				if (filter_level != 0)
				{
					r.x = j * sell_length;
					r.y = i * sell_width;
					r.height = sell_length;
					r.width = sell_width;
					rectangle(img, r, Scalar(0, 0, 255), -1);
				}
				else
				{
					r.x = j;
					r.y = i;
					r.height = 1;
					r.width = 1;
					rectangle(img, r, Scalar(0, 0, 255), -1);
				}
			}
		}
	}
	imshow("image", img);
	img.release();
}


void show_image(pair<int,int>**& array)
{
	//int baseline;
	//Size text_size = getTextSize("100,100", FONT_HERSHEY_COMPLEX, 1,1, &baseline);//字的大小
	Mat img = Mat::zeros(Size(screen_width, screen_length), CV_8UC3);
	img.setTo(Scalar(0, 252, 124));
	r.height = 1;
	r.width = 1;
	for (int i = 0; i < screen_length; i++)
	{
		for (int j = 0; j < screen_width; j++)
		{
			if (array[i*screen_width + j] != nullptr)
			{
				r.x = j;
				r.y = i;
				rectangle(img, r, Scalar(0, 0, 255),1);
			}
		}
	}


	/*int shrimeScale = 2;
	resize(img, img, Size(img.cols / shrimeScale, img.rows / shrimeScale));

	Mat background = imread("../img/Background.jpg");
	Mat imageROI = background(Rect(147, 152, img.cols, img.rows)); //獲取感興趣區域，即logo要放置的區域
	addWeighted(imageROI, 0, img, 1, 0, imageROI); //圖像疊加

	resize(background, background, Size(background.cols / shrimeScale, background.rows / shrimeScale));

	//putText(background, "100,100", Point(20, 20), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 0));*/

	//imshow("image", background);
	imshow("image", img);
	img.release();
	setMouseCallback("image", on_mouse, NULL);
	waitKey(10);
}

