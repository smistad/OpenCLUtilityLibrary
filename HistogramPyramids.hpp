#ifndef HISTOGRAMPYRAMIDS_HPP_
#define HISTOGRAMPYRAMIDS_HPP_

#include "OpenCLManager.hpp"
#include <vector>

namespace oul {

class HistogramPyramid {
    public:
        static void compileCode(oul::Context &context);
        int getSum();
        virtual cl::Buffer createPositionBuffer() = 0;
        virtual void deleteHPlevels() = 0;
    protected:
        oul::Context context;
        int size;
        int sum;
};

class HistogramPyramid2D : public HistogramPyramid {
    public:
        HistogramPyramid2D(oul::Context &context);
        void create(cl::Image2D &image, int, int);
        cl::Buffer createPositionBuffer();
        void deleteHPlevels();
        void traverse(cl::Kernel &kernel, int);
    private:
        std::vector<cl::Image2D> HPlevels;
};

class HistogramPyramid3D : public HistogramPyramid {
    public:
        HistogramPyramid3D(oul::Context &context);
        void create(cl::Image3D &image, int, int, int);
        cl::Buffer createPositionBuffer();
        void deleteHPlevels();
        void traverse(cl::Kernel &kernel, int);
    private:
        std::vector<cl::Image3D> HPlevels;
};

class HistogramPyramid3DBuffer : public HistogramPyramid {
    public:
        HistogramPyramid3DBuffer(oul::Context &context);
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
