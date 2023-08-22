# nbody-stdpar
Simple Nbody gravitational simulator utilizing C++ Standard Parallelism.

Build this project by invoking the shell script in the top-level source directory:
sh build_nbody.sh

Toggle the ENABLE_NVIDIA_GPU option in the script to switch between building for CPU or GPU.
Setting ENABLE_NVIDIA_GPU:BOOL=ON requires a Nvidia GPU, CUDA, and the Nvidia HPC SDK.
Setting ENABLE_NVIDIA_GPU:BOOL=OFF requires the Intel TBB library to be available.
