#include "catch.hpp"

#include "TestFixture.hpp"
#include "OpenCLManager.hpp"
#include "HistogramPyramids.hpp"
#include "OulConfig.hpp"

namespace test
{

void compileCode(oul::Context &context) {
    context.createProgramFromSource(std::string(OUL_DIR)+"/HistogramPyramids.cl", "-I " + std::string(OUL_DIR));
}

TEST_CASE("Histogram Pyramid OpenCL code compiles") {
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    oul::Context context = oul::opencl()->createContext(criteria);
    CHECK_NOTHROW(compileCode(context));
}

TEST_CASE("3D Histogram Pyramid Buffer create") {
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    oul::Context context = oul::opencl()->createContext(criteria);
    int sizeX = 64;
    int sizeY = 64;
    int sizeZ = 64;
    const int size = sizeX*sizeY*sizeZ;
    unsigned char * data = new unsigned char[size]();
    cl::Buffer buffer = cl::Buffer(
            context.getContext(),
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(char)*size,
            data
    );
    delete[] data;
    oul::HistogramPyramid3DBuffer hp(context);
    CHECK_NOTHROW(hp.create(buffer, sizeX, sizeY, sizeZ));
}

TEST_CASE("2D Histogram Pyramid create") {
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    oul::Context context = oul::opencl()->createContext(criteria);
    int sizeX = 32;
    int sizeY = 32;
    unsigned char * data = new unsigned char[sizeX*sizeY]();
    cl::Image2D image = cl::Image2D(
            context.getContext(),
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            cl::ImageFormat(CL_R, CL_UNSIGNED_INT8),
            sizeX, sizeY,
            0,
            data
    );
    oul::HistogramPyramid2D hp(context);
    CHECK_NOTHROW(hp.create(image, sizeX, sizeY));
    delete[] data;
}


TEST_CASE("3D Histogram Pyramid create") {
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    oul::Context context = oul::opencl()->createContext(criteria);
    int sizeX = 32;
    int sizeY = 32;
    int sizeZ = 32;
    unsigned char * data = new unsigned char[sizeX*sizeY*sizeZ]();
    cl::Image3D image = cl::Image3D(
            context.getContext(),
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            cl::ImageFormat(CL_R, CL_UNSIGNED_INT8),
            sizeX, sizeY, sizeZ,
            0, 0,
            data
    );
    delete[] data;
    oul::HistogramPyramid3D hp(context);
    CHECK_NOTHROW(hp.create(image, sizeX, sizeY, sizeZ));
}

unsigned char * createRandomData(unsigned int size, unsigned int * sum) {
    unsigned char * data = new unsigned char[size];
    // Populate with random data
    srand(time(NULL));
    unsigned int correctSum = 0;
    for(int i = 0; i < size; i++) {
        unsigned char value = rand()%2;
        data[i] = value;
        correctSum += value;
    }

    *sum = correctSum;

    return data;
}

TEST_CASE("3D Histogram Pyramid Buffer Sum") {
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    oul::Context context = oul::opencl()->createContext(criteria);
    unsigned int sizeX = 64;
    unsigned int sizeY = 64;
    unsigned int sizeZ = 64;
    unsigned int size = sizeX*sizeY*sizeZ;
    unsigned int correctSum = 0;
    unsigned char * data = createRandomData(size, &correctSum);
    cl::Buffer buffer = cl::Buffer(
            context.getContext(),
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(char)*size,
            data
    );
    delete[] data;
    oul::HistogramPyramid3DBuffer hp(context);
    hp.create(buffer, sizeX, sizeY, sizeZ);

    CHECK(hp.getSum() == correctSum);
}


TEST_CASE("2D Histogram Pyramid Sum") {
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    oul::Context context = oul::opencl()->createContext(criteria);
    int sizeX = 64;
    int sizeY = 64;
    unsigned int correctSum = 0;
    unsigned char * data = createRandomData(sizeX*sizeY, &correctSum);

    cl::Image2D image = cl::Image2D(
            context.getContext(),
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            cl::ImageFormat(CL_R, CL_UNSIGNED_INT8),
            sizeX, sizeY,
            0,
            data
    );
    delete[] data;
    oul::HistogramPyramid2D hp(context);
    hp.create(image, sizeX, sizeY);

    CHECK(hp.getSum() == correctSum);
}


TEST_CASE("3D Histogram Pyramid Sum") {
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    oul::Context context = oul::opencl()->createContext(criteria);
    int sizeX = 64;
    int sizeY = 64;
    int sizeZ = 64;
    unsigned int correctSum = 0;
    unsigned char * data = createRandomData(sizeX*sizeY*sizeZ, &correctSum);

    cl::Image3D image = cl::Image3D(
            context.getContext(),
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            cl::ImageFormat(CL_R, CL_UNSIGNED_INT8),
            sizeX, sizeY, sizeZ,
            0, 0,
            data
    );
    delete[] data;
    oul::HistogramPyramid3D hp(context);
    hp.create(image, sizeX, sizeY, sizeZ);

    CHECK(hp.getSum() == correctSum);
}

} // end namespace
