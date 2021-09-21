[![Build](https://github.com/subski/neat_c-gpu/workflows/CMake/badge.svg)](https://github.com/subski/neat_c-gpu/actions)
## 1. How to build (Linux & Windows)

### 1.1. Install OpenCL (Required)
- (**Linux**) Install via the commands: 
  - `sudo apt-get install ocl-icd-opencl-dev ocl-icd-libopencl1`
  - `sudo apt-get install opencl-headers ocl-icd-dev ocl-icd-libopencl1`
- (**Windows**) [Download CUDA here](https://developer.nvidia.com/cuda-downloads).

1. Build the CMake project with the command: `cmake -B build/`
2. Build the binaries with the command:      `cmake --build build/`

## 2. How to run

Execute the `NEAT_C-gpu` binary in the `build/bin/` folder.