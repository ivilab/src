
/* $Id: get_temp_file_name.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h"

int main(void)
{
    char temp_file_name[ MAX_FILE_NAME_SIZE ]; 


    EPETE(get_temp_file_name(temp_file_name, sizeof(temp_file_name)));
    dbs(temp_file_name); 
    NPETE(ivi_fopen(temp_file_name, "w")); 

    EPETE(get_temp_file_name(temp_file_name, sizeof(temp_file_name)));
    dbs(temp_file_name); 
    NPETE(ivi_fopen(temp_file_name, "w")); 

    EPETE(get_temp_file_name(temp_file_name, sizeof(temp_file_name)));
    dbs(temp_file_name); 
    NPETE(ivi_fopen(temp_file_name, "w")); 

    return EXIT_SUCCESS; 
}

