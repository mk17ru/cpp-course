#pragma once

#include "Image.h"

class SeamCarver
{
    using Seam = std::vector<size_t>;

public:

    constexpr static const  double INF = std::numeric_limits<int>::max();

    SeamCarver(Image image);

    /**
     * Returns current image
     */
    const Image& GetImage() const;

    /**
     * Gets current image width
     */
    size_t GetImageWidth() const;

    /**
     * Gets current image height
     */
    size_t GetImageHeight() const;

    /**
     * Returns pixel energy
     * @param columnId column index (x)
     * @param rowId row index (y)
     */
    double GetPixelEnergy(size_t columnId, size_t rowId) const;

    /**
     * Returns sequence of pixel row indexes (y)
     * (x indexes are [0:W-1])
     */
    Seam FindHorizontalSeam() const;

    /**
     * Returns sequence of pixel column indexes (x)
     * (y indexes are [0:H-1])
     */
    Seam FindVerticalSeam() const;

    /**
     * Removes sequence of pixels from the image
     */
    void RemoveHorizontalSeam(const Seam& seam);

    /**
     * Removes sequence of pixes from the image
     */
    void RemoveVerticalSeam(const Seam& seam);

    Seam FindCommon(size_t max_x, size_t max_y, bool vertical) const;

private:
    Image m_image;
};