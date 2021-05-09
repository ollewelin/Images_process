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

Mat transf(Mat src, Mat background)
{
   Point2f srcTri[3];
   Point2f dstTri[3];

    float scale_r;
    float scale_r_n;
    float scale_gain=0.45f;///Change here
    float trans_x_r;
    float trans_y_r;
    float transelation_x = 0.0f;
    float transelation_y = 0.0f;
    float x_gain = 0.2f;///Change here
    float y_gain = 0.2f;///Change here
    float rot_max_min = 25.0f;///Change here
    float rot_random =0.0f;

 //   srand (static_cast <unsigned> (time(0)));//Seed the randomizer
    scale_r = (float) (rand() % 65535) / 65536;
 //   printf("scale_r %f\n", scale_r);
    trans_x_r = (float) (rand() % 65535) / 65536;
 //   printf("trans_x_r %f\n", trans_x_r);
    trans_y_r = (float) (rand() % 65535) / 65536;
 //   printf("trans_y_r %f\n", trans_y_r);
    rot_random = (float) (rand() % 65535) / 65536;
 //   printf("rot_random %f\n", rot_random);

    scale_r = 0.5f + scale_r;
//    printf("scale_r %f\n", scale_r);
    scale_r_n = 1.0f - scale_r;


transelation_x = (trans_x_r - 0.5f) * x_gain;
transelation_y = (trans_y_r - 0.5f) * y_gain;

    scale_r = ((scale_r - 1.0f) * scale_gain) + 1.0f;
    scale_r_n = scale_r_n * scale_gain;
//    printf("scale_r_n %f\n", scale_r_n);

   Mat rot_mat( 2, 3, CV_8UC1 );
   Mat warp_mat( 2, 3, CV_8UC1 );
   Mat warp_dst, warp_rotate_dst;

   /// Set the dst image the same type and size as src
   warp_dst = Mat::zeros( src.rows, src.cols, src.type() );



   /// Set your 3 points to calculate the  Affine Transform
   srcTri[0] = Point2f( 0,0 );
   srcTri[1] = Point2f( src.cols - 1, 0 );
   srcTri[2] = Point2f( 0, src.rows - 1 );

//2:1
   dstTri[0] = Point2f( src.cols*0.25, src.rows*0.25 );
   dstTri[1] = Point2f( src.cols*0.75, src.rows*0.25 );
   dstTri[2] = Point2f( src.cols*0.25, src.rows*0.75 );


   /// Get the Affine Transform
   warp_mat = getAffineTransform( srcTri, dstTri );


   /// Apply the Affine Transform just found to the src image
   warpAffine( src, warp_dst, warp_mat, warp_dst.size() );
//   dstTri[0] = Point2f( src.cols*(0.25+transelation_x), src.rows*0.25 );
//   dstTri[1] = Point2f( src.cols*(0.75+transelation_x), src.rows*0.25 );
//   dstTri[2] = Point2f( src.cols*(0.25+transelation_x), src.rows*0.75 );

//   dstTri[0] = Point2f( src.cols*(0.0+transelation_x), src.rows*(0.0+transelation_y) );
//   dstTri[1] = Point2f( src.cols*(1.0+transelation_x), src.rows*(0.0+transelation_y) );
//   dstTri[2] = Point2f( src.cols*(0.0+transelation_x), src.rows*(1.0+transelation_y) );

   dstTri[0] = Point2f( src.cols*(scale_r_n+transelation_x), src.rows*(scale_r_n+transelation_y) );
   dstTri[1] = Point2f( src.cols*(scale_r+transelation_x), src.rows*(scale_r_n+transelation_y) );
   dstTri[2] = Point2f( src.cols*(scale_r_n+transelation_x), src.rows*(scale_r+transelation_y) );

    /// Get the Affine Transform
   warp_mat = getAffineTransform( srcTri, dstTri );

   /// Do the Affine Transformation scale and translation
   warpAffine( src, warp_dst, warp_mat, warp_dst.size() );

//================== Rotation ==============================
   /** Rotating the image after Warp */
    rot_random = rot_random - 0.5f;// Make it to a random value of +/- 0.5
    rot_random = rot_random * rot_max_min;
//    printf("rot_random %f\n", rot_random);

   /// Compute a rotation matrix with respect to the center of the image
   Point center = Point( warp_dst.cols/2, warp_dst.rows/2 );
   double angle = 0.0;
   double rot_scale = 1.0;
    angle = (double)rot_random;
   /// Get the rotation matrix with the specifications above
   rot_mat = getRotationMatrix2D( center, angle, rot_scale );

   /// Rotate the warped image
   warpAffine( warp_dst, warp_rotate_dst, rot_mat, warp_dst.size() );
//============= End Rotation ==============================
//   imshow("warp_dst", warp_dst);
//   imshow("warp_rotate_dst", warp_rotate_dst);
//   imshow("src", src);

   cv::Mat orig_background = background.clone();
   cv::Size outRectSize(warp_rotate_dst.cols, warp_rotate_dst.rows);
 //  printf("warp_rotate_dst.cols =%d\n", warp_rotate_dst.cols);
 //  printf("warp_rotate_dst.rows =%d\n", warp_rotate_dst.rows);
   //resize(src,dst,size);//resize image
   // imshow("background", background);
  //  waitKey(5000);
   cv::resize(background,background,outRectSize);
 
float *index_ptr_warp_rotate_dst = warp_rotate_dst.ptr<float>(0);
float *index_ptr_background = background.ptr<float>(0);
if(background.cols == warp_dst.cols && background.rows == warp_dst.rows && background.type() == warp_dst.type()){
    //Sanity check OK
   for(int c=0;c<warp_rotate_dst.cols;c++){
       for(int r=0;r<warp_rotate_dst.rows;r++){
           for(int ch=0;ch<warp_rotate_dst.channels();ch++){
               if(*index_ptr_warp_rotate_dst == 0.0f)
               {
                   //Keep background data
               }
               else
               {
                   //Fill with affine data
                   *index_ptr_background = *index_ptr_warp_rotate_dst;
               }
               index_ptr_background++;
               index_ptr_warp_rotate_dst++;
           }
       }
   }
}
else{
    //Sanity check Fail
    printf("Error background Mat is not equal with warp_dst Mat Exit program \n");
    exit(0);
}
    //imshow("orig_background", orig_background);
  //  imshow("background", background);
   //imshow("warp_rotate_dst", warp_rotate_dst);
   //waitKey(1000);

    return background;
}

