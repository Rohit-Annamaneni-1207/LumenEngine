#pragma once

#include <cstddef>
#include <stdexcept>

namespace lumen {

    template <typename T>
    class ImageView {
    
    public:
        ImageView(
            T* data,
            std::size_t width,
            std::size_t height,
            std::size_t channels,
            std::size_t row_stride
        ):

        data_(data),
        width_(width),
        height_(height),
        channels_(channels),
        row_stride_(row_stride) {

        if (data_ == nullptr){
            throw std::invalid_argument("Image data cannot be null");
        }

        if (width_ == 0 || height_ == 0 || channels_ == 0) {
            throw std::invalid_argument(
                "image dimensions and channels must be positive"
            );
        }

        if (row_stride_ < width_ * channels_) {
            throw std::invalid_argument(
                "row stride is too small for the image width"
            );
        }
    }


        [[nodiscard]] std::size_t width() const noexcept {
            return width_;
        }

        [[nodiscard]] std::size_t height() const noexcept {
            return height_;
        }

        [[nodiscard]] std::size_t channels() const noexcept {
            return channels_;
        }

        [[nodiscard]] std::size_t row_stride() const noexcept {
            return row_stride_;
        }

        // Memory address for pixel given
        T& at(
            std::size_t x,
            std::size_t y,
            std::size_t channel = 0
        ) const {
            if (x >= width_ || y >= height_ || channel >= channels_)
            {
                throw std::out_of_range("Out of range coordinates");
            }

            return data_[y*row_stride_ + x*channels_ + channel];
        }

    private:
        T* data_;
        std::size_t width_;
        std::size_t height_;
        std::size_t channels_;
        std::size_t row_stride_;

    };
}