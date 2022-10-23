#include <opencv2/opencv.hpp>
#include <cassert>

namespace Prac
{

void meanBlur(cv::Mat& _src, cv::Mat& _dst, cv::Size k)
{
  assert((k.height % 2 == 1) && (k.width % 2 == 1));

  int pad_w = (k.width - 1) / 2;
  int pad_h = (k.height - 1) / 2;

  cv::Mat src;
  cv::copyMakeBorder(_src, src, pad_h, pad_h, pad_w, pad_w, cv::BORDER_REFLECT_101);

  _dst = cv::Mat::zeros(_src.size(), _src.type());

  for (int i = 0; i < _src.rows; ++i)
  {
    for (int j = 0; j < _src.cols; ++j)
    {
      // [i,i+h) [j,j+w)
      for (int c = 0; c < src.channels(); ++c)
      {
        int tmp = 0;
        for (int k_i = i; k_i < i + k.height; ++k_i)
        {
          for (int k_j = j; k_j < j + k.width; ++k_j)
          {
            tmp += src.ptr<uchar>(k_i, k_j)[c];
          }
        }
        _dst.ptr<uchar>(i, j)[c] = tmp / k.area();
      }
    }
  }
}

}// namespace

int testMeanBlur()
{
  cv::Mat lena = cv::imread(R"(Lena.png)");
  cv::Mat dst;

  Prac::meanBlur(lena, dst, cv::Size(5,5));
  
  return 0;
}
