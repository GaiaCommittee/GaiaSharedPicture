#include "HeaderCoder.hpp"

#include <opencv2/opencv.hpp>

namespace Gaia::SharedPicture
{
    /// Encode a header into bytes.
    void HeaderCoder::Encode(const PictureHeader &header, unsigned char *buffer)
    {
        buffer[0] = 0xBE;
        switch (header.PixelBits)
        {
            case PictureHeader::PixelBitSizes::Bits8:
                buffer[1] = 8;
                break;
            case PictureHeader::PixelBitSizes::Bits16:
                buffer[1] = 16;
                break;
            case PictureHeader::PixelBitSizes::Bits32:
                buffer[1] = 32;
                break;
            case PictureHeader::PixelBitSizes::Bits64:
                buffer[1] = 64;
                break;
        }
        switch (header.PixelType)
        {
            case PictureHeader::PixelTypes::Unsigned:
                buffer[2] = 0;
                break;
            case PictureHeader::PixelTypes::Signed:
                buffer[2] = 1;
                break;
            case PictureHeader::PixelTypes::Float:
                buffer[2] = 2;
                break;
        }
        *(reinterpret_cast<unsigned short*>(&buffer[3])) = header.Channels;
        *(reinterpret_cast<unsigned short*>(&buffer[5])) = header.Width;
        *(reinterpret_cast<unsigned short*>(&buffer[7])) = header.Height;
        buffer[9] = 0xED;
    }

    /// Decode header bytes into a header instance.
    bool HeaderCoder::Decode(const unsigned char *buffer, PictureHeader &header)
    {
        if (buffer[0] != 0xBE || buffer[9] != 0xED) return false;

        switch (buffer[1])
        {
            case 8:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits8;
                break;
            case 16:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits16;
                break;
            case 32:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits32;
                break;
            case 64:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits64;
                break;
            default:
                return false;
        }
        switch (buffer[2])
        {
            case 0:
                header.PixelType = PictureHeader::PixelTypes::Unsigned;
                break;
            case 1:
                header.PixelType = PictureHeader::PixelTypes::Signed;
                break;
            case 2:
                header.PixelType = PictureHeader::PixelTypes::Float;
                break;
            default:
                return false;
        }
        header.Channels = *(reinterpret_cast<const unsigned short*>(&buffer[3]));
        header.Width = *(reinterpret_cast<const unsigned short*>(&buffer[5]));
        header.Height = *(reinterpret_cast<const unsigned short*>(&buffer[7]));
        return true;
    }

    /// Convert the pixel type in header into an OpenCV pixel type.
    int HeaderCoder::GetCVPixelType(const PictureHeader &header)
    {
        switch (header.PixelBits)
        {
            case PictureHeader::PixelBitSizes::Bits8:
                switch (header.PixelType)
                {
                    case PictureHeader::PixelTypes::Unsigned:
                        return CV_8UC(header.Channels);
                    case PictureHeader::PixelTypes::Signed:
                        return CV_8SC(header.Channels);
                    default:
                        return -1;
                }
            case PictureHeader::PixelBitSizes::Bits16:
                switch (header.PixelType)
                {
                    case PictureHeader::PixelTypes::Unsigned:
                        return CV_16UC(header.Channels);
                    case PictureHeader::PixelTypes::Signed:
                        return CV_16SC(header.Channels);
                    case PictureHeader::PixelTypes::Float:
                        return CV_16FC(header.Channels);
                }
            case PictureHeader::PixelBitSizes::Bits32:
                switch (header.PixelType)
                {
                    case PictureHeader::PixelTypes::Signed:
                        return CV_32SC(header.Channels);
                    case PictureHeader::PixelTypes::Float:
                        return CV_32FC(header.Channels);
                    default:
                        return -1;
                }
            case PictureHeader::PixelBitSizes::Bits64:
                switch (header.PixelType)
                {
                    case PictureHeader::PixelTypes::Float:
                        return CV_64FC(header.Channels);
                    default:
                        return -1;
                }
        }
    }

