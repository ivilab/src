
/* $Id: word_list.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "l/l_incl.h" 


int main(void)
{
    Word_list* word_list_ptr = NULL; 
    Word_list* sorted_word_list_ptr = NULL; 

    ivi_init();

    if (is_interactive())
    {
        ivi_set_debug_level(2); 
    }
    else 
    {
        ivi_set_debug_level(0); 
    }

    EPETE(read_word_list(&word_list_ptr, NULL));
    EPETE(sort_word_list(&sorted_word_list_ptr, word_list_ptr));
    EPETE(write_word_list(sorted_word_list_ptr, NULL));

    free_word_list(word_list_ptr); 
    free_word_list(sorted_word_list_ptr); 

    ivi_cleanup();
    
    return EXIT_SUCCESS;
}

