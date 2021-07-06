#include "PictureWriter.hpp"
#include "HeaderCoder.hpp"

namespace Gaia::SharedPicture
{

    /// Delete the memory of a writer if it owns that memory.
    void PictureWriter::Release()
    {
        if (OwnedMemory)
        {
            if (MemoryObject) boost::interprocess::shared_memory_object::remove(MemoryObject->get_name());
            RegionObject.reset();
            MemoryObject.reset();
        }
    }

    /// Connect to the shared memory block.
    PictureWriter::PictureWriter(const std::string& shared_block_name, unsigned int max_size, bool create):
        MaxSize(max_size), OwnedMemory(!create)
    {
        MemoryObject = std::make_unique<boost::interprocess::shared_memory_object>(
                boost::interprocess::open_or_create, shared_block_name.c_str(),
                boost::interprocess::read_write);
        if (create)
        {
            MemoryObject->truncate(max_size + 10);
        }
        RegionObject = std::make_unique<boost::interprocess::mapped_region>(
                *MemoryObject,
                boost::interprocess::read_write);
    }

    /// Copy constructor.
    PictureWriter::PictureWriter(PictureWriter &&target) noexcept:
        MaxSize(target.MaxSize), OwnedMemory(target.OwnedMemory),
        MemoryObject(std::move(target.MemoryObject)), RegionObject(std::move(target.RegionObject))
    {}

    PictureWriter::PictureWriter(const PictureWriter &target):
        MaxSize(target.MaxSize), OwnedMemory(false)
    {
        if (target.MemoryObject)
        {
            MemoryObject = std::make_unique<boost::interprocess::shared_memory_object>(
                    boost::interprocess::open_only, target.MemoryObject->get_name(),
                    boost::interprocess::read_write);
            RegionObject = std::make_unique<boost::interprocess::mapped_region>(
                    *MemoryObject,
                    boost::interprocess::read_write);
        }
    }

    /// Release the memory if it owns the memory.
    PictureWriter::~PictureWriter()
    {
        Release();
    }

    /// Write a cv::Mat into shared memory block.
    bool PictureWriter::Write(const cv::Mat &picture)
    {
        if (RegionObject && RegionObject->get_address())
        {
            if (GetMaxSize() < picture.elemSize())
            {
                throw std::runtime_error("Insufficient shared memory space for the picture to write: "
                    + std::to_string(GetMaxSize()) + " bytes for "
                    + std::to_string(picture.elemSize()) + " bytes.");
            }
            HeaderCoder::Encode(HeaderCoder::GetHeader(picture),
                                static_cast<unsigned char *>(RegionObject->get_address()));
            cv::Mat destination(cv::Size(picture.cols, picture.rows), picture.type(), GetPointer());
            picture.copyTo(destination);
            return true;
        }
        return false;
    }

    /// Set header data of the memory owned by this writer.
    void PictureWriter::SetHeader(const PictureHeader &header)
    {
        if (!RegionObject) throw std::runtime_error("Failed to set header: shared memory has not been opened.");
        HeaderCoder::Encode(header, static_cast<unsigned char *>(RegionObject->get_address()));
    }
}