#include <cmath>
#include <queue>
#include <algorithm>
#include "SeamCarver.h"

SeamCarver::SeamCarver(Image image)
    : m_image(std::move(image))
{}

const Image& SeamCarver::GetImage() const
{
    return m_image;
}

size_t SeamCarver::GetImageWidth() const
{
    return m_image.m_table.size();
}

size_t SeamCarver::GetImageHeight() const
{
    if (GetImageWidth() > 0) {
        return m_image.m_table[0].size();
    } else {
        return 0;
    }
}

double SeamCarver::GetPixelEnergy(size_t columnId, size_t rowId) const
{
    std::pair<int, int> x_points;
    std::pair<int, int> y_points;
    if (columnId == 0) {
        y_points = {1, GetImageWidth() - 1};
    } else if (columnId == GetImageWidth() - 1) {
        y_points = {0, GetImageWidth() - 2};
    } else {
        y_points = {columnId - 1, columnId + 1};
    }
    if (rowId == 0) {
        x_points = {1, GetImageHeight() - 1};
    } else if (rowId == GetImageHeight()  - 1) {
        x_points = {0, GetImageHeight() - 2};
    } else {
        x_points = {rowId - 1, rowId + 1};
    }
    const Image::Pixel& y1 = m_image.GetPixel(y_points.first, rowId);
    const Image::Pixel& y2 = m_image.GetPixel(y_points.second, rowId);
    const Image::Pixel& x1 = m_image.GetPixel(columnId, x_points.first);
    const Image::Pixel& x2 = m_image.GetPixel(columnId, x_points.second);
    int delta_x = (x1.m_blue - x2.m_blue) * (x1.m_blue - x2.m_blue) +
            (x1.m_green - x2.m_green) * (x1.m_green - x2.m_green) + (x1.m_red - x2.m_red) * (x1.m_red - x2.m_red);
    int delta_y = (y1.m_blue - y2.m_blue) * (y1.m_blue - y2.m_blue) +
            (y1.m_green - y2.m_green) * (y1.m_green - y2.m_green) + (y1.m_red - y2.m_red) * (y1.m_red - y2.m_red);
    return sqrt(delta_x + delta_y);
}

SeamCarver::Seam SeamCarver::FindCommon(size_t max_x, size_t max_y, bool vertical) const {
        Seam result;
        std::vector<double> cur;
        std::vector<double> last;
        std::vector<std::vector<size_t>> table;
        cur.resize(max_x);
        table.resize(max_y);
        last.resize(max_x);
        for (size_t i = 0; i < max_x; ++i) {
            if (vertical) {
                last[i] = GetPixelEnergy(i, 0);
            } else {
                last[i] = GetPixelEnergy(0, i);
            }
            cur[i] = INF;
        }
        for (size_t i = 1; i < max_y; ++i) {
            table[i].resize(max_x);
            for (size_t j = 0; j < max_x; ++j) {
                if (j > 0 && last[j - 1] < cur[j]) {
                    table[i][j] =  j - 1;
                    cur[j] = last[j - 1];
                }
                if (last[j] < cur[j]) {
                    table[i][j] = j;
                    cur[j] = last[j];
                }
                if (j < max_x - 1 && last[j + 1] < cur[j]) {
                    table[i][j] =  j + 1;
                    cur[j] = last[j + 1];
                }
                if (vertical) {
                    cur[j] += GetPixelEnergy(j, i);
                } else {
                    cur[j] += GetPixelEnergy(i, j);
                }
            }
            for (size_t j = 0; j < max_x; ++j) {
                last[j] = cur[j];
                cur[j] = INF;
            }
        }
        size_t j_ans, answer = INF;
        for (size_t j = 0; j < max_x; ++j) {
            if (last[j] < answer) {
                answer = last[j];
                j_ans = j;
            }
        }

        int i = max_y - 1;

        while (i > 0) {
            result.push_back(j_ans);
            j_ans = table[i][j_ans];
            i--;
        }
        result.push_back(j_ans);
        std::reverse(result.begin(), result.end());
        return result;
};

SeamCarver::Seam SeamCarver::FindHorizontalSeam() const
{
    return FindCommon(GetImageHeight(), GetImageWidth(), false);
}

SeamCarver::Seam SeamCarver::FindVerticalSeam() const
{
    return FindCommon(GetImageWidth(), GetImageHeight(), true);
}

void SeamCarver::RemoveHorizontalSeam(const Seam& seam)
{
    for (size_t i = 0; i < seam.size(); ++i) {
        m_image.m_table[i].erase(m_image.m_table[i].begin() + seam[i]);
    }
}

void SeamCarver::RemoveVerticalSeam(const Seam& seam)
{
    for (size_t i = 0; i < seam.size(); ++i) {
        for (size_t j = seam[i]; j < GetImageWidth() - 1; ++j) {
            m_image.m_table[j][i] = m_image.m_table[j + 1][i];
        }
    }
    m_image.m_table.pop_back();
}

