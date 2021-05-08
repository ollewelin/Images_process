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
cv::Mat makeSquareImg(cv::Mat frame, int outputImageSize, std::string file_location)
{
  
  cv::Mat rectImage, graymat;
  frame.convertTo(frame, CV_32FC3, 1.0f / 255.0f);
  //Resize input image to fit into a example 224 x 224 dataset images
  //First step scale the input image so the least side h or w will be resized to example 224
  //Next step crop the larger h or w to example 224 pixel as well, crop with images in center
  //bool inp_landscape;
  int x_start = 0;
  int y_start = 0;
  int inputSize = 0;
  //Find out the smallest side hight or width of the input image
  cv::Mat varLight;
//  for(int test=0;test<10;test++){

  
  float Rando = (float) (rand() % 65535) / 65536;//0..1.0 range
  if (frame.rows == 0 || frame.cols == 0)
  {
    //Zero divition protection here.
    printf("Error! Zero divition protection input image rows = %d cols = %d. Exit program.\n", frame.rows, frame.cols);
    printf("file_location = ");
    cout << file_location << endl;

    exit(0);
  }
  else
  {
    
    if (frame.rows > frame.cols)
    {
      //Input images is a portrait mode
      //Calculate the starting point of the square rectangle
      inputSize = frame.cols;
  //    y_start = (frame.rows / 2) - inputSize / 2;
      float y_start_flt = ((float)(frame.rows - inputSize) * Rando);
      y_start = (int)y_start_flt;
      if(y_start < 0){
        y_start = 0;
      }
      if(y_start > frame.rows-1){
        y_start = frame.rows-1;
      }
      x_start = 0;
    }
    else
    {
      //Input images is a landscape mode
      //Make a square rectangle of input image
      inputSize = frame.rows;
    //  x_start = (frame.cols / 2) - inputSize / 2;
      float x_start_flt = ((float)(frame.cols - inputSize) * Rando);
      x_start = (int)x_start_flt;
      if(x_start < 0){
        x_start = 0;
      }
      if(x_start > frame.cols-1){
        x_start = frame.cols-1;
      }
      y_start = 0;
    }
    //Make a square rectangle of input image
    //Mat rect_part(image, Rect(rand_x_start, rand_y_start, Width, Height));//Pick a small part of image
    cv::Mat rectImageTemp(frame, cv::Rect(x_start, y_start, inputSize, inputSize)); //
    //Size size(input_image_width,input_image_height);//the dst image size,e.g.100x100
    cv::Size outRectSize(outputImageSize, outputImageSize);
    //resize(src,dst,size);//resize image
    cv::resize(rectImageTemp, rectImage, outRectSize);
  }

  //Flip
//  Rando = (float) (rand() % 65535) / 65536;//0..1.0 range
//  cv::Mat unfliped = rectImage.clone();
//  if (Rando > 0.5){
//    cv::flip(rectImage,rectImage,1);//1 x axis flip
//  }
  Rando = (float) (rand() % 65535) / 65536;//0..1.0 range
  float light_rand = 1.0f;
  Rando *= light_rand;
  Rando -= light_rand / 2.0f;

  varLight = rectImage.clone();
  varLight = varLight * (1.0f + Rando);
/*
  cv::imshow("frame", frame);
  cv::imshow("Unfliped image", unfliped);
  cv::imshow("rectImage", rectImage);
  cv::imshow("varLight", varLight);
  cv::waitKey(5000);
*/
//  }
  return varLight;
}

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
  std::string str1 = "./data/";
  std::string str2 = "/";
  std::string str3 = ".jpg";
    std::string train_file;


  DIR *dp;
  int nr_of_files = 0;
  struct dirent *ep;     
  dp = opendir ("./data/");

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
    flip_img_str = str1 + inp_img_str + str3;
    printf("input_file = ");
    cout << input_file << endl;

    cv::Mat img = cv::imread(input_file);
    cv::Mat fliped_img;
   // cv::Mat imgSquarRect = makeSquareImg(img, input_img_size, input_file);
   //cv::flip(src,dst,1);//1 x axis flip
    cv::flip(img,fliped_img,1);//1 x axis flip
    cv::imshow("img", img);
    cv::imshow("fliped_img", fliped_img);
    printf("output_file = ");
    cout << flip_img_str << endl;
    cv::imwrite(flip_img_str, fliped_img);
    cv::waitKey(100);

  }
}
