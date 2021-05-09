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
const int input_img_size = 224;

auto main() -> int
{
  printf("Use this example command to rename jpg images in a folder\n");
  printf("$ counter=0; for file in *; do [[ -f $file ]] && mv -i %c$file%c $((counter+1)).jpg && ((counter++)); done\n", 34, 34);
      
///
    printf("Enter continue \n");
    char answer_character = getchar();


  srand (static_cast <unsigned> (time(0)));///Seed the randomizer (need to do only once)
  srand(time(NULL));

  int test_class = 1;
  int train_class = 1;
  int rand_img_indx;
  std::string str1 = "./data/src/";
  std::string str2 = "./data/src/";
  std::string str3 = ".jpg";
    std::string train_file;


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
  


    rand_img_indx = rand() % (1000);
    std::string train_class_str = std::to_string(train_class);
    std::string train_img_str = std::to_string(rand_img_indx);
    train_file = str1 + train_class_str + str2 + train_img_str + str3;
    std::string input_file;
    printf("\n");
    printf("************ Test image ************* \n");


  //  rand_img_indx = rand() % (custom_dataset_size / nr_of_classes);
    std::string test_class_str = std::to_string(test_class);
    std::string inp_img_str = std::to_string(rand_img_indx);
    std::string flip_img_str = std::to_string(rand_img_indx);
    input_file = str1 + test_class_str + str2 + inp_img_str + str3;
    input_file = str1 + test_class_str + str2 + "617" + str3;

  for(int i=0;i<nr_of_files;i++)
  {
    inp_img_str = std::to_string(i+1);
    input_file = str1 + inp_img_str + str3;
    inp_img_str = std::to_string(i+nr_of_files+1);
    flip_img_str = str2 + inp_img_str + str3;
    printf("input_file = ");
    cout << input_file << endl;

    cv::Mat img = cv::imread(input_file);
    cv::Mat fliped_img;
   // cv::Mat imgSquarRect = makeSquareImg(img, input_img_size, input_file);
   //cv::flip(src,dst,1);//1 x axis flip
    cv::flip(img,fliped_img,1);//1 x axis flip
    cv::imwrite(flip_img_str, fliped_img);

    if(i<100){
    cv::imshow("img", img);
    cv::imshow("fliped_img", fliped_img);
    cv::waitKey(100);

    }
    printf("output_file = ");
    cout << flip_img_str << endl;

  }
}
