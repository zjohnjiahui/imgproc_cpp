#include <opencv2/opencv.hpp>

void resizeNearest(cv::Mat& src, cv::Mat& dst, cv::Size size, double sx = 0, double sy = 0);
void resizeBilinear(cv::Mat& src, cv::Mat& dst, cv::Size size, double sx = 0, double sy = 0);
void resizeBilinearQuantization(cv::Mat& src, cv::Mat& dst, cv::Size size, double sx = 0, double sy = 0);
void resizeBicubic(cv::Mat& src, cv::Mat& dst, cv::Size size, double sx = 0, double sy = 0);

int main()
{
  cv::Mat lena = cv::imread(R"(D:\VisualStudio\CVPractice\ImageProc\Lena.png)");

  cv::Mat dst;
  resizeBilinear(lena, dst, cv::Size(1024, 1024));

  return 0;
}