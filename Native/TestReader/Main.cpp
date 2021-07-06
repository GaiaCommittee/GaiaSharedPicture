#include <GaiaSharedPicture/GaiaSharedPicture.hpp>

int main()
{
    using namespace Gaia::SharedPicture;

    PictureReader reader("test_picture");

    cv::imshow("test_picture", reader.Read());

    cv::waitKey(0);
    return 0;
}