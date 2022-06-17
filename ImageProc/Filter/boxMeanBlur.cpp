#include <opencv2/opencv.hpp>
#include <cassert>

// refer:
// https://www.cnblogs.com/lwl2015/p/4460711.html

namespace Prac
{
  void boxMeanBlur(cv::Mat& _src, cv::Mat& _dst, cv::Size kSize)
  {
    assert((kSize.height % 2 == 1) && (kSize.width % 2 == 1));

    int pad_w = (kSize.width - 1) / 2;
    int pad_h = (kSize.height - 1) / 2;
    int area = kSize.area();

    cv::Mat src;
    cv::copyMakeBorder(_src, src, pad_h, pad_h, pad_w, pad_w, cv::BORDER_REFLECT_101);
    _dst = cv::Mat::zeros(_src.rows, _src.cols, _src.type());

    for (int c = 0; c < src.channels(); ++c)
    {
      std::vector<int> buf(src.cols, 0); // col buf
      // buf init, sum of 0 ~ KSize.height rows, per col
      for (int i = 0; i < src.cols; ++i)
      {
        for (int j = 0; j < kSize.height; ++j)
        {
          buf[i] += src.ptr<uchar>(j, i)[c];
        }
      }

      // shift window
      for (int y = 0; y < _src.rows; ++y) // row
      {
        int sum = 0;
        // sum of 0~kSize.width cols
        for (int w = 0; w < kSize.width; ++w)
          sum += buf[w];

        for (int x = 0; x < _src.cols; ++x) // col
        {
          if (x != 0)
            sum = sum - buf[x - 1] + buf[x + kSize.width - 1];
          _dst.ptr<uchar>(y, x)[c] = cv::saturate_cast<uchar>(sum / area);
        }

        // refresh buf for new row
        int add_row = y + kSize.height;
        for (int x = 0; x < src.cols; ++x)
        {
          uchar pixel_discard = src.ptr<uchar>(y, x)[c];
          uchar pixel_add = 0;
          if (add_row < src.rows)
            pixel_add = src.ptr<uchar>(add_row, x)[c];
          buf[x] = buf[x] - pixel_discard + pixel_add;
        }
      }
    }

  }

}// namespace

int testBoxMeanBlur()
{
  cv::Mat lena = cv::imread(R"(D:\VisualStudio\CVPractice\ImageProc\Lena.png)");
  cv::Mat dst;

  Prac::boxMeanBlur(lena, dst, cv::Size(5,5));

  return 0;
}
