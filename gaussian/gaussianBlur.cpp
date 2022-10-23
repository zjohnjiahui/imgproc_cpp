#include <opencv2/opencv.hpp>
#include <cassert>


namespace Prac
{
  void guassianBlur(cv::Mat& _src, cv::Mat& _dst, cv::Size kSize, double sigma)
  {
    // assert kSize
    assert((kSize.height % 2 == 1) && (kSize.width % 2 == 1));

    // pad src img
    int pad_w = (kSize.width - 1) / 2;
    int pad_h = (kSize.height - 1) / 2;

    cv::Mat src;
    cv::copyMakeBorder(_src, src, pad_h, pad_h, pad_w, pad_w, cv::BORDER_REFLECT_101);
    _dst = cv::Mat::zeros(_src.rows, _src.cols, _src.type());

    // create gauss kernel
    int center_w = pad_w;
    int center_h = pad_h;
    cv::Mat kernel(kSize, CV_64F);
    double sum = 0.0f;
    double sigma2 = sigma * sigma;
    for (int i = 0; i < kSize.height; ++i)
    {
      double y = pow(i - center_h, 2);
      for (int j = 0; j < kSize.width; ++j)
      {
        double x = pow(j - center_w, 2);
        double g = exp(-(x+y)/(2* sigma2));
        kernel.at<double>(i, j) = g;
        sum += g;
      }
    }
    kernel = kernel / sum;

    // apply kernel
    for (int y = 0; y < _src.rows; ++y)
    {
      for (int x = 0; x < _src.cols; ++x)
      {
        for (int c = 0; c < src.channels(); ++c)
        {
          int conv = 0;
          for (int k_y = y; k_y < y + kSize.height; ++k_y)
          {
            for (int k_x = x; k_x < x + kSize.width; ++k_x)
            {
              conv += static_cast<int>(
                kernel.at<double>(k_y - y, k_x - x) * src.ptr<uchar>(k_y, k_x)[c]);
            }
          }
          _dst.ptr<uchar>(y, x)[c] = cv::saturate_cast<uchar>(conv);
        }
      }
    }
  }

}// namespace

int testGaussianBlur()
{
  cv::Mat lena = cv::imread(R"(Lena.png)");
  cv::Mat dst;

  Prac::guassianBlur(lena, dst, cv::Size(5, 5), 3);

  return 0;
}
