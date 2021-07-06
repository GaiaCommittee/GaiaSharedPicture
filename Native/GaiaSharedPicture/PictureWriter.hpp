#pragma once

#include <string>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <memory>
#include <opencv2/opencv.hpp>

namespace Gaia::SharedPicture
{
    /**
     * @brief Picture writer provides function to write a picture into a shared memory block.
     * @details
     *  It also provides basic shared memory management operations.
     */
    class PictureWriter
    {
    private:
        /// The max capacity of the shared memory picture part.
        const unsigned int MaxSize;
        /// Whether this writer owned the memory or not.
        const bool OwnedMemory;

    protected:
        /// Shared memory management object.
        std::unique_ptr<boost::interprocess::shared_memory_object> MemoryObject;
        /// Shared memory accessor object.
        std::unique_ptr<boost::interprocess::mapped_region> RegionObject;

    public:
        /// Release the memory if it owns the memory.
        virtual ~PictureWriter();

        /**
         * @brief Create or open the shared memory block and construct a writer on it.
         * @param shared_block_name Name of the shared memory block.
         * @param max_size Size of the memory block.
         * @param create If false, then only try to open the existing memory block.
         */
        PictureWriter(const std::string& shared_block_name, unsigned int max_size, bool create = true);

        /// Move constructor.
        PictureWriter(PictureWriter&& target) noexcept;
        /// Copy constructor.
        PictureWriter(const PictureWriter& target);

        /// Get the max size of the space for picture in shared memory block.
        [[nodiscard]] inline const unsigned int& GetMaxSize() const noexcept
        {
            return MaxSize;
        }
        /// Get the address of the picture buffer memory, which header is not included.
        [[nodiscard]] inline unsigned char* GetPointer() const
        {
            if (RegionObject && RegionObject->get_address())
                return static_cast<unsigned char*>(RegionObject->get_address()) + 10;
            return nullptr;
        }

        /**
         * @brief Write a picture into the connected shared memory block.
         * @param picture Picture to write down.
         * @retval true Successfully written.
         * @retval false Failed to write.
         * @throws runtime_error If size of picture is bigger than max size.
         */
        bool Write(const cv::Mat& picture);

        /// Release the memory block if it owns the memory.
        void Release();
    };
}