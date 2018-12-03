/**
 * @file ECImageProvider.cpp
 * @author Felix Thielke
 * @author <a href="mailto:jesse@tzi.de">Jesse Richter-Klug</a>
 */

#include "ECImageProvider.h"
#include "Tools/Debugging/DebugImages.h"
#include "Tools/ImageProcessing/YHS2SimpleConversion.h"
#define PREFETCH

using namespace PixelTypes;

MAKE_MODULE(ECImageProvider, perception)

void ECImageProvider::update(ECImage& ecImage)
{
  COMPLEX_IMAGE("HuedImage")
    update<true>(ecImage);
  else if(hueIsNeeded)
    update<true>(ecImage);
  else
    update<false>(ecImage);
}

template<bool saveHue>
void ECImageProvider::update(ECImage& ecImage)
{
  ecImage.grayscaled.setResolution(theCameraInfo.width, theCameraInfo.height);
  ecImage.colored.setResolution(theCameraInfo.width, theCameraInfo.height);

  cv::Mat imgGrayScale;
  cv::Mat imgBlurred;
  cv::Mat imgCanny;

  int lowThreshold = 45;
  int highThreshold = 90;

  cv::GaussianBlur(theImage.cv_image, imgBlurred, cv::Size(5,5), 1.8);
  cv::Canny(imgBlurred, imgCanny, lowThreshold, highThreshold);

  cv::imwrite("/home/oluwayetty/Desktop/Robocup/raw_image.jpg",theImage.cv_image); //Image from robot camera
  cv::imwrite("/home/oluwayetty/Desktop/Robocup/raw_image_edge.jpg", imgCanny); //edges contained in the image

  if(theImage.timeStamp > 10 && theImage.width == theCameraInfo.width / 2)
  {
    ecImage.ued.setResolution(theCameraInfo.width / 2, theCameraInfo.height);
    ecImage.ved.setResolution(theCameraInfo.width / 2, theCameraInfo.height);
    COMPLEX_IMAGE("SaturatedImage") ecImage.saturated.setResolution(theCameraInfo.width, theCameraInfo.height);
    if(saveHue) ecImage.hued.setResolution(theCameraInfo.width, theCameraInfo.height);

    ASSERT((theCameraInfo.width * theCameraInfo.height) % 32 == 0);
    ASSERT(theImage.width != theCameraInfo.width);

    COMPLEX_IMAGE("SaturatedImage")
      YHS2s::updateSSE<false /* calc all colors */, true /* save grayscaled */, true /* save colorchanneled */, true, saveHue>(reinterpret_cast<const PixelTypes::YUYVPixel* const>(theImage[0]), theImage.width, theImage.height * 2, theFieldColors, ecImage.grayscaled, ecImage.ued, ecImage.ved, ecImage.colored, ecImage.hued, ecImage.saturated);
    else
      YHS2s::updateSSE<false /* calc all colors */, true/* save grayscaled */, true /* save colorchanneled */, false, saveHue>(reinterpret_cast<const PixelTypes::YUYVPixel* const>(theImage[0]), theImage.width, theImage.height * 2, theFieldColors, ecImage.grayscaled, ecImage.ued, ecImage.ved, ecImage.colored, ecImage.hued, ecImage.saturated);

    ecImage.timeStamp = theImage.timeStamp;
  }
}
