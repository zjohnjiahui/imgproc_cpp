#include <opencv2/opencv.hpp>
#include <cassert>


namespace Prac
{
  void integralMeanBlur(cv::Mat& _src, cv::Mat& _dst, cv::Size kSize)
  {
    assert((kSize.height % 2 == 1) && (kSize.width % 2 == 1));

    int pad_w = (kSize.width - 1) / 2;
    int pad_h = (kSize.height - 1) / 2;
    int area = kSize.area();

    cv::Mat src;
    cv::copyMakeBorder(_src, src, pad_h, pad_h, pad_w, pad_w, cv::BORDER_REFLECT_101);
    _dst = cv::Mat::zeros(_src.rows, _src.cols, _src.type());

    cv::Mat inte;
    cv::integral(src, inte);


    for (int y = 0; y < _src.rows; ++y)
    {
      for (int x = 0; x < _src.cols; ++x)
      {
        // calcu sum of [y,y+kSize.height) [x,x+kSize.width) in _src
        uchar* ptr = _dst.ptr<uchar>(y, x);
        int* ptr_top_left = inte.ptr<int>(y, x);
        int* ptr_top_right = inte.ptr<int>(y, x + kSize.width);
        int* ptr_bottom_left = inte.ptr<int>(y + kSize.height, x);
        int* ptr_bottom_right = inte.ptr<int>(y + kSize.height, x + kSize.width);

        // sum = br + tl - tr - bl
        for (int c = 0; c < src.channels(); ++c)
        {
          int sum = ptr_bottom_right[c] + ptr_top_left[c] -
            ptr_top_right[c] - ptr_bottom_left[c];
          ptr[c] = cv::saturate_cast<uchar>(sum /area);
        }
      }
    }    
  }

}// namespace

int testintegralMeanBlur()
{
  cv::Mat lena = cv::imread(R"(Lena.png)");
  cv::Mat dst;

  Prac::integralMeanBlur(lena, dst, cv::Size(5, 5));

  return 0;
}
