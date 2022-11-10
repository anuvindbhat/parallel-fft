# Parallelizing Image Compression using FFT

### Anuvind Bhat (anuvindb) & Saatvik Suryajit Korisepati (skorisep)

## **Summary**
We are going to be parallelizing an image compression algorithm that utilizes Fast Fourier Transform (FFT). This will be done using OpenMP, …. We will also be analyzing the performance benefits obtained from the parallelization with regards to the sequential version.
## **Background**
### *What is Fast Fourier Transform (FFT)?*
FFT is a signal processing algorithm that allows for the transformation of signals from their “original domain to a frequency domain” by utilizing multiplications of numerous sinosoidal functions.
### *How can it be used for image compressions?*
An image itself can be considered as a set of signals. In this perspective each pixel can be seperated into brightness and color. The brightness can be viewed as the amplitude while the color can be viewed as the shift in phase. Therefore, by converting the image into signals, we are able to compress the image using the same FFT concept.
### *How does it work?*
Upon applying the sinosoidal mapping to each pixel in the image, we are left with a structured representation of the image where each pixel contains a specific FFT value. It can be noticed for many images that the majority of FFT values are extremely small (ex. 98% of pixels are less than 5% of the maximum value). These small values can be disregarded as they contribute little to no information to the actual image. Therefore, when the FFT mapping of the image is inverted, the majority of the image’s information is preserved.
### *Where can it be parallelized?*
FFT image compression requires computations of rows and then computations of columns of an image (or vice versa). In this case, the actual computation across the rows and columns can be parallelized. However, it is worth considering whether actual pixel values itself can be parallelized. Furthermore, upon calculating the FFT compression of an image, the result has to be post-processed to disregard insiginificant data. This can also be potentially parallelized as it involves looking at each pixel value in the result.

## **Challenge (needs work)**
The data access patterns in the FFT algorithm do not follow a pattern and hence cannot be predicted. Therefore, obtaining any performance by optimizing data movement is not a viable option.
## **Resources**
We will be using the CPUs of the GHC machines. Our implementation will start from an existing FFT sequential implementation that will have to be modified to compress images. Upon completion of this general sequential implementation, we will parallelize the algorithm.

For inspiration, we will be having a look at Spiral Lab’s work on FFT optimization as well as using some papers on optimizing the algorithm (include papers here).

## **Goals and Deliverables**
### *Plan to achieve:*
Parallelize FFT with at least OpenMP and analyze the performance against the sequential version.
### *Hope to achieve:*
If time allows, we will be creating a tool that will allow users to share an extremely large image file and compare the outputs of the two versions of FFT compression.

We will also be parallelizing the algorithm using other frameworks such as ISPC. If this step is completed, we will also analyze the performance and compare the results from using different frameworks.

### *Deliverables:*
We will predominantly be focusing on showing speedup graphs (comparing performance of the two versions of the algorithm).

Additionally, if time allows, we would like to create a demo tool of the images being compressed.

## **Platform Choice**
We are going to be implementing the algorithm with a focus on CPU execution. We made this decision as it is the most regularly used system to perform image compression.

~~FFT is an inherently recursive algorithm therefore, we can use OpenMP, ISPC, … to parallelize the algorithm.~~
## **Schedule**
Note that we are targeting the early submission deadline of 12/9.
| Week | Description |
| ----------- | ----------- |
| Week 0 (11/7) | Familiarize ourselves with FFT and start implementation of sequential code
| Week 1 (11/14) | Complete implementation of sequential code and identify areas of parallelism
| Week 2 (11/21) | Begin and complete parallelization using OpenMP
| Week 3 (11/28) | Measure and analyze results
| Week 4 (12/5) | Work on and finalize writeup
