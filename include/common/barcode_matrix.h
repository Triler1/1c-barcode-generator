#pragma once

#include <cstddef>
#include <vector>
#include <stdexcept>
class BarcodeMatrix {
    public:
    BarcodeMatrix() = default;
    BarcodeMatrix(std::size_t width, std::size_t height, bool initialValue = false) : width_(width), height_(height), data_(width * height, initialValue) {}
    
    [[nodiscard]] std::size_t GetWidth() const noexcept {
        return width_;
    }
    [[nodiscard]] std::size_t GetHeight() const noexcept {
        return height_;
    }
    [[nodiscard]] bool IsEmpty() const noexcept {
        return width_ == 0 || height_ == 0;
    }
    [[nodiscard]] bool Get(std::size_t x, std::size_t y) const {
        CheckBounds(x, y);
        return data_[GetIndex(x, y)];
    }
    void Set(std::size_t x, std::size_t y, bool value) {
        CheckBounds(x, y);
        data_[GetIndex(x, y)] = value;
    }

    private:
    std::size_t width_ = 0;
    std::size_t height_ = 0;
    std::vector<bool> data_;
    [[nodiscard]] std::size_t GetIndex(std::size_t x, std::size_t y) const noexcept {
        return y * width_ + x;
    }
    void CheckBounds(std::size_t x, std::size_t y) const {
        if (x >= width_ || y >= height_) {
            throw std::out_of_range("BarcodeMatrix index out of range");
        }
    }
};