    /// Get the header from a cv::Mat.
    PictureHeader HeaderCoder::GetHeader(const cv::Mat &picture)
    {
        PictureHeader header;

        // Code generated from python.
        switch (picture.type())
        {
            case CV_8UC1:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits8;
                header.PixelType = PictureHeader::PixelTypes::Unsigned;
                header.Channels = 1;
                break;
            case CV_8UC2:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits8;
                header.PixelType = PictureHeader::PixelTypes::Unsigned;
                header.Channels = 2;
                break;
            case CV_8UC3:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits8;
                header.PixelType = PictureHeader::PixelTypes::Unsigned;
                header.Channels = 3;
                break;
            case CV_8UC4:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits8;
                header.PixelType = PictureHeader::PixelTypes::Unsigned;
                header.Channels = 4;
                break;
            case CV_8SC1:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits8;
                header.PixelType = PictureHeader::PixelTypes::Signed;
                header.Channels = 1;
                break;
            case CV_8SC2:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits8;
                header.PixelType = PictureHeader::PixelTypes::Signed;
                header.Channels = 2;
                break;
            case CV_8SC3:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits8;
                header.PixelType = PictureHeader::PixelTypes::Signed;
                header.Channels = 3;
                break;
            case CV_8SC4:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits8;
                header.PixelType = PictureHeader::PixelTypes::Signed;
                header.Channels = 4;
                break;
            case CV_16UC1:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits16;
                header.PixelType = PictureHeader::PixelTypes::Unsigned;
                header.Channels = 1;
                break;
            case CV_16UC2:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits16;
                header.PixelType = PictureHeader::PixelTypes::Unsigned;
                header.Channels = 2;
                break;
            case CV_16UC3:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits16;
                header.PixelType = PictureHeader::PixelTypes::Unsigned;
                header.Channels = 3;
                break;
            case CV_16UC4:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits16;
                header.PixelType = PictureHeader::PixelTypes::Unsigned;
                header.Channels = 4;
                break;
            case CV_16SC1:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits16;
                header.PixelType = PictureHeader::PixelTypes::Signed;
                header.Channels = 1;
                break;
            case CV_16SC2:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits16;
                header.PixelType = PictureHeader::PixelTypes::Signed;
                header.Channels = 2;
                break;
            case CV_16SC3:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits16;
                header.PixelType = PictureHeader::PixelTypes::Signed;
                header.Channels = 3;
                break;
            case CV_16SC4:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits16;
                header.PixelType = PictureHeader::PixelTypes::Signed;
                header.Channels = 4;
                break;
            case CV_16FC1:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits16;
                header.PixelType = PictureHeader::PixelTypes::Float;
                header.Channels = 1;
                break;
            case CV_16FC2:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits16;
                header.PixelType = PictureHeader::PixelTypes::Float;
                header.Channels = 2;
                break;
            case CV_16FC3:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits16;
                header.PixelType = PictureHeader::PixelTypes::Float;
                header.Channels = 3;
                break;
            case CV_16FC4:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits16;
                header.PixelType = PictureHeader::PixelTypes::Float;
                header.Channels = 4;
                break;
            case CV_32SC1:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits32;
                header.PixelType = PictureHeader::PixelTypes::Signed;
                header.Channels = 1;
                break;
            case CV_32SC2:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits32;
                header.PixelType = PictureHeader::PixelTypes::Signed;
                header.Channels = 2;
                break;
            case CV_32SC3:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits32;
                header.PixelType = PictureHeader::PixelTypes::Signed;
                header.Channels = 3;
                break;
            case CV_32SC4:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits32;
                header.PixelType = PictureHeader::PixelTypes::Signed;
                header.Channels = 4;
                break;
            case CV_32FC1:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits32;
                header.PixelType = PictureHeader::PixelTypes::Float;
                header.Channels = 1;
                break;
            case CV_32FC2:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits32;
                header.PixelType = PictureHeader::PixelTypes::Float;
                header.Channels = 2;
                break;
            case CV_32FC3:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits32;
                header.PixelType = PictureHeader::PixelTypes::Float;
                header.Channels = 3;
                break;
            case CV_32FC4:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits32;
                header.PixelType = PictureHeader::PixelTypes::Float;
                header.Channels = 4;
                break;
            case CV_64FC1:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits64;
                header.PixelType = PictureHeader::PixelTypes::Float;
                header.Channels = 1;
                break;
            case CV_64FC2:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits64;
                header.PixelType = PictureHeader::PixelTypes::Float;
                header.Channels = 2;
                break;
            case CV_64FC3:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits64;
                header.PixelType = PictureHeader::PixelTypes::Float;
                header.Channels = 3;
                break;
            case CV_64FC4:
                header.PixelBits = PictureHeader::PixelBitSizes::Bits64;
                header.PixelType = PictureHeader::PixelTypes::Float;
                header.Channels = 4;
                break;
        }
        header.Width = picture.cols;
        header.Height = picture.rows;

        return header;
    }
}