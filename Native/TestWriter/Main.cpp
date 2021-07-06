#include <GaiaSharedPicture/GaiaSharedPicture.hpp>
#include <iostream>

int main()
{
    using namespace Gaia::SharedPicture;

    auto test_picture = cv::imread("test.png");

    PictureWriter writer("test_picture", 1920*1080*3);

    writer.Write(test_picture);

    char key;
    std::cin >> key;

    writer.Release();
    return 0;
}