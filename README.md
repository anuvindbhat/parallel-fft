# Parallelizing Image Compression using FFT

Anuvind Bhat (`anuvindb`) & Saatvik Suryajit Korisepati (`skorisep`)

## **Summary**

We are going to be parallelizing an image compression algorithm that utilizes Fast Fourier Transform (FFT). We will be focusing on CPU parallelism on a shared memory machine. Depending on avenues for parallelism we discover, we will explore using different frameworks such as OpenMP, Cilk, ISPC, etc. We will also analyze the overhead and bottlenecks in our approach.

## **Background**

### *What is Fast Fourier Transform (FFT)?*

FFT is an algorithm, with roots in signal processing, for computing the so-called discrete Fourier transform (DFT) of a sequence of numbers. The naive approach to computing the DFT takes O(n2) time while FFT computes the result in O(n log(n)) time. In the context of signal processing, FFT is used to decompose a signal into sinusoidal curves with different amplitudes and frequencies. This is known as converting a signal from the time domain to the frequency domain.

### *How can it be used for image compressions?*

An image itself can be considered as a set of signals. In this perspective each pixel can be separated into brightness and color. The brightness can be viewed as the amplitude while the color can be viewed as the shift in phase. Therefore, by converting the image into signals, we are able to compress the image using the same FFT concept. Note that since images are 2D, a generalized version of the FFT known as 2D FFT needs to be used.

### *How does it work?*

After applying FFT to an image, we are left with a new “image” where every pixel has a fourier coefficient. Empirically, for many images, the majority of coefficients are extremely small (eg. 98% of pixels are less than 5% of the maximum value). These small coefficients can be discarded as they contribute little to no information to the actual image. We can then store only those coefficients which are large (and their location), hence compressing the image. To retrieve the original image, inverse FFT is used and the majority of the original image’s information is preserved.

### *Where can it be parallelized?*

2D FFT performs FFT of rows and then FFT of columns of an image (or vice versa). In this case, the actual computation across the rows and columns can be parallelized. However, we will also explore parallelizing FFT of a given row or column as well (1D FFT). Since FFT is a recursive algorithm, we expect most of the challenge to be in parallelizing 1D FFT. Furthermore, upon calculating the FFT of an image, the result has to be post-processed to disregard insignificant data. This can also be potentially parallelized as it involves looking at each pixel value in the result.

## **Challenge (needs work)**

The recursive nature of FFT makes it non-trivial to parallelize. Furthermore, it is a memory bound algorithm that doesn’t scale well with increasing compute. The algorithm also has poor access patterns (locality) and does not utilize the cache hierarchy well. We also expect to run into several other challenges once we begin our implementation.

As we are starting from scratch with our implementation, we expect the sequential implementation to also require significant work.

## **Resources**

We will be using the CPUs of the GHC machines as well as the shared memory nodes on the Bridges 2 machines to measure how our implementation scales. We will start from scratch in our implementation. Upon completion of a sequential implementation, we will identify areas for parallelism (possibly in addition to ones we’ll already described) and will work on parallelizing the algorithm.

For inspiration, we will refer to existing research on parallel FFT implementations. For example, we will take a look at Spiral Lab’s work on optimizing FFT.

## **Goals and Deliverables**

### *Plan to achieve:*

Parallelize FFT with at least OpenMP and analyze the performance against the sequential version. Quantitatively identify bottlenecks and overhead in our parallel implementation. We will also find a good reference implementation of FFT that we can use as a benchmark.

### *Hope to achieve:*

We hope to parallelize our implementation using multiple frameworks (such as Cilk and ISPC). If this step is completed, we will also analyze the performance and compare the results from using these frameworks. We will also explore other algorithmic optimizations such as switching to iterative DFT algorithms at high recursion depths with the aim of utilizing SIMD instructions and better cache locality.

We also hope to create a demo tool that shows the different intermediate steps in image compression, and how discarding a large percentage of fourier coefficients doesn’t adversely affect image quality.


### *Deliverables:*

We will predominantly be focusing on showing speedup graphs of our various implementations as well as other performance metrics that we measure. If time allows, we will either have a demo of the various stages in image compression or some images that illustrate them.

## **Platform Choice**

We are going to be implementing the algorithm with a focus on parallelizing CPU execution. We made this decision as images are small enough to fit onto a computer’s memory and shared memory machines are more than sufficient for the task. Moreover, the recursive nature of FFT makes it most amenable to parallelism on shared memory systems.

The parallelism in 2D FFT makes OpenMP a good candidate while the recursive nature of 1D FFT makes Cilk something worth exploring as well. In addition, we might explore switching over to the naive O(n2) DFT algorithm for small enough sizes, and SIMD instructions (with ISPC) would help with that.

## **Schedule**

Note that we are targeting the early submission deadline of 12/9.
Week 0 (11/7): Familiarize ourselves with FFT and start implementation of sequential code
Week 1 (11/14): Complete implementation of sequential code and identify areas of parallelism
Week 2 (11/21): Begin and complete parallelization using OpenMP
Week 3 (11/28): Measure and analyze results
Week 4 (12/5): Final writeup
Week 5 (12/12): Spillover in case we don’t meet the early deadline target
