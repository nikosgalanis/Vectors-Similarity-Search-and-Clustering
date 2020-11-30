# Vectors Similarity Search and Clustering

One of the most difficult to solve problems in the big-data era is the curse of dimensionality. It refers to high computing needs in order to process vectors in a high dimension. In order to solve that problem, many solutions were introduced, the most of which include a probabilistic approach.


In our case, we have a large set of images, which are represented as a vector of their pixels' color, and we want to create 2 programs:

-  A program that predicts the nearest neighbor of a vector. 
-   A programs that performs Clustering given a dataset.


The nearest neighbor problem is solved using an approximate approach, as well as a brute force one. In the approximate, we are trying to predict the NN, by applying 2 different algorithms: __Locality Sensitive Hashing__ and __Random Projections to the Hypercube__. We compare our timing results and our predictions with the Brute Force model, in order to find out how well do our algorithms perform. In order to test the program, we use the MNIST dataset, that contains pictures as vectors, of size 28x28.

The clustering problem is solved using a variety of algorithms. During the initialization stage we implement ___k-means__, during the update step we implement __k-median__, and during the assignment we implement direct assignment using the __Lloyd's algorithm__, as well as reverse assignment using the __LSH__ and __Hypercube__ Range Search that we implemented earlier.

The project was implemented using the C++ language, by taking advantage of the STL library.

## Compiling and running

For the compilation of all the programs we can use the Makefile. When being in the root directory of our project, in order to compile all our programs at once   we can just call: __make__ or __make all__
The lsh, cube and cluster executables will then be placed in the executables directory in their corresponding sub-directory.

We can delete these directories by using __make clean__.

 In order to compile a program of our choice we have these options:

 - LSH: __make lsh__
 - Hypercube: __make cube__
 - Clustering: __make cluster__

 In order to delete the directories of a specific program we can use clean\_lsh , {clean\_cube, or clean\_cluster respectively.

 We also use __-Ofast__ argument for the gcc to optimize the performance of our programs. This addition gave us significant improvement in the the performance of our programs. 

 We can run the executables of each program by calling __make \_run _executable_\_ _mode___. Ex: __make run_cluster_lloyds__. Complete documentation for the running part is available on the pdf report in the parent directory.

## License

This project is licensed under the MIT License - see the [LICENSE](/LICENCE) file for details

## Contributors

[Nikos Galanis](https://github.com/nikosgalanis) \
[Sofoklis Strompolas](https://github.com/SofoSt/)

