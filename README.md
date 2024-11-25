# Overview
This study presents a replication attempt in C++ of the study by Phoophakdee Benjarath, and Mohammed J. Zaki., on
Genome-scale disk-based suffix tree indexing. Our goal was to replicate the four-stage of researchers from scratch and
evaluate their original findings. However, we encountered challenges due to the limited guidance for its implementation. As
a result, instead of a one-to-one replication of the study, we decided to implement our interpretation of the algorithm, this
solution while functional and able to replicate the algorithm’s main stages, had its own limitations and was unable to be
utilized for genome-scale sequences. This experience showcases the importance of thoroughness in reporting research to
ensure the reproducibility and reliability of findings.

An in-depth technical view can be accessed at `./Project Report/Technical_Report.pdf`, we provide some main figures below for ease of access:

Genetic sequence Suffix Tree example:

![image](https://github.com/user-attachments/assets/5ff94da7-46fc-49a2-8223-56f2e343e39a)


Algorithm stages:

![image](https://github.com/user-attachments/assets/018a1e8a-995a-433e-a67d-3fae5e0420bf)


Suffix Tree and Node class variables:

![image](https://github.com/user-attachments/assets/1019d518-3f8d-45d5-934b-a7945b32b2c3)

Algorithm 1, Variable length prefix creation: 

![image](https://github.com/user-attachments/assets/3333d1fe-6453-43b8-a968-8a9a789185b7)

Algorithm 2, Sequence partitioning:

![image](https://github.com/user-attachments/assets/73e0e678-6f44-4ee6-abb7-fb5d8f5aa171)

Algorithm 3, Suffix tree construction:

![image](https://github.com/user-attachments/assets/6fc7d0a2-41df-487f-95de-14a58f1243be)

Algorithm 4, Querying partition tree for prefix:

![image](https://github.com/user-attachments/assets/70c82b19-cde2-4b26-8ee2-b35f197502e4)

Algorithm 5, Tree merging:

![image](https://github.com/user-attachments/assets/3bbb93b9-24cb-41d1-bb69-89e1496ee274)


# File/Folder descriptions

## _Files_

- \*.h files are the header files for the corresponding .cpp files to be included in the main.cpp file.
- **file_utils** contains functions for creating, deleting folders to store the output files of each stage and for partitioning the input sequence into smaller files.
- **node** contains the class constructor and respective methods for the nodes of the suffix tree.
- **suffix_tree** contains the class constructor and respective methods for the suffix tree.
- **prefix_count** contains the methods required for the variable lenght prefix stage.
- **suffix_links** contains the methods required for the suffix links stage.
- **run_pipeline** contains the four stages of the algorithm in distinct functions and the main function that calls them in the right order.
- **TRELLIS.exe** is the executable file of the algorithm

## _Folders_
- **./Project Report** contains the technical report of the project.
- **./input** contains the input sequence files for the algorithm.
- **./temp_prfx** contains the identified variable length prefixes.
- **./temp_partition** contains the partitioned input sequence files.
- **./temp_trees** contains the prefixed suffix sub-trees of the partitioned input for each prefix.
- **./temp_trees/merged_trees** contains the merged suffix trees of the partitioned input for each prefix.
- **./temp_trees/final_trees** contains the merged trees with the addition of suffix links.

# _Instructions on running the algorithm_

1. Make sure an `./input` directory exists in the same directory as the executable file `TRELLIS.exe`.
2. The Sars Cov-2 genome sequence should be in the `./input` directory with the name `NC_045512V2.fa`.
3. Run the executable file `TRELLIS.exe`.

If you wish to compile the algorithm the command utilized is:

`g++ -fdiagnostics-color=always -g -ggdb -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -std=c++20 *.cpp -o /home/cinnamonk/projects/Trelis_repo/TRELLIS.exe`
