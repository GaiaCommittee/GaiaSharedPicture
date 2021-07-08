#include "PictureReader.hpp"
#include "HeaderCoder.hpp"

namespace Gaia::SharedPicture
{
    /// Open the shared memory block and construct a reader on it.
    PictureReader::PictureReader(const std::string &shared_block_name)
    {
        try
        {
            MemoryObject = std::make_unique<boost::interprocess::shared_memory_object>(
                    boost::interprocess::open_only, shared_block_name.c_str(),
                    boost::interprocess::read_only);
            RegionObject = std::make_unique<boost::interprocess::mapped_region>(
                    *MemoryObject,
                    boost::interprocess::read_only);
        }catch(std::exception& error)
        {
            throw std::runtime_error(std::string("Failed to open shared picture:") + error.what());
        }

    }

    /// Copy constructor.
    PictureReader::PictureReader(const PictureReader &target)
    {
        if (target.MemoryObject)
        {
            MemoryObject = std::make_unique<boost::interprocess::shared_memory_object>(
                    boost::interprocess::open_only, target.MemoryObject->get_name(),
                    boost::interprocess::read_only);
            RegionObject = std::make_unique<boost::interprocess::mapped_region>(
                    *MemoryObject,
                    boost::interprocess::read_only);
        }
    }

    /// Move constructor.
    PictureReader::PictureReader(PictureReader&& target) noexcept:
        MemoryObject(std::move(target.MemoryObject)), RegionObject(std::move(target.RegionObject))
    {}

    /// Read a picture from the shared memory.
    cv::Mat PictureReader::Read()
    {
        if (RegionObject && RegionObject->get_address())
        {
            PictureHeader header;
            if (HeaderCoder::Decode(static_cast<unsigned char *>(RegionObject->get_address()), header))
            {
                int byte_scale_factor = 1;
                switch (header.PixelBits)
                {
                    case PictureHeader::PixelBitSizes::Bits8:
                        byte_scale_factor = 1;
                        break;
                    case PictureHeader::PixelBitSizes::Bits16:
                        byte_scale_factor = 2;
                        break;
                    case PictureHeader::PixelBitSizes::Bits32:
                        byte_scale_factor = 4;
                        break;
                    case PictureHeader::PixelBitSizes::Bits64:
                        byte_scale_factor = 8;
                        break;
                }
                if (RegionObject->get_size() < header.Width * header.Height * header.Channels * byte_scale_factor)
                {
                    throw std::runtime_error("Failed to read picture, "
                                             "insufficient shared memory for picture bytes described in header.");
                }
                return cv::Mat(cv::Size(header.Width, header.Height), HeaderCoder::GetCVPixelType(header),
                               GetPointer());
            }
            else
            {
                throw std::runtime_error("Failed to read picture, header information decoding failed.");
            }
        }
        else
        {
            throw std::runtime_error("Failed to read picture, memory block is not opened.");
        }
    }
}