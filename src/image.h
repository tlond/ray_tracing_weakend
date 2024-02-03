#pragma once

#include <span>
#include <memory>

template <class T, size_t Channels>
struct Image
{
    int width;
    int height;
    static constexpr auto numChannels = Channels;
    std::unique_ptr<T[]> data;

    Image(int _width, int _height)
        : width(_width), height(_height)
    {
        const auto imageDataSize = static_cast<size_t>(width) * height * numChannels;
        data = std::make_unique<T[]>(imageDataSize);
    }

    struct RowProxy
    {
        int width;
        int numChannels;
        T* rowData;

        RowProxy() : width(0), numChannels(0), rowData(nullptr) {}
        RowProxy(int _width, int _numChannels, T* _rowData)
            : width(_width), numChannels(_numChannels), rowData(_rowData) {
        }

        T& operator[](int colIndex) {
            return rowData[colIndex * numChannels];
        }

        const T& operator[](int colIndex) const {
            return rowData[colIndex * numChannels];
        }

        auto operator()() {
            return std::span<T>(rowData, width * numChannels, * sizeof(T));
        }
    };

    RowProxy operator[](int rowIndex) {
        T* rowData = data.get() + (rowIndex * width * numChannels * sizeof(T));
        return RowProxy(width, numChannels, rowData);
    }

    RowProxy operator[](int rowIndex) const
    {
        T *rowData = data.get() + (rowIndex * width * numChannels);
        return RowProxy(width, numChannels, rowData);
    }

    Vec2i size() const
    {
        return {width, height};
    }
    // Add member functions to access/manipulate image data if needed
};