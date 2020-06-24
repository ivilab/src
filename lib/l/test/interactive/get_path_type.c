
/* $Id: get_path_type.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h"

int main(int argc, char **argv)
{
    Path_type path_type;


    check_num_args(argc, 1, 1, (char*)NULL); 

    path_type = get_path_type(argv[ 1 ]);

    if (path_type == PATH_DOES_NOT_EXIST)
    {
        dbm("PATH_DOES_NOT_EXIST");
    }
    else if (path_type == PATH_IS_REGULAR_FILE)
    {
        dbm("PATH_IS_REGULAR_FILE");
    }
    else if (path_type == PATH_IS_DIRECTORY)
    {
        dbm("PATH_IS_DIRECTORY");
    }
    else if (path_type == PATH_IS_PIPE) 
    {
        dbm("PATH_IS_PIPE");
    }
    else if (path_type == PATH_IS_SOCKET) 
    {
        dbm("PATH_IS_SOCKET");
    }
    else if (path_type == PATH_IS_CHARACTER_SPECIAL) 
    {
        dbm("PATH_IS_CHARACTER_SPECIAL");
    }
    else if (path_type == PATH_IS_BLOCK_SPECIAL)
    {
        dbm("PATH_IS_BLOCK_SPECIAL");
    }
    else if (path_type == PATH_IS_UNCLASSIFIED)
    {
        dbm("PATH_IS_UNCLASSIFIED");
    }

    return EXIT_SUCCESS; 
}

