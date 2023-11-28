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
