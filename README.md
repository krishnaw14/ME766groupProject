# ME766groupProject
This zip file contains the following:
n_body.cpp : O(n^2) serial code
n_body-openmp.cpp : openMP parallelised O(n^2) code
n_body-mpi.cpp : MPI parallelised O(n^2) code





Link to the various data sets : https://bima.astro.umd.edu/nemo/archive/

Instructions for running openACC code:
Use a PGI compiler before the 17. version
Type the following command in the terminal before executing the code: export PGI_ACC_TIME=1
