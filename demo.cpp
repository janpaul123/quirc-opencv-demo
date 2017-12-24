#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "quirc-lib/quirc.h"

int main(int argc, char **argv) {
  cv::VideoCapture videoCapture(0);
  int width = videoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
  int height = videoCapture.get(CV_CAP_PROP_FRAME_HEIGHT);

  struct quirc *qr = quirc_new();
  quirc_resize(qr, width, height);

  namedWindow("window", cv::WINDOW_AUTOSIZE);
  cv::Mat video_mat, grey_mat, grey_uchar_mat;

  while (true) {
    videoCapture.read(video_mat);
    cvtColor(video_mat, grey_mat, CV_BGR2GRAY);
    grey_mat.convertTo(grey_uchar_mat, CV_8U);

    uint8_t *qr_image = quirc_begin(qr, NULL, NULL);
    memcpy(qr_image, grey_uchar_mat.data, width*height);
    quirc_end(qr);

    int num_codes = quirc_count(qr);
    for (int i=0; i<num_codes; i++) {
      struct quirc_code code;
      quirc_extract(qr, i, &code);

      for (int j=0; j<4; j++) {
        struct quirc_point *a = &code.corners[j];
        struct quirc_point *b = &code.corners[(j + 1) % 4];
        line(video_mat, cv::Point(a->x, a->y), cv::Point(b->x, b->y), cv::Scalar(110, 220, 0), 2, 8);
      }

      struct quirc_data data;
      quirc_decode_error_t err = quirc_decode(&code, &data);
      if (!err) {
        putText(video_mat, (char*)data.payload, cv::Point(code.corners[0].x, code.corners[0].y), cv::FONT_HERSHEY_TRIPLEX, 1, cv::Scalar(110, 220, 0));
      }
    }

    imshow("window", video_mat);

    if(cv::waitKey(1) >= 0) break;
  }

  quirc_destroy(qr);
  videoCapture.release();
  return 0;
}