cv::Mat makeSquareImg(cv::Mat frame, int outputImageSize, std::string file_location, cv::Mat background_src)
{

  
  cv::Mat rectImage, graymat;
  frame.convertTo(frame, CV_32FC3, 1.0f / 255.0f);
  background_src.convertTo(background_src, CV_32FC3, 1.0f / 255.0f);
  //imshow("background_src",background_src);
  //waitKey(3000);

  frame = transf(frame, background_src);
  
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
  }
/// first center crop image to cut away rotation black corners

    //Size up image 
    const float size_up = 1.0f;
    //Size size(input_image_width,input_image_height);//the dst image size,e.g.100x100
    int frm_col = frame.cols;
    int frm_row = frame.rows;
    int x_size =  (int)(size_up*(float)frame.cols);
//    printf("x_size = %d\n", x_size);
    int y_size = (int)(size_up*(float)frame.rows);
//    printf("y_size = %d\n", y_size);
//    printf("frame.cols =%d\n", frame.cols);
//    printf("frame.rows =%d\n", frame.rows);

    cv::Size outRectSize(x_size, y_size);
    
    //resize(src,dst,size);//resize image
    cv::resize(frame,frame,outRectSize);
//    printf("frame.cols =%d\n", frame.cols);
//    printf("frame.rows =%d\n", frame.rows);

   // cv::imshow("frame", frame);
   // cv::waitKey(5000);
    //Mat rect_part(image, Rect(rand_x_start, rand_y_start, Width, Height));//Pick a small part of image
    x_start = (frame.cols-frm_col)/2;
    y_start = (frame.rows-frm_row)/2;
 //   printf("x_start =%d\n", x_start);
 //   printf("y_start =%d\n", y_start);
    int width = frm_col;
    int height = frm_row;
 //   printf("width =%d\n", width);
 //   printf("height =%d\n", height);
    //Mat rect_part(image, Rect(rand_x_start, rand_y_start, Width, Height));//Pick a small part of image
    cv::Mat rectImageTemp(frame, cv::Rect(x_start, y_start, width, height)); //
    //Size size(input_image_width,input_image_height);//the dst image size,e.g.100x100
