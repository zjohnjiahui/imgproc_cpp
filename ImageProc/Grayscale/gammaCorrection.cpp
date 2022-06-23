#include <opencv2/opencv.hpp>
#include <cmath>

// refer: 
// https://blog.csdn.net/linqianbi/article/details/78617615
// https://blog.csdn.net/weixin_40647819/article/details/88027391
// https://zhuanlan.zhihu.com/p/36581276
// https://blog.csdn.net/qq_15972907/article/details/123748166

namespace Prac {

  void makeGammaLUT(uchar LUT[], float gamma)
  {
    for (int i = 0; i < 256; ++i)
    {
      float cur_f = (i + 0.5f) / 255; // normalize to [0, 1]
      cur_f = powf(cur_f, gamma); // I^r
      LUT[i] = cv::saturate_cast<uchar>(cur_f*255.0f - 0.5f);
    }
  }

  void applyLUT(cv::Mat& src, uchar LUT[])
  {
    for (int i = 0; i < src.rows; ++i)
    {
      uchar* ptr = src.ptr<uchar>(i, 0);
      for (int j = 0; j < src.cols; ++j)
      {
        for (int c = 0; c < src.channels(); ++c)
        {
          ptr[c] = LUT[ptr[c]];
        }
        ptr += src.channels();
      }
    }
  }

} //namespace 

int testGammaCorrection()
{
  cv::Mat lena = cv::imread(R"(Lena.png)");

  uchar LUT[256];
  Prac::makeGammaLUT(LUT, 1/2.2);
  Prac::applyLUT(lena, LUT);
  cv::imshow("lena", lena);

  return 0;
}
