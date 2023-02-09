#pragma once

#include <iostream>

#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

class ManualMatcher
{
public:
  ManualMatcher(const std::string &_left_img_path,
                const std::string &_right_img_path)
    : m_rng(12345)
  {
    cv::Mat left_img = cv::imread(_left_img_path, cv::IMREAD_ANYCOLOR);
    cv::Mat right_img = cv::imread(_right_img_path, cv::IMREAD_ANYCOLOR);

    if (left_img.channels() < 3)
      cv::cvtColor(left_img, left_img, cv::COLOR_GRAY2BGR);

    if (right_img.channels() < 3)
      cv::cvtColor(right_img, right_img, cv::COLOR_GRAY2BGR);

    cv::hconcat(left_img, right_img, m_matching_img);

    m_nleft_img_cols = left_img.cols;

    m_nkp_radius = m_nleft_img_cols / 200;

    if (m_matching_img.cols > 1920)
    {
      m_sx = 1920. / static_cast<double>(m_matching_img.cols);
      m_sy = m_sx;
    }

    if (static_cast<double>(m_matching_img.rows) * m_sy > 1080.)
    {
      m_sy = 1080. / static_cast<double>(m_matching_img.rows);
      m_sx = m_sy;
    }

    cv::resize(m_matching_img, m_matching_img, cv::Size(), m_sx, m_sy);
  }

  void manualMatching(std::vector<cv::Point2f> &_vpts_left, std::vector<cv::Point2f> &_vpts_right)
  {
    const std::string window_name = "Manual Matching";
    cv::namedWindow(window_name, cv::WINDOW_KEEPRATIO);

    cv::setMouseCallback(window_name, onMouse, this);

    while(!m_bmatching_done)
    {
      cv::Mat img2draw = m_matching_img.clone();
      for (size_t i=0; i < m_vright_kps.size(); ++i)
      {
        cv::circle(img2draw, m_vleft_kps[i], m_nkp_radius, m_vkps_color[i], -1);
        cv::circle(img2draw, m_vright_kps[i], m_nkp_radius, m_vkps_color[i], -1);
        cv::line(img2draw, m_vleft_kps[i], m_vright_kps[i], m_vkps_color[i], std::max(m_nkp_radius / 3, 1));
      }
      if (m_vleft_kps.size() > m_vright_kps.size())
      {
        cv::circle(img2draw, m_vleft_kps.back(), m_nkp_radius, m_vkps_color.back(), -1);
      }

      cv::imshow(window_name, img2draw);

      cv::waitKey(25);
    }

    cv::destroyAllWindows();

    _vpts_left.reserve(m_vright_kps.size());
    _vpts_right.reserve(m_vright_kps.size());

    const float inv_sx = 1.f / static_cast<float>(m_sx);
    const float inv_sy = 1.f / static_cast<float>(m_sy);

    for (size_t i=0; i < m_vright_kps.size(); ++i)
    {
      const auto& lkp = m_vleft_kps[i];
      const auto& rkp = m_vright_kps[i];
      _vpts_left.push_back(cv::Point2f(static_cast<float>(lkp.x) * inv_sx,
                                       static_cast<float>(lkp.y) * inv_sy));
      _vpts_right.push_back(cv::Point2f(static_cast<float>(rkp.x - m_nleft_img_cols) * inv_sx,
                                       static_cast<float>(rkp.y) * inv_sy));
    }
  } 

private:

  static void onMouse(int event, int x, int y, int flag, void* param)
  {
    ManualMatcher* pthis = static_cast<ManualMatcher*>(param);

    // Add new kp / match
    if (event == cv::EVENT_LBUTTONDOWN)
    {
      // Click on right image
      if (x >= pthis->m_nleft_img_cols)
      {
        // Only validate point if left image first clicked
        if (pthis->m_vleft_kps.size() > pthis->m_vright_kps.size())
        {
          pthis->m_vright_kps.push_back(cv::Point(x, y));
        }
      }
      else // Click on left image
      {
        if (pthis->m_vleft_kps.size() == pthis->m_vright_kps.size())
        {
          pthis->m_vleft_kps.push_back(cv::Point(x, y));
          pthis->m_vkps_color.push_back(cv::Scalar(
                                              pthis->m_rng.uniform(0,255),
                                              pthis->m_rng.uniform(0,255),
                                              pthis->m_rng.uniform(0,255)));
        }
      }
    }

    // Cancel last kp / match
    if (event == cv::EVENT_MBUTTONDOWN)
    {
      if (pthis->m_vleft_kps.size() > pthis->m_vright_kps.size())
      {
        pthis->m_vleft_kps.pop_back();
        pthis->m_vkps_color.pop_back();
      }
      else
      {
        pthis->m_vright_kps.pop_back();
      }
    }

    if (event == cv::EVENT_RBUTTONDBLCLK)
    {
      pthis->m_bmatching_done = true;
    }
  }

  cv::RNG m_rng;

  cv::Mat m_matching_img;
  int m_nleft_img_cols;
  double m_sx = 1; 
  double m_sy = 1;
  
  std::vector<cv::Point2i> m_vleft_kps, m_vright_kps;
  std::vector<cv::Scalar> m_vkps_color;

  int m_nkp_radius;

  bool m_bmatching_done = false;
};
