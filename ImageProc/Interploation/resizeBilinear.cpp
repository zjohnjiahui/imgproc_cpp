#include<opencv2/opencv.hpp>

// refer:
// https://blog.csdn.net/weixin_40647819/article/details/86601070
// https://www.cnblogs.com/Imageshop/archive/2011/11/12/2246808.html

namespace Prac {

  void resizeBilinear(cv::Mat& src, cv::Mat& dst, cv::Size size, double sx=0, double sy=0) {
    int dst_rows;
    int dst_cols;

    if (size.height != 0 && size.width != 0) {
      dst_rows = size.height;
      dst_cols = size.width;
      sy = static_cast<double>(dst_rows) / src.rows;
      sx = static_cast<double>(dst_cols) / src.cols;
    }
    else if (sx != 0 && sy != 0) {
      dst_rows = static_cast<int>(round(sy*src.rows));
      dst_cols = static_cast<int>(round(sx*src.cols));
    }
    else {
      dst = src.clone();
      return;
    }

    dst = cv::Mat(dst_rows, dst_cols, src.type());

    for (int i = 0; i < dst.rows; i++) {
      double index_i = (i + 0.5) / sy - 0.5; // index i mapped to src
      if (index_i < 0) index_i = 0;
      if (index_i > src.rows - 1) index_i = src.rows - 1;

      int i1 = static_cast<int>(floor(index_i));
      int i2 = static_cast<int>(ceil(index_i));
      double u = index_i - i1;

      for (int j = 0; j < dst.cols; j++) {

        double index_j = (j + 0.5) / sx - 0.5; // index j mapped to src
        if (index_j < 0) index_j = 0;
        if (index_j > src.cols - 1) index_j = src.cols - 1;

        int j1 = static_cast<int>(floor(index_j));
        int j2 = static_cast<int>(ceil(index_j));
        double v = index_j - j1;

        uchar* ptr = dst.ptr<uchar>(i, j);
        for (int c = 0; c < src.channels(); ++c)
        {
          ptr[c] = static_cast<uchar>(
            (1 - u)*(1 - v)*src.ptr<uchar>(i1, j1)[c] +
            (1 - u)*v*src.ptr<uchar>(i1, j2)[c] +
            u * (1 - v)*src.ptr<uchar>(i2, j1)[c] +
            u * v*src.ptr<uchar>(i2, j2)[c]);
        }
      }
    }
  }

  void resizeBilinearQuantization(cv::Mat& src, cv::Mat& dst, cv::Size size, double sx=0, double sy=0)
  {
    int dst_rows;
    int dst_cols;

    if (size.height != 0 && size.width != 0) {
      dst_rows = size.height;
      dst_cols = size.width;
      sy = static_cast<double>(dst_rows) / src.rows;
      sx = static_cast<double>(dst_cols) / src.cols;
    }
    else if (sx != 0 && sy != 0) {
      dst_rows = static_cast<int>(round(sy*src.rows));
      dst_cols = static_cast<int>(round(sx*src.cols));
    }
    else {
      dst = src.clone();
      return;
    }

    dst = cv::Mat(dst_rows, dst_cols, src.type());

    int quantiztionBits = 11;
    int quantizationLevel = 1 << quantiztionBits; // 2^11

    for (int i = 0; i < dst.rows; i++) {
      double index_i = (i + 0.5) / sy - 0.5; // index i mapped to src
      if (index_i < 0) index_i = 0;
      if (index_i > src.rows - 1) index_i = src.rows - 1;

      int i1 = static_cast<int>(floor(index_i));
      int i2 = i1 + 1;
      int u = static_cast<int>((index_i - i1) * quantizationLevel);

      for (int j = 0; j < dst.cols; j++) {

        double index_j = (j + 0.5) / sx - 0.5; // index j mapped to src
        if (index_j < 0) index_j = 0;
        if (index_j > src.cols - 1) index_j = src.cols - 1;

        int j1 = static_cast<int>(floor(index_j));
        int j2 = j1 + 1;
        int v = static_cast<int>((index_j - j1) * quantizationLevel);

        uchar* ptr = dst.ptr<uchar>(i, j);
        uchar* ptr1 = src.ptr<uchar>(i1, j1);
        uchar* ptr2 = ptr1 + src.channels();
        uchar* ptr3 = ptr1 + src.step[0];
        uchar* ptr4 = ptr3 + src.channels();
        for (int c = 0; c < src.channels(); ++c)
        {
          /*ptr[c] = static_cast<uchar>((
            (quantizationLevel - u)*(quantizationLevel - v)*src.ptr<uchar>(i1, j1)[c] +
            (quantizationLevel - u)*v*src.ptr<uchar>(i1, j2)[c] +
            u * (quantizationLevel - v)*src.ptr<uchar>(i2, j1)[c] +
            u * v*src.ptr<uchar>(i2, j2)[c]) >> 2 * quantiztionBits);*/
          ptr[c] = static_cast<uchar>(
            ((quantizationLevel - u)*((quantizationLevel - v)*ptr1[c] + v * ptr2[c]) +
              u * ((quantizationLevel - v)*ptr3[c] + v * ptr4[c])) >> 2 * quantiztionBits);

        }
      }
    }
  }

}// namespace

int testBilinear()
{
  cv::Mat lena = cv::imread(R"(D:\VisualStudio\CVPractice\ImageProc\Lena.png)");

  cv::Mat dst;
  Prac::resizeBilinear(lena, dst, cv::Size(1024, 1024));
  Prac::resizeBilinearQuantization(lena, dst, cv::Size(1024, 1024));

  return 0;
}
