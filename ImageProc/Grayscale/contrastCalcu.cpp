#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>  

// refer:
// https://en.wikipedia.org/wiki/Contrast_(vision)

// calcu contrast of single channel image,
// if the image is multichannel, only calu the first channel

namespace Prac {

  // MichelsonContrast ~ [0,1]
  float calcuMichelsonContrast(const cv::Mat& src)
  {
    // (Imax - Imin)/(Imax + Imin)
    uchar Imin = 255;
    uchar Imax = 0;
    for (int i = 0; i < src.rows; ++i)
    {
      const uchar* ptr = src.ptr<uchar>(i, 0);
      for (int j = 0; j < src.cols; ++j)
      {
        Imin = *ptr < Imin ? *ptr : Imin;
        Imax = *ptr > Imax ? *ptr : Imax;
        ptr += src.channels();
      }
    }

    std::cout << (int)Imax << std::endl;
    std::cout << (int)Imin << std::endl;

    return static_cast<float>(Imax - Imin) / (Imax + Imin);
  }


  float calcuRMSContrast(const cv::Mat& src)
  {
    // std of normalized img

    cv::Mat src_f(src.rows, src.cols, CV_32FC1);
    for (int i = 0; i < src.rows; ++i)
    {
      const uchar* ptr = src.ptr<uchar>(i, 0);
      float* ptr_f = src_f.ptr<float>(i, 0);
      for (int j = 0; j < src.cols; ++j)
      {
        *ptr_f = *ptr / 255.0f;

        ptr += src.channels();
        ++ptr_f;
      }
    }

    cv::Mat mean;
    cv::Mat std;
    cv::meanStdDev(src_f, mean, std);

    double* p = reinterpret_cast<double*>(std.data);

    return static_cast<float>(*p);
  }

} // namespace

void testCalcuContrast()
{
  cv::Mat lena = cv::imread(R"(Lena.png)");
  cv::Mat lena_gray;
  cv::cvtColor(lena, lena_gray, cv::COLOR_BGR2GRAY);

  float contrastMichelson = Prac::calcuMichelsonContrast(lena_gray);
  std::cout << "contrastMichelson: " << contrastMichelson << std::endl;
  float contrastRMS = Prac::calcuRMSContrast(lena_gray);
  std::cout << "contrastRMS: " << contrastRMS << std::endl;

  return;
}
