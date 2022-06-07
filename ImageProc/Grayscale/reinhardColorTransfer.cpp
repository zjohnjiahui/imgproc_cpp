#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>

// refer:
// https://zhuanlan.zhihu.com/p/267832794
// https://github.com/chia56028/Color-Transfer-between-Images


// src shall be in bgr or lab color space
// the mean and std of src will be the same as tgt's after transfer 
int transfer(cv::Mat& src, const cv::Mat& tgt)
{
  cv::Mat tgt_mean;
  cv::Mat src_mean;
  cv::Mat tgt_std;
  cv::Mat src_std;
  cv::meanStdDev(tgt, tgt_mean, tgt_std);
  cv::meanStdDev(src, src_mean, src_std);

  double* s_mean = (double*)src_mean.data;
  double* s_std = (double*)src_std.data;
  double* t_mean = (double*)tgt_mean.data;
  double* t_std = (double*)tgt_std.data;
  int channel = 3;

  for (int i = 0; i < src.rows; ++i)
  {
    for (int j = 0; j < src.cols; ++j)
    {
      for (int k = 0; k < channel; ++k)
      {
        float x = src.ptr<uchar>(i)[j*channel + k];

        int tmp = static_cast<int>(((x - s_mean[k]) * (t_std[k] / s_std[k])) + t_mean[k]); // color transfer formula

        // boundary check
        tmp = tmp < 0 ? 0 : tmp;
        tmp = tmp > 255 ? 255 : tmp;
        src.ptr<uchar>(i)[j*channel + k] = static_cast<uchar>(tmp);
      }
    }
  }

  return 0;
}

int main()
{
  cv::Mat lena = cv::imread(R"(Lena.png)");
  cv::Mat lena_lab;
  cv::cvtColor(lena, lena_lab, cv::COLOR_BGR2Lab);

  cv::Mat girl = cv::imread(R"(Girl.png)");
  cv::Mat girl_lab;
  cv::cvtColor(girl, girl_lab, cv::COLOR_BGR2Lab);

  transfer(lena_lab, girl_lab);
  cv::Mat dst;
  cv::cvtColor(lena_lab, dst, cv::COLOR_Lab2BGR);

  return 0;
}