//    cv::imshow("rectImageTemp", rectImageTemp);
//    cv::waitKey(5000);
///

  Rando = (float) (rand() % 65535) / 65536;//0..1.0 range
  const float light_rand_strengt = 0.5f;
  Rando *= light_rand_strengt;
  Rando -= light_rand_strengt / 2.0f;

  varLight = rectImageTemp.clone();
  varLight = varLight * (1.0f + Rando);
  varLight.convertTo(varLight, CV_8UC3, 255);
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

 
  int rand_img_indx;
  std::string str1 = "./data/src/";
  std::string str2 = "/";
  std::string str3 = ".jpg";
  std::string str4 = "./data/Background/";
  std::string str5 = "./data/Paste/";


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
  
  DIR *dp2;
  int nr_of_background = 0;
  struct dirent *ep2;     
  dp2 = opendir ("./data/Background/");

  if (dp2 != NULL)
  {
    while (ep2 = readdir (dp2))
      nr_of_background++;

    (void) closedir (dp2);
  }
  else
    perror ("Couldn't open the directory");

  nr_of_background = nr_of_background -2;
  if(nr_of_background < 0){nr_of_background = 0;}
  printf("There's %d files background directory.\n", nr_of_background);



    rand_img_indx = rand() % (1000);
    std::string input_file;
    printf("\n");
    printf("************ Test image ************* \n");


  //  rand_img_indx = rand() % (custom_dataset_size / nr_of_classes);
    std::string inp_img_str = std::to_string(0);
    std::string background_str = std::to_string(0);
    std::string output_file = std::to_string(0);
  const int nr_randomize_images = 5;
  printf("Randomize input images by factor =%d\n", nr_randomize_images);

  printf("Check order of files\n");
  for(int i=0;i<nr_of_background;i++){
      inp_img_str = std::to_string(i+1);
      background_str = str4 + inp_img_str + str3;
      std::cout << background_str << std::endl;

      cv::Mat background_src = cv::imread(background_str);
      cv::imshow("background_src", background_src);
      cv::waitKey(1);
  }

 printf("Check order of files\n");
  for(int i=0;i<nr_of_files;i++){
    inp_img_str = std::to_string(i+1);
    input_file = str1 + inp_img_str + str3;
    printf("input_file = ");
    cout << input_file << endl;
    cv::Mat img = cv::imread(input_file);
    cv::imshow("input img", img);
     cv::waitKey(1);
  }



  for(int i=0;i<nr_of_files;i++)
  {
    
    inp_img_str = std::to_string(i+1);
    input_file = str1 + inp_img_str + str3;
    printf("input_file = ");
    cout << input_file << endl;
    cv::Mat img = cv::imread(input_file);
    cv::imshow("input img", img);
    for(int r=0;r<nr_randomize_images;r++)
    {
      rand_img_indx = rand() % nr_of_background;
      inp_img_str = std::to_string(rand_img_indx+1);
      background_str = str4 + inp_img_str + str3;
      std::cout << background_str << std::endl;
      cv::Mat background_src = cv::imread(background_str);

      output_file = std::to_string(r+50000 + nr_randomize_images*i);
      output_file = str1 + output_file + str3;
      int random_sized_img = rand() % 350;
      random_sized_img += 224;//Minimum 224 pixel
      printf("Random size =%d\n", random_sized_img);
      cv::Mat imgSquarRect = makeSquareImg(img, random_sized_img, input_file, background_src);
      printf("output_file = ");
      cout << output_file << endl;
      
      //cv::imwrite("output file images", imgSquarRect);
      cv::imshow("output file images", imgSquarRect);
      cv::waitKey(100);
      cv::imwrite(output_file, imgSquarRect);
    }


  //  cv::Mat img = cv::imread(input_file);
 //   cv::Mat fliped_img;
   // cv::Mat imgSquarRect = makeSquareImg(img, input_img_size, input_file);
   //cv::flip(src,dst,1);//1 x axis flip
   // cv::flip(img,fliped_img,1);//1 x axis flip
   // cv::imshow("img", img);
   // cv::imshow("fliped_img", fliped_img);

  }
}
