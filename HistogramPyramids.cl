#pragma OPENCL EXTENSION cl_khr_3d_image_writes : enable

__constant sampler_t hpSampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;

__constant int4 cubeOffsets2D[4] = {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {1, 0, 0, 0},
    {1, 1, 0, 0},
};

__constant int4 cubeOffsets[8] = {
    {0, 0, 0, 0},
    {1, 0, 0, 0},
    {0, 0, 1, 0},
    {1, 0, 1, 0},
    {0, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {1, 1, 1, 0},
};

__kernel void constructHPLevel3D(
    __read_only image3d_t readHistoPyramid,
    __write_only image3d_t writeHistoPyramid
    ) { 

    int4 writePos = {get_global_id(0), get_global_id(1), get_global_id(2), 0};
    int4 readPos = writePos*2;
    int writeValue = read_imagei(readHistoPyramid, hpSampler, readPos).x + // 0
    read_imagei(readHistoPyramid, hpSampler, readPos+cubeOffsets[1]).x + // 1
    read_imagei(readHistoPyramid, hpSampler, readPos+cubeOffsets[2]).x + // 2
    read_imagei(readHistoPyramid, hpSampler, readPos+cubeOffsets[3]).x + // 3
    read_imagei(readHistoPyramid, hpSampler, readPos+cubeOffsets[4]).x + // 4
    read_imagei(readHistoPyramid, hpSampler, readPos+cubeOffsets[5]).x + // 5
    read_imagei(readHistoPyramid, hpSampler, readPos+cubeOffsets[6]).x + // 6
    read_imagei(readHistoPyramid, hpSampler, readPos+cubeOffsets[7]).x; // 7

    write_imagei(writeHistoPyramid, writePos, writeValue);
}

__kernel void constructHPLevel2D(
    __read_only image2d_t readHistoPyramid,
    __write_only image2d_t writeHistoPyramid
    ) { 

    int2 writePos = {get_global_id(0), get_global_id(1)};
    int2 readPos = writePos*2;
    int writeValue = 
        read_imagei(readHistoPyramid, hpSampler, readPos).x + 
        read_imagei(readHistoPyramid, hpSampler, readPos+(int2)(1,0)).x + 
        read_imagei(readHistoPyramid, hpSampler, readPos+(int2)(0,1)).x + 
        read_imagei(readHistoPyramid, hpSampler, readPos+(int2)(1,1)).x;

    write_imagei(writeHistoPyramid, writePos, writeValue);
}

int3 scanHPLevel2D(int target, __read_only image2d_t hp, int3 current) {

    int4 neighbors = {
        read_imagei(hp, hpSampler, current.xy).x,
        read_imagei(hp, hpSampler, current.xy + (int2)(0,1)).x,
        read_imagei(hp, hpSampler, current.xy + (int2)(1,0)).x,
        0
    };

    int acc = current.z + neighbors.s0;
    int4 cmp;
    cmp.s0 = acc <= target;
    acc += neighbors.s1;
    cmp.s1 = acc <= target;
    acc += neighbors.s2;
    cmp.s2 = acc <= target;

    current += cubeOffsets2D[(cmp.s0+cmp.s1+cmp.s2)].xyz;
    current.x = current.x*2;
    current.y = current.y*2;
    current.z = current.z +
    cmp.s0*neighbors.s0 +
    cmp.s1*neighbors.s1 +
    cmp.s2*neighbors.s2; 
    return current;

}


int4 scanHPLevel3D(int target, __read_only image3d_t hp, int4 current) {

    int8 neighbors = {
        read_imagei(hp, hpSampler, current).x,
        read_imagei(hp, hpSampler, current + cubeOffsets[1]).x,
        read_imagei(hp, hpSampler, current + cubeOffsets[2]).x,
        read_imagei(hp, hpSampler, current + cubeOffsets[3]).x,
        read_imagei(hp, hpSampler, current + cubeOffsets[4]).x,
        read_imagei(hp, hpSampler, current + cubeOffsets[5]).x,
        read_imagei(hp, hpSampler, current + cubeOffsets[6]).x,
        0
    };

    int acc = current.s3 + neighbors.s0;
    int8 cmp;
    cmp.s0 = acc <= target;
    acc += neighbors.s1;
    cmp.s1 = acc <= target;
    acc += neighbors.s2;
    cmp.s2 = acc <= target;
    acc += neighbors.s3;
    cmp.s3 = acc <= target;
    acc += neighbors.s4;
    cmp.s4 = acc <= target;
    acc += neighbors.s5;
    cmp.s5 = acc <= target;
    acc += neighbors.s6;
    cmp.s6 = acc <= target;


    current += cubeOffsets[(cmp.s0+cmp.s1+cmp.s2+cmp.s3+cmp.s4+cmp.s5+cmp.s6)];
    current.s0 = current.s0*2;
    current.s1 = current.s1*2;
    current.s2 = current.s2*2;
    current.s3 = current.s3 +
    cmp.s0*neighbors.s0 +
    cmp.s1*neighbors.s1 +
    cmp.s2*neighbors.s2 +
    cmp.s3*neighbors.s3 +
    cmp.s4*neighbors.s4 +
    cmp.s5*neighbors.s5 +
    cmp.s6*neighbors.s6; 
    return current;

}

int4 traverseHP3D(
    int target,
    int HP_SIZE,
    image3d_t hp0,
    image3d_t hp1,
    image3d_t hp2,
    image3d_t hp3,
    image3d_t hp4,
    image3d_t hp5,
    image3d_t hp6,
    image3d_t hp7,
    image3d_t hp8,
    image3d_t hp9
    ) {
    int4 position = {0,0,0,0}; // x,y,z,sum
    if(HP_SIZE > 512)
    position = scanHPLevel3D(target, hp9, position);
    if(HP_SIZE > 256)
    position = scanHPLevel3D(target, hp8, position);
    if(HP_SIZE > 128)
    position = scanHPLevel3D(target, hp7, position);
    if(HP_SIZE > 64)
    position = scanHPLevel3D(target, hp6, position);
    if(HP_SIZE > 32)
    position = scanHPLevel3D(target, hp5, position);
    if(HP_SIZE > 16)
    position = scanHPLevel3D(target, hp4, position);
    if(HP_SIZE > 8)
    position = scanHPLevel3D(target, hp3, position);
    position = scanHPLevel3D(target, hp2, position);
    position = scanHPLevel3D(target, hp1, position);
    position = scanHPLevel3D(target, hp0, position);
    position.x = position.x / 2;
    position.y = position.y / 2;
    position.z = position.z / 2;
    return position;
}

int2 traverseHP2D(
    int target,
    int HP_SIZE,
    image2d_t hp0,
    image2d_t hp1,
    image2d_t hp2,
    image2d_t hp3,
    image2d_t hp4,
    image2d_t hp5,
    image2d_t hp6,
    image2d_t hp7,
    image2d_t hp8,
    image2d_t hp9,
    image2d_t hp10,
    image2d_t hp11,
    image2d_t hp12,
    image2d_t hp13
    ) {
    int3 position = {0,0,0};
    if(HP_SIZE > 8192)
    position = scanHPLevel2D(target, hp13, position);
    if(HP_SIZE > 4096)
    position = scanHPLevel2D(target, hp12, position);
    if(HP_SIZE > 2048)
    position = scanHPLevel2D(target, hp11, position);
    if(HP_SIZE > 1024)
    position = scanHPLevel2D(target, hp10, position);
    if(HP_SIZE > 512)
    position = scanHPLevel2D(target, hp9, position);
    if(HP_SIZE > 256)
    position = scanHPLevel2D(target, hp8, position);
    if(HP_SIZE > 128)
    position = scanHPLevel2D(target, hp7, position);
    if(HP_SIZE > 64)
    position = scanHPLevel2D(target, hp6, position);
    if(HP_SIZE > 32)
    position = scanHPLevel2D(target, hp5, position);
    if(HP_SIZE > 16)
    position = scanHPLevel2D(target, hp4, position);
    if(HP_SIZE > 8)
    position = scanHPLevel2D(target, hp3, position);
    position = scanHPLevel2D(target, hp2, position);
    position = scanHPLevel2D(target, hp1, position);
    position = scanHPLevel2D(target, hp0, position);
    position.x = position.x / 2;
    position.y = position.y / 2;
    return position.xy;
}


__kernel void createPositions3D(
        __global int * positions,
        __private int HP_SIZE,
        __private int sum,
        __read_only image3d_t hp0, // Largest HP
        __read_only image3d_t hp1,
        __read_only image3d_t hp2,
        __read_only image3d_t hp3,
        __read_only image3d_t hp4,
        __read_only image3d_t hp5
        ,__read_only image3d_t hp6
        ,__read_only image3d_t hp7
        ,__read_only image3d_t hp8
        ,__read_only image3d_t hp9
    ) {
    int target = get_global_id(0);
    if(target >= sum)
        target = 0;
    int4 pos = traverseHP3D(target,HP_SIZE,hp0,hp1,hp2,hp3,hp4,hp5,hp6,hp7,hp8,hp9);
    vstore3(pos.xyz, target, positions);
}

__kernel void createPositions2D(
        __global int * positions,
        __private int HP_SIZE,
        __private int sum,
        __read_only image2d_t hp0, // Largest HP
        __read_only image2d_t hp1,
        __read_only image2d_t hp2,
        __read_only image2d_t hp3,
        __read_only image2d_t hp4,
        __read_only image2d_t hp5
        ,__read_only image2d_t hp6
        ,__read_only image2d_t hp7
        ,__read_only image2d_t hp8
        ,__read_only image2d_t hp9
        ,__read_only image2d_t hp10
        ,__read_only image2d_t hp11
        ,__read_only image2d_t hp12
        ,__read_only image2d_t hp13
    ) {
    int target = get_global_id(0);
    if(target >= sum)
        target = 0;
    int2 pos = traverseHP2D(target,HP_SIZE,hp0,hp1,hp2,hp3,hp4,hp5,hp6,hp7,hp8,hp9,hp10,hp11,hp12,hp13);
    vstore2(pos, target, positions);
}
