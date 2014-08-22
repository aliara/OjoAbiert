#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <iostream>

using namespace cv;

/** @function main */
int main( int argc, char** argv )
{

  Mat src, src_gray;
  Mat grad;
  char* window_gray = "Gris";
  char* window_sobel = "Sobel";
  char* window_threshold = "Threshold";
  char* window_thres2 = "Segundo umbral";
  char* window_canny = "Canny";
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;



  /// Load an image
  src = imread( argv[1] );

  if( !src.data )
  { return -1; }

  GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

  /// Convert it to gray
  cvtColor( src, src_gray, CV_RGB2GRAY );
  namedWindow( window_gray, CV_WINDOW_AUTOSIZE );
  moveWindow(window_gray,0,0);
  imshow(window_gray,src_gray);
  threshold( src_gray, src_gray, 65, 255,2 );
  namedWindow( window_threshold, CV_WINDOW_AUTOSIZE );
  moveWindow(window_threshold,150,0);
  imshow(window_threshold,src_gray);
  /// Create window
  namedWindow( window_sobel, CV_WINDOW_AUTOSIZE );

  /// Generate grad_x and grad_y
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );

  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_y, abs_grad_y );

  /// Total Gradient (approximate)
  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
  moveWindow(window_sobel,300,0);
  imshow( window_sobel, grad );
  threshold( grad, grad, 30, 255,1 );
  namedWindow( window_thres2, CV_WINDOW_AUTOSIZE );
  moveWindow(window_thres2,450,0);
  imshow( window_thres2, grad );
  std::cout<<"Filas: "<<grad.rows<<"	"<<"Columnas: "<<grad.cols<<std::endl;
  Canny(grad, grad, 50, 200, 3);
  namedWindow( window_canny, CV_WINDOW_AUTOSIZE );
  moveWindow(window_canny,600,0);
    imshow( window_canny, grad );
  vector<Vec2f> lines;
  HoughLines(grad, lines, 20, CV_PI/180, 100, 0, 0 );
  std::cout<<"Lineas detectadas: "<<lines.size()<<std::endl;
  for( size_t i = 0; i < lines.size(); i++ )
  {
    float rho = lines[i][0], theta = lines[i][1];
    Point pt1, pt2;
    double a = cos(theta), b = sin(theta);
    double x0 = a*rho, y0 = b*rho;
    pt1.x = cvRound(x0 + 1000*(-b));
    pt1.y = cvRound(y0 + 1000*(a));
    pt2.x = cvRound(x0 - 1000*(-b));
    pt2.y = cvRound(y0 - 1000*(a));
    line( src_gray, pt1, pt2, Scalar(0,0,0), 3, CV_AA);
  }



  waitKey(0);

  return 0;
  }