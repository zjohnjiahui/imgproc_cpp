
void resizeNearest(cv::Mat& src, cv::Mat& dst, cv::Size size, double sx = 0, double sy = 0) {
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

    int i1 = static_cast<int>(round(index_i));

    for (int j = 0; j < dst.cols; j++) {

      double index_j = (j + 0.5) / sx - 0.5; // index j mapped to src
      if (index_j < 0) index_j = 0;
      if (index_j > src.cols - 1) index_j = src.cols - 1;

      int j1 = static_cast<int>(round(index_j));

      uchar* ptr = dst.ptr<uchar>(i, j);
      for (int c = 0; c < src.channels(); ++c)
      {
        ptr[c] = static_cast<uchar>(src.ptr<uchar>(i1, j1)[c]);
      }
    }
  }
}