# Programming-Languages-Final-Project
Given two large .dat files populated with longs, find all numbers that are present in both files. 

The goal was to solve this problem as fast as possible in my choice of language. Tested on 1b integers. 

Serial solution:
• I changed the way I was reading longs from a file. Instead of fscanf, I decided to use fread which is a lower level function. Trip cost of getting a memory block from the disk is really high and because this function can read a file in big chunks it shortens the reading time dramatically.
• I used hashmap data structure
• I read the original values into a hashmap and the search keys into a static array
(allocated in heap).
• I went through search keys array and searching for items in values
hashmap.

Parallel solution:
• I parallelized quicksort and binary search.

Results: 
Serial - 1b items - average of 3 runs: 383.8587s
Parallel - 1b items - average of 3 runs: 157.109 (NUM_THREADS=8)
