//*LB*
// Copyright (c) 2010, University of Bonn, Institute for Computer Science VI
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//  * Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//  * Neither the name of the University of Bonn 
//    nor the names of its contributors may be used to endorse or promote
//    products derived from this software without specific prior written
//    permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*LE*





/**
 *
 *
 * @mainpage
 *
 * @section summary  Summary
 *
 * CUV is a C++ template and Python library which makes it easy to use NVIDIA(tm) CUDA.
 *
 * @section features  Features
 *
 * Supported Platforms:
 * - This library was only tested on Ubuntu Karmic, Lucid and Maverick. It uses only standard
 *   components and should run without major modification on any current linux
 *   system.
 *
 * Supported GPUs:
 * - All GT 9800 and GTX 280 and above
 * - GT 9200 without convolutions. It might need some minor modifications to make the rest work.
 *   If you want to use that card and have problems, just get in contact.
 * - On 8800GTS, random numbers and convolutions wont work.
 * 
 * Structure: 
 * - Like for example Matlab, CUV assumes that everything is an n-dimensional matrix called "tensor"
 * - Tensors can have an arbitrary data-type and can be on the host (CPU-memory) or device (GPU-memory)
 * - Tensors can be column-major or row-major (1-dimensional tensors are, by convention, row-major)
 * - The library defines many functions which may or may not apply to all possible combinations. Variations are easy to add.
 * - For convenience, we also wrap some of the functionality provided by Alex
 *   Krizhevsky on his website (http://www.cs.utoronto.ca/~kriz/) with
 *   permission. Thanks Alex for providing your code!
 *
 * Python Integration 
 * - CUV plays well with python and numpy. That is, once you wrote your fast
 *   GPU functions in CUDA/C++, you can export them using Boost.Python. You can
 *   use Numpy for pre-processing and fancy stuff you have not yet implemented,
 *   then push the Numpy-matrix to the GPU, run your operations there, pull
 *   again to CPU and visualize using matplotlib. Great.
 *
 * Implemented Functionality
 * - Simple Linear Algebra for dense vectors and matrices (BLAS level 1,2,3)
 * - Helpful functors and abstractions
 * - Sparse matrices in DIA format and matrix-multiplication for these matrices
 * - I/O functions using boost.serialization
 * - Fast Random Number Generator
 * - Up to now, CUV was used to build dense and sparse Neural Networks and
 *   Restricted Boltzmann Machines (RBM), convolutional or locally connected.
 * 
 * Documentation
 * - Tutorials are available on
 *   http://www.ais.uni-bonn.de/~schulz/tag/cuv
 * - The documentation can be generated from the code or accessed on the internet:
 *   http://www.ais.uni-bonn.de/deep_learning/doc/html/index.html
 *
 * Contact
 * - We are eager to help you getting started with CUV and improve the library continuously!
 *   http://www.ais.uni-bonn.de/deep_learning/index.html
 *
 * @section installation  Installation
 *
 * @subsection req  Requirements
 *
 * For C++ libs, you will need:
 * - libboost-dev >= 1.37
 * - libblas-dev
 * - libtemplate-perl -- (we might get rid of this dependency soon)
 * - NVIDIA CUDA (tm), including SDK. We support versions 3.X and 4.0
 * - thrust library (from http://code.google.com/p/thrust/)
 * - doxygen (if you want to build the documentation yourself)
 *
 * For Python Integration, you additionally have to install
 * - pyublas -- from http://mathema.tician.de/software/pyublas
 * - python-nose -- for python testing
 * - python-dev 
 *
 * Optionally, install dependent libraries
 * - cimg-dev for visualization of matrices (grayscale only, ATM)
 *
 *
 * @subsection obtaining  Obtaining CUV
 *
 * You should check out the git repository
 *   @code
 *   $ git clone git://github.com/deeplearningais/CUV.git
 *   @endcode
 *
 * @subsection instproc  Installation Procedure
 *
 * Building a debug version:
 *
 * @code
 * $ cd cuv-version-source
 * $ mkdir -p build/debug
 * $ cd build/debug
 * $ cmake -DCMAKE_BUILD_TYPE=Debug ../../
 * $ ccmake .          # adjust paths to your system (cuda, thrust, pyublas, ...)!
 *                     # turn on/off optional libraries (CImg, ...)
 * $ make -j
 * $ ctest             # run tests to see if it went well
 * $ sudo make install
 * $ export PYTHONPATH=`pwd`/src      # only if you want python bindings
 * @endcode
 *
 * Building a release version:
 *
 * @code
 * $ cd cuv-version-source
 * $ mkdir -p build/release
 * $ cd build/release
 * $ cmake -DCMAKE_BUILD_TYPE=Release ../../
 * $ ccmake .          # adjust paths to your system (cuda, thrust, pyublas, ...)!
 *                     # turn on/off optional libraries (CImg, ...)
 * $ make -j
 * $ ctest             # run tests to see if it went well
 * $ sudo make install
 * $ export PYTHONPATH=`pwd`/src      # only if you want python bindings
 * @endcode
 *
 * @subsection docinst  Building the documentation
 *
 * @code
 * $ cd build/debug    # change to the build directory
 * $ make doc
 * @endcode
 *
 * @section samplecode Sample Code
 *
 * We show two brief examples. For further inspiration, please take a look at
 * the test cases implemented in the @c src/tests  directory.
 *
 * @par Pushing and pulling of memory
 *
 * C++ Code:
 * @code
 * #include <cuv.hpp>
 *     using namespace cuv;
 * 
 *     int main(void){
 *         tensor<float,host_memory_space> h(256);  // reserves space in host memory
 *         tensor<float,dev_memory_space>  d(256);  // reserves space in device memory
 * 
 *         fill(h,0);                          // terse form
 *         apply_0ary_functor(h,NF_FILL,0.f);    // more verbose
 * 
 *         d=h;                                // push to device
 *         sequence(d);                        // fill device vector with a sequence
 * 
 *         h=d;                                // pull to host
 *         for(int i=0;i<h.size();i++)
 *         {
 *             assert(d[i] == h[i]);
 *         }
 *     }
 * @endcode
 *
 * Python Code:
 * @code
 * import cuv_python as cp
 * import numpy as np
 *
 * h = np.zeros((1,256))                                   # create numpy matrix
 * d = cp.dev_tensor_float(h)                              # constructs by copying numpy_array
 *
 * h2 = np.zeros((1,256)).copy("F")                        # create numpy matrix
 * d2 = cp.dev_tensor_float_cm(h2)                         # creates dev_tensor_float_cm (column-major float) object
 *
 * cp.fill(d,1)                                            # terse form
 * cp.apply_nullary_functor(d,cp.nullary_functor.FILL,1)   # verbose form
 *
 * h = d.np                                                # pull and convert to numpy
 * assert(np.sum(h) == 256)
 * d.dealloc()                                             # explicitly deallocate memory (optional)
 *
 * @endcode
 *
 * @par Simple Matrix operations
 *
 * C++-Code
 * @code
 *
 * #include <cuv.hpp>
 * using namespace cuv;
 * 
 * int main(void){
 *     tensor<float,dev_memory_space,column_major> C(2048,2048),A(2048,2048),B(2048,2048);
 * 
 *     fill(C,0);         // initialize to some defined value, not strictly necessary here
 *     sequence(A);
 *     sequence(B);
 * 
 *     apply_binary_functor(A,B,BF_MULT);  // elementwise multiplication
 *     A *= B;                             // operators also work (elementwise)
 *     prod(C,A,B, 'n','t');               // matrix multiplication
 * }
 * @endcode
 *
 * Python Code
 * @code
 * import cuv_python as cp
 * import numpy as np
 * C = cp.dev_tensor_float_cm([2048,2048])   # column major tensor
 * A = cp.dev_tensor_float_cm([2048,2048])   
 * B = cp.dev_tensor_float_cm([2048,2048])
 * cp.fill(C,0)                       # fill with some defined values, not really necessary here
 * cp.sequence(A)
 * cp.sequence(B)
 * cp.apply_binary_functor(B,A,cp.binary_functor.MULT) # elementwise multiplication
 * B *= A                                              # operators also work (elementwise)
 * cp.prod(C,A,B,'n','t')                              # matrix multiplication
 * @endcode
 *
 * The examples can be found in the "examples/" folder under "python" and "cpp"
 */


/** 
 * @namespace cuv
 * @brief contains all cuv functionality
 */

namespace cuv{
}
