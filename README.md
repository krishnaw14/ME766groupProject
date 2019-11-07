## Link to the github Repository
https://github.com/krishnaw14/ME766groupProject

# Parallel Code analysis of  N-body simulation
This is the ME766 (High Performance Scientific Computing) course project report. Our objective was to make a simulation for the generic n-body system under gravitational forces using appropriate masses and initial conditions, analysing the time efficiency of the code with different parallel programming methods.

This zip file contains the following:

## Codes
n_body.cpp : O(n^2) serial code

n_body_openmp.cpp : openMP parallelised O(n^2) code

n_body_mpi2.cpp : MPI parallelised O(n^2) code

n_body_openacc.cpp : openACC parallelised O(n^2) code

barnes_hut.cpp : Code developed for the Barnes-Hut quad tree data structure and implementation for 2 bodies


## DATA SETS

tab128.txt : For 128 bodies

tab1024.txt : For 1024 bodies

tab8096.txt : For 8096 bodies

tab65536.txt : For 65536 bodies

Link to the various data sets : https://bima.astro.umd.edu/nemo/archive/

## PROFILING:
analysis.txt : Generated code profile for n_body.cpp

## Instructions for Compiling

For openACC code:           
`$ pgc++ -acc -ta=host -Minfo=accel -o nbody n_body_openacc.cpp`   
`$ ./nbody`

For openmp code:       
`$ g++ n_body_openmp.cpp -fopenmp -o nbody`   
`$ ./nbody` 

For MPI code:       
`$ mpic++ n_body_openmp.cpp -o nbody ` 
`$ mpirun -np 4 ./nbody`

## NOTE 
For compiling any of the codes, the corresponding dataset must be present in the same directory

