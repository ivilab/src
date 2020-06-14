
/* $Id: append_word_list.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "l/l_incl.h" 

#define BASE_NUM_TRIES  1

int main(void)
{
    Word_list* word_list_ptr = NULL; 
    char line[ 1000 ];

    ivi_init();

    if (is_interactive())
    {
        ivi_set_debug_level(2); 
    }
    else 
    {
        ivi_set_debug_level(0); 
    }

    while (BUFF_GET_REAL_LINE(stdin, line) != EOF)
    {
        EPETE(append_word_list(&word_list_ptr, line));
    }

    EPETE(write_word_list(word_list_ptr, NULL));

    free_word_list(word_list_ptr); 

    ivi_cleanup();
    
    return EXIT_SUCCESS;
}

