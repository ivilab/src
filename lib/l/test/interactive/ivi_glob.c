
/* $Id: ivi_glob.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "l/l_incl.h" 

int main(void)
{
    char glob[ MAX_FILE_NAME_SIZE ];
    Word_list* paths = NULL; 
    unsigned int i;
    char** words;
    unsigned int num_words; 
    


    while (BUFF_STDIN_GET_LINE("glob> ", glob) != EOF)
    {
        dbp("--------- MATCHES  -------------");

        EPE(ivi_glob(&paths, glob, NULL));

        num_words = paths->num_words;
        words = paths->words;

        for (i = 0; i < num_words; i++)
        {
            dbs(words[ i ]); 
        }

    }

    free_word_list(paths); 

    return EXIT_SUCCESS; 
} 


/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

