#include <opencv2/opencv.hpp>
#include <cassert>

namespace Prac{

  void integral(cv::Mat& src, cv::Mat& dst)
  {
    dst = cv::Mat::zeros(src.rows + 1, src.cols + 1, src.type());
    dst.convertTo(dst, CV_32S);

    for (int c = 0; c < src.channels(); ++c)
    {
      for (int i = 1; i < dst.rows; ++i) {
        int sum_r = 0;
        for (int j = 1; j < dst.cols; ++j) {
          sum_r = src.ptr<uchar>(i - 1, j - 1)[c] + sum_r; // sum of each row
          dst.ptr<int>(i, j)[c] = dst.ptr<int>(i - 1, j)[c] + sum_r;
        }
      }
    }
  }

  void integral(cv::Mat& src, cv::Mat& dst, int channel) {
    assert(channel < src.channels());

    dst = cv::Mat::zeros(src.rows + 1, src.cols + 1, CV_32S);
    for (int i = 1; i < dst.rows; ++i) {
      int sum_r = 0;
      for (int j = 1; j < dst.cols; ++j) {
        sum_r = src.ptr<uchar>(i - 1, j - 1)[channel] + sum_r; // sum of each row
        dst.at<int>(i, j) = dst.at<int>(i - 1, j) + sum_r;
      }
    }
  }

} // namespace

int testIntegral()
{
  cv::Mat lena = cv::imread(R"(Lena.png)");
  cv::Mat dst0;
  cv::Mat dst1;

  Prac::integral(lena, dst0);
  cv::integral(lena, dst1);

  return 0;
}
