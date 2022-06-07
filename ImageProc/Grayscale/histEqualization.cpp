#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>  

// refer:
// https://zhuanlan.zhihu.com/p/382430357
// https://blog.csdn.net/weixin_40163266/article/details/113802909

// Statistical histogram
void statHist(const cv::Mat& image, int* hist)
{
  int idx = 0;
  for (int i = 0; i < image.rows; i++)
  {
    for (int j = 0; j < image.cols; j++)
    {
      idx = int(image.at<uchar>(i, j));
      hist[idx]++;
    }
  }
}

// Get map table
void mapTable(const int* hist, const int size, uchar* table)
{ 
  // map_value = (cumu_num / size) * 255
  // (cumu_num / size) ~ [0, 1]

  float factor = 255.0f / size;
  int cumu_num = 0; // [0, size]
  int map_value;
  for (int i = 0; i < 256; i++)
  {
    cumu_num += hist[i];
    map_value = int(cumu_num * factor);
    map_value = (map_value > 255) ? 255 : map_value;
    table[i] = uchar(map_value);
  }
}

// Mapping
void map(const cv::Mat& src, const uchar* table, cv::Mat& dst)
{
  src.copyTo(dst);
  for (int i = 0; i < src.rows; i++)
  {
    for (int j = 0; j < src.cols; j++)
    {
      dst.at<uchar>(i, j) = table[(int)src.at<uchar>(i, j)];
    }
  }
}

// Global histogram equlization
void histEqualization(const cv::Mat& src, cv::Mat& dst)
{
  int hist[256] = { 0 };
  uchar table[256] = { 0 };
  statHist(src, hist);
  mapTable(hist, src.rows * src.cols, table);
  map(src, table, dst);
}

int test()
{
  cv::Mat lena = cv::imread(R"(D:\VisualStudio\CVPractice\ImageProc\Lena.png)");
  cv::Mat lena_gray;
  cv::cvtColor(lena, lena_gray, cv::COLOR_BGR2GRAY);
  cv::Mat dst_gray;
  histEqualization(lena_gray, dst_gray);

  cv::Mat lena_hsv;
  cv::cvtColor(lena, lena_hsv, cv::COLOR_BGR2HSV);
  std::vector<cv::Mat> channels;
  cv::split(lena_hsv, channels);

  cv::Mat  lena_v_eq;
  histEqualization(channels[2], lena_v_eq);
  channels[2] = lena_v_eq;
  cv::merge(channels, lena_hsv);
  cv::Mat dst_v;
  cv::cvtColor(lena_hsv, dst_v, cv::COLOR_HSV2BGR);

  return 0;
}
