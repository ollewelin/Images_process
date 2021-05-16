//#include <cstddef>
//#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>///rand() srand()
#include <string>
#include <vector>

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

//#include <tuple>
//#include "model.h"
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp> // Basic OpenCV structures (cv::Mat, Scalar)


using namespace std;
using namespace cv;

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " <option(s)> SOURCES"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-W,--width [float] Argument specify width gain of output imaged, range 0.05 to 1.0. In example -W 0.859\n"
              << "\t-H,--height [float] Argument specify height gain of output imaged, range 0.05 to 1.0. In example -H 0.859\n"
              << "\t-OH [float] Argument specify offset height of output imaged, range 0.0 to 1.0. In example -OH 0.9 , 0.5 = center\n"
              << "\t-OW [float] Argument specify offset width of output imaged, range 0. to 1.0. In example -OW 0.1 , 0.5 = center\n"

              << std::endl;
                printf("\n");            

}

bool isFloat( string myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}
int CheckNoZero(int a)
{
  if(a<1)
  {
    a=1;
    cout << "\033[1;31mError! Zero integer detect \033[0m" << endl;
  }
  return a;
}
int main(int argc, char* argv[])
{
    if (argc < 3) {
        show_usage(argv[0]);
        return 1;
    }
    float width_g = 1.0f;
    float height_g = 1.0f;
    float offset_w = 0.5f;
    float offset_h = 0.5f;

    std::vector <std::string> sources;
    std::string destination;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return 0;
        } else if ((arg == "--width") || (arg == "-W") || (arg == "--height") || (arg == "-H") || (arg == "-OH") || (arg == "-OW")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                argv[i++]; // Increment 'i' so we don't get the argument as the next argv[i].
                if(isFloat(argv[i])==true){
                  //printf("It is a Float arg \n");
                  if((arg == "--width") || (arg == "-W")){
                    width_g = stof(argv[i]);
                  }
                  if((arg == "--height") || (arg == "-H")){
                    height_g = stof(argv[i]);
                  }
                  if((arg == "-OH") ){
                    offset_h = stof(argv[i]);
                  }
                  if((arg == "-OW")){
                    offset_w = stof(argv[i]);
                  }

                }
                else{
                  cout << "\033[1;31mError! \033[0m";
                  printf("user input argument failure !\n");
                  cout << "\033[1;31m" << argv[i] << "\033[0m";
                  printf("  Is Not a proper float argument try instead In example 0.95\n");
                  printf("use -h or --help argumet\n");
                }
            } else { // Uh-oh, there was no argument to the destination option.
                  cout << "\033[1;31mError! \033[0m";
                  printf("user input argument failure !\n");
                  std::cerr << "--destination option requires one argument." << std::endl;
                  printf("use -h or --help argumet\n");
                return 1;
            }  
        } else {
            sources.push_back(argv[i]);
        }
    }

    const float min_g = 0.05f;
    if(width_g < min_g){
      width_g = min_g;
    }
    if(height_g < min_g){
      height_g = min_g;
    }
    if(width_g > 1.0f){
      width_g = 1.0f;
    }
    if(height_g > 1.0f){
      height_g = 1.0f;
    }
    if(offset_w > 0.99f){ offset_w = 0.99f; }
    if(offset_h > 0.99f){ offset_h = 0.99f; }
    if(offset_w < 0.0f){ width_g = 0.0f; }
    if(offset_h < 0.0f){ offset_h = 0.0f; }


    printf("witdh gain = %f\n", width_g);    
    printf("height gain = %f\n", height_g);
    printf("offset_w = %f\n", offset_w);    
    printf("offset_h = %f\n", offset_h);
    
  std::string str1 = "./data/src/";
  std::string str2 = "/";
  std::string str3 = ".jpg";
  std::string str5 = "./data/dst/";

