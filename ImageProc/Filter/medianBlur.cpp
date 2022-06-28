#include <opencv2/opencv.hpp>
#include <cassert>


namespace Prac
{
  void medianBlur(cv::Mat& _src, cv::Mat& _dst, cv::Size kSize)
  {
    assert((kSize.height % 2 == 1) && (kSize.width % 2 == 1));

    int pad_w = (kSize.width - 1) / 2;
    int pad_h = (kSize.height - 1) / 2;
    int mid = kSize.height * kSize.width / 2;

    cv::Mat src;
    cv::copyMakeBorder(_src, src, pad_h, pad_h, pad_w, pad_w, cv::BORDER_REFLECT_101);
    _dst = cv::Mat::zeros(_src.rows, _src.cols, _src.type());

    for (int y = 0; y < _src.rows; ++y)
    {
      for (int x = 0; x < _src.cols; ++x)
      {
        for (int c = 0; c < src.channels(); ++c)
        {
          std::vector<uchar> vec;
          for (int k_y = y; k_y < y + kSize.height; ++k_y)
          {
            for (int k_x = x; k_x < x + kSize.width; ++k_x)
            {
              vec.push_back(src.ptr<uchar>(k_y, k_x)[c]);
            }
          } 
          std::sort(vec.begin(), vec.end());
          _dst.ptr<uchar>(y, x)[c] = vec[mid];
        }
      }
    }
  }

}// namespace

int testMedianBlur()
{
  cv::Mat lena = cv::imread(R"(Lena.png)");
  cv::Mat dst;

  Prac::medianBlur(lena, dst, cv::Size(5, 5));

  return 0;
}
