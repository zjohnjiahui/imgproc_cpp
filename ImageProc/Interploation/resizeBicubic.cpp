#include<opencv2/opencv.hpp>

// refer: 
// https://blog.csdn.net/qq_29058565/article/details/52769497
// https://blog.csdn.net/u010555688/article/details/24352343

namespace Prac {

  void getWeight(double weight[4], double x, double a = -0.5) {
    int X = static_cast<int>(x);
    double dist[4];
    dist[0] = 1 + (x - X);
    dist[1] = x - X;
    dist[2] = 1 - (x - X);
    dist[3] = 2 - (x - X);

    weight[0] = a * abs(dist[0] * dist[0] * dist[0]) - 5 * a*dist[0] * dist[0] + 8 * a*abs(dist[0]) - 4 * a;
    weight[1] = (a + 2)*abs(dist[1] * dist[1] * dist[1]) - (a + 3)*dist[1] * dist[1] + 1;
    weight[2] = (a + 2)*abs(dist[2] * dist[2] * dist[2]) - (a + 3)*dist[2] * dist[2] + 1;
    weight[3] = a * abs(dist[3] * dist[3] * dist[3]) - 5 * a*dist[3] * dist[3] + 8 * a*abs(dist[3]) - 4 * a;
  }

  void resizeBicubic(cv::Mat& src, cv::Mat& dst, cv::Size size, double sx=0, double sy=0)
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
    cv::Mat _src(src.rows + 4, src.cols + 4, src.type()); // padding
    src.copyTo(_src(cv::Rect(2, 2, src.cols, src.rows)));
    double w_y[4]; // row weight
    double w_x[4]; // col weight

    for (int y = 0; y < dst.rows; y++) {
      double index_y = (y + 0.5) / sy - 0.5; // index i mapped to src
      if (index_y < 0) index_y = 0;
      if (index_y > src.rows - 1) index_y = src.rows - 1;
      index_y += 2; // padding offset

      getWeight(w_y, index_y);

      for (int x = 0; x < dst.cols; x++) {

        double index_x = (x + 0.5) / sx - 0.5; // index j mapped to src
        if (index_x < 0) index_x = 0;
        if (index_x > src.cols - 1) index_x = src.cols - 1;
        index_x += 2;

        getWeight(w_x, index_x);

        int window_y = static_cast<int>(index_y - (index_y - static_cast<int>(index_y)) - 1);
        int window_x = static_cast<int>(index_x - (index_x - static_cast<int>(index_x)) - 1);
        cv::Mat window_roi = _src(cv::Rect(window_x, window_y, 4, 4));

        uchar* ptr = dst.ptr<uchar>(y, x);

        for (int c = 0; c < _src.channels(); ++c)
        {
          double temp = 0;
          for (int u = 0; u < 4; ++u)
          {
            for (int v = 0; v < 4; ++v)
            {
              temp += static_cast<double>(window_roi.ptr<uchar>(u, v)[c]) * w_y[u] * w_x[v];
            }
          }
          temp = temp > 255 ? 255 : temp;
          ptr[c] = static_cast<uchar>(temp);
        }
      }
    }
  }

} // namespace


void testBicubic()
{
  cv::Mat lena = cv::imread(R"(Lena.png)");

  cv::Mat dst;
  Prac::resizeBicubic(lena, dst, cv::Size(1024, 1024));
  
  return;
}