///
  cout << "Information: " << endl;
  cout << "Path of images source file is: " << endl << endl << str1 << endl << endl;
  cout << "Path of images destination file is: " << endl << endl << str5 << endl << endl;
  cout << "All source images files MUST be organized in this way 0.jpg and up. No number gaps allowed in the " << str1 << endl << endl;
  printf("Tip Images rename in Linux command\n");
  printf("Use this example command to rename jpg images in a folder\n");
  printf("$ counter=0; for file in *; do [[ -f $file ]] && mv -i %c$file%c $((counter+1)).jpg && ((counter++)); done\n\n", 34, 34);
  printf("Enter continue \n");
  char answer_character = getchar();

  printf("Checking images in source folder\n");
  DIR *dp;
  int nr_of_files = 0;
  
  struct dirent *ep;     
  dp = opendir ("./data/src/");

  if (dp != NULL)
  {
    while (ep = readdir (dp))
      nr_of_files++;

    (void) closedir (dp);
  }
  else
    perror ("Couldn't open the directory");

  nr_of_files = nr_of_files -2;
  if(nr_of_files < 0){nr_of_files = 0;}
  printf("There's %d files in the current directory.\n", nr_of_files);
  
     std::string input_file;
    printf("\n");
    printf("************ Test image ************* \n");


    std::string inp_img_str = std::to_string(0);
    std::string output_file = std::to_string(0);


 printf("Check order of files\n");
  for(int i=0;i<nr_of_files;i++){
    inp_img_str = std::to_string(i+1);
    input_file = str1 + inp_img_str + str3;
    printf("input_file = ");
    cout << input_file << endl;
    cv::Mat img = cv::imread(input_file);
    cv::imshow("input img", img);
     cv::waitKey(100);
  }

  for(int i=0;i<nr_of_files;i++)
  {
    
    inp_img_str = std::to_string(i+1);
    input_file = str1 + inp_img_str + str3;
    printf("input_file = ");
    cout << input_file << endl;
    cv::Mat img = cv::imread(input_file);
   // cv::imshow("input img", img);
   // cv::waitKey(2000);

    cv::Mat frame;
    img.convertTo(frame, CV_32FC3, 1.0f / 255.0f);
    cv::imshow("frame", frame);
    cv::waitKey(100);
    int inp_img_x = img.cols;
    int inp_img_y = img.rows;
    int width_int = (int)((float)inp_img_x * width_g);
    int height_int = (int)((float)inp_img_y * height_g);
    width_int = CheckNoZero(width_int);
    height_int = CheckNoZero(height_int);
    inp_img_x = CheckNoZero(inp_img_x);
    inp_img_y = CheckNoZero(inp_img_y);
 
    int x_start = 0;
    int y_start = 0;

    x_start = (inp_img_x / 2) - (width_int / 2);
    y_start = (inp_img_y / 2) - (height_int / 2);

    x_start += (int)((float)x_start * ((offset_w-0.5f)*2.0f));
    y_start += (int)((float)y_start * ((offset_h-0.5f)*2.0f));
    if((x_start+width_int)>(frame.cols-1)){
      cout << "\033[1;31mError! x_start to high detect \033[0m" << endl;
      x_start=frame.cols-width_int;
    }
    if((y_start+height_int)>(frame.rows-1)){
      cout << "\033[1;31mError! y_start to high detect \033[0m" << endl;
      y_start=frame.rows-height_int;
    }

    if(x_start<0){
      cout << "\033[1;31mError! x_start<0 detect \033[0m" << endl;
      x_start=0;
    }
    if(y_start<0){
      cout << "\033[1;31mError! y_start<0 detect \033[0m" << endl;
      y_start=0;
    }

    printf("x_start = %d\n", x_start);
    printf("y_start = %d\n", y_start);
    printf("width_int = %d\n", width_int);
    printf("height_int = %d\n", height_int);
    printf("inp_img_x = %d\n", inp_img_x);
    printf("inp_img_y = %d\n", inp_img_y);


    //Mat rect_part(image, Rect(rand_x_start, rand_y_start, Width, Height));//Pick a small part of image
    cv::Mat shriked_img(frame, cv::Rect(x_start, y_start, width_int, height_int)); //
    shriked_img.convertTo(shriked_img, CV_8UC3, 255);

    output_file = std::to_string(i+1);
    output_file = str5 + output_file + str3;
    printf("output_file = ");
    cout << output_file << endl;
    cv::imshow("output shriked_img images", shriked_img);
    cv::waitKey(100);
    cv::imwrite(output_file, shriked_img);

  }
}
