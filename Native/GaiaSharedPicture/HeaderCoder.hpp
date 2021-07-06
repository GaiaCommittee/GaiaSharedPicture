#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

namespace Gaia::SharedPicture
{
    /// Header information for a picture.
    struct PictureHeader
    {
        /// Bits count of a pixel.
        enum class PixelBitSizes
        {
            Bits8,
            Bits16,
            Bits32,
            Bits64
        } PixelBits {PixelBitSizes::Bits8};
        /// Value type of a pixel.
        enum class PixelTypes{
            Unsigned,
            Signed,
            Float
        } PixelType {PixelTypes::Unsigned};
        /// Counts of channels.
        unsigned short Channels {3};
        /// Width of the picture.
        unsigned short Width {0};
        /// Height of the picture.
        unsigned short Height {0};
    };

    /**
     * @brief Picture header information coder, providing encode and decode functions.
     */
    class HeaderCoder
    {
    public:
        /**
         * @brief Encode a header into bytes.
         * @param buffer Buffer to store encoded header information, must not be lesser than 10 bytes.
         */
        static void Encode(const PictureHeader& header, unsigned char* buffer);
        /**
         * @brief Decode header bytes into a header instance.
         * @param buffer Buffer with the encoded information to decode, must not be lesser than 10 bytes.
         */
        static bool Decode(const unsigned char* buffer, PictureHeader& header);

        /**
         * @brief Convert the pixel type in header into an OpenCV pixel type.
         * @param header Header of the picture.
         * @return OpenCV pixel type.
         */
        static int GetCVPixelType(const PictureHeader& header);

        /// Get the header from a cv::Mat.
        static PictureHeader GetHeader(const cv::Mat& picture);
    };


}