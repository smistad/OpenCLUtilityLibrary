#ifndef HISTOGRAMPYRAMIDS_HPP_
#define HISTOGRAMPYRAMIDS_HPP_

#include "openclutilitylibrary_export.h"

#include "Context.hpp"
#include <vector>

namespace oul
{

/**
 * Abstract class for all HistogramPyramids
 */
class OPENCLUTILITYLIBRARY_EXPORT HistogramPyramid {
    public:
        static void compileCode(oul::Context &context, std::string kernel_dir);
        int getSum();
        virtual cl::Buffer createPositionBuffer() = 0;
        virtual void deleteHPlevels() = 0;
    protected:
        oul::Context context; //this will call the default constructor in Context
        int size;
        int sum;
};

/**
 * 2D Histogram pyramid
 */
class OPENCLUTILITYLIBRARY_EXPORT HistogramPyramid2D : public HistogramPyramid {
    public:
        HistogramPyramid2D(oul::Context &context, std::string kernel_dir);
        void create(cl::Image2D &image, int, int);
        cl::Buffer createPositionBuffer();
        void deleteHPlevels();
        void traverse(cl::Kernel &kernel, int);
    private:
        std::vector<cl::Image2D> HPlevels;
};

/**
 * 3D Histogram pyramid (that uses 3D textures)
 */
class OPENCLUTILITYLIBRARY_EXPORT HistogramPyramid3D : public HistogramPyramid {
    public:
        HistogramPyramid3D(oul::Context &context, std::string kernel_dir);
        void create(cl::Image3D &image, int, int, int);
        cl::Buffer createPositionBuffer();
        void deleteHPlevels();
        void traverse(cl::Kernel &kernel, int);
    private:
        std::vector<cl::Image3D> HPlevels;
};

/**
 * 3D Histogram pyramid (which uses buffers instead of textures)
 */
class OPENCLUTILITYLIBRARY_EXPORT HistogramPyramid3DBuffer : public HistogramPyramid {
    public:
        HistogramPyramid3DBuffer(oul::Context &context, std::string kernel_dir);
        void create(cl::Buffer &buffer, int, int, int);
        cl::Buffer createPositionBuffer();
        void deleteHPlevels();
        void traverse(cl::Kernel &kernel, int);
    private:
        int sizeX,sizeY,sizeZ;
        std::vector<cl::Buffer> HPlevels;
};
} // end namespace

#endif /* HISTOGRAMPYRAMIDS_HPP_ */
