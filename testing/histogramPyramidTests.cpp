#include "catch.hpp"

#include "TestFixture.hpp"
#include "OpenCLManager.hpp"
#include "HistogramPyramids.hpp"
#include "OulConfig.hpp"

namespace test
{

void compileCode(oul::Context &context) {
    context.createProgramFromSource(std::string(OUL_DIR)+"/HistogramPyramids.cl");
}

TEST_CASE("Histogram Pyramid OpenCL code compiles") {
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    oul::Context context = oul::opencl()->createContext(criteria);
    CHECK_NOTHROW(compileCode(context));
}

/*
TEST_CASE("3D Histogram Pyramid Buffer create") {
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    oul::Context context = oul::opencl()->createContext(criteria);
    compileCode(context);
    const int size = 32*32*32;
    char * data = new char[size]();
    cl::Buffer buffer = cl::Buffer(
            context.getContext(),
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(char)*size,
            data
    );
    // TODO refactor this
    OpenCL ocl;
    ocl.context = context.getContext();
    ocl.device = context.getDevice(0);
    ocl.program = context.getProgram(0);
    ocl.queue = context.getQueue(0);
    oul::HistogramPyramid3DBuffer hp(ocl);
    CHECK_NOTHROW(hp.create(buffer, 32, 32, 32));
    delete[] data;
}
*/

TEST_CASE("2D Histogram Pyramid create") {
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    oul::Context context = oul::opencl()->createContext(criteria);
    compileCode(context);
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
    // TODO refactor this
    OpenCL ocl;
    ocl.context = context.getContext();
    ocl.device = context.getDevice(0);
    ocl.program = context.getProgram(0);
    ocl.queue = context.getQueue(0);
    oul::HistogramPyramid2D hp(ocl);
    CHECK_NOTHROW(hp.create(image, sizeX, sizeY));
    delete[] data;
}


TEST_CASE("3D Histogram Pyramid create") {
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    oul::Context context = oul::opencl()->createContext(criteria);
    compileCode(context);
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
    // TODO refactor this
    OpenCL ocl;
    ocl.context = context.getContext();
    ocl.device = context.getDevice(0);
    ocl.program = context.getProgram(0);
    ocl.queue = context.getQueue(0);
    oul::HistogramPyramid3D hp(ocl);
    CHECK_NOTHROW(hp.create(image, sizeX, sizeY, sizeZ));
    delete[] data;
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

TEST_CASE("2D Histogram Pyramid Sum") {
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    oul::Context context = oul::opencl()->createContext(criteria);
    compileCode(context);
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
    // TODO refactor this
    OpenCL ocl;
    ocl.context = context.getContext();
    ocl.device = context.getDevice(0);
    ocl.program = context.getProgram(0);
    ocl.queue = context.getQueue(0);
    oul::HistogramPyramid2D hp(ocl);
    hp.create(image, sizeX, sizeY);

    CHECK(hp.getSum() == correctSum);
}


TEST_CASE("3D Histogram Pyramid Sum") {
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    oul::Context context = oul::opencl()->createContext(criteria);
    compileCode(context);
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
    // TODO refactor this
    OpenCL ocl;
    ocl.context = context.getContext();
    ocl.device = context.getDevice(0);
    ocl.program = context.getProgram(0);
    ocl.queue = context.getQueue(0);
    oul::HistogramPyramid3D hp(ocl);
    hp.create(image, sizeX, sizeY, sizeZ);

    CHECK(hp.getSum() == correctSum);
}

} // end namespace
