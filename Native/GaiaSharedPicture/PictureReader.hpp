#pragma once

#include <string>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <memory>
#include <opencv2/opencv.hpp>

namespace Gaia::SharedPicture
{
    /**
     * @brief Picture reader provides function to read a picture from a shared memory block.
     */
    class PictureReader
    {
    protected:
        /// Shared memory management object.
        std::unique_ptr<boost::interprocess::shared_memory_object> MemoryObject;
        /// Shared memory accessor object.
        std::unique_ptr<boost::interprocess::mapped_region> RegionObject;

    public:
        /**
         * @brief Open the shared memory block and construct a reader on it.
         * @param shared_block_name Name of the shared memory block.
         */
        explicit PictureReader(const std::string& shared_block_name);

        /// Move constructor.
        PictureReader(PictureReader&& target) noexcept;
        /// Copy constructor.
        PictureReader(const PictureReader& target);

        /// Get the address of the picture buffer memory, which header is not included.
        [[nodiscard]] inline unsigned char* GetPointer() const
        {
            if (RegionObject && RegionObject->get_address())
                return static_cast<unsigned char*>(RegionObject->get_address()) + 10;
            return nullptr;
        }

        /// Get the address of the shared memory.
        [[nodiscard]] inline unsigned char* GetMemoryPointer()
        {
            if (RegionObject && RegionObject->get_address())
                return static_cast<unsigned char*>(RegionObject->get_address());
            return nullptr;
        }

        /// Get the size of the shared memory.
        [[nodiscard]] inline std::size_t GetMemorySize()
        {
            if (RegionObject) return RegionObject->get_size();
            return 0;
        }

        /**
         * @brief Read a picture from the connected shared memory block.
         * @throws runtime_error If failed to decode header information or memory size is smaller than
         *                       size needed according to the header.
         * @return Picture in the shared memory block.
         */
        cv::Mat Read();
    };
}