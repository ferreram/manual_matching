#include "manual_matching.hpp"


void displayCommands()
{
  std::cout << "\n\n***********************************************\n";
  std::cout << "***********************************************\n\n";
  std::cout << "\tManual Matching Program\n\n";
  std::cout << "\t   ------------------\n\n";
  std::cout << "1) Select a keypoint on the left image with a left click\n";
  std::cout << "2) Select its match on the right image with a left click\n";
  std::cout << "3) Repeat (1) and (2) as much as needed...\n";
  std::cout << "4) Stop with a double right click\n\n\n";
  std::cout << "* Middle click to cancel previous keypoint / match selection";
  std::cout << "\n\n***********************************************\n";
  std::cout << "***********************************************\n";
}

void displayMatchingResults(
    const std::vector<cv::Point2f>& _vleft_kps,
    const std::vector<cv::Point2f>& _vright_kps)
{
  std::cout << "\n\n Left / Right Matched Keypoints:\n\n";

  for (size_t i=0; i < _vleft_kps.size(); ++i)
  {
    std::cout << "- " << _vleft_kps[i] << " | " << _vright_kps[i] << "\n";
  }
}

int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    std::cerr << "\nUsage: ./manual_matching image1_path image2_path\n";
    return -1;
  }

  displayCommands();

  const std::string left_img_path = argv[1];
  const std::string right_img_path = argv[2];

  ManualMatcher manual_matcher(left_img_path, right_img_path);

  std::vector<cv::Point2f> vleft_kps, vright_kps;

  manual_matcher.manualMatching(vleft_kps, vright_kps);

  displayMatchingResults(vleft_kps, vright_kps);

  return 0;
}
