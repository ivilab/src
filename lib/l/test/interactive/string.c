
/* $Id: string.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h" 

int main(void)
{
    char line[ 50 ]; 
    char buff[ 200 ]; 
    char *buff_pos; 


    ivi_l_set("debug_level", "2"); 

    buff_pos = buff; 

    while (BUFF_STDIN_GET_LINE("> ",line) != EOF) 
    {
        dbi(find_char(line,'a')); 
        dbi(n_find_char(line,5,'a')); 
        dbi(count_char(line,'a')); 
        dbs(line); 
        rpad(line,3,5);
        dbs(line); 
        char_for_char_translate(line,'a','*'); 
        dbs(line); 
        str_char_build(&buff_pos,'!',10); 
        dbs(buff); 
        dbs(line); 
        dbnc(line, 7); 
        dbc(*(line + 1)); 
        
        dbs(line);
        dbi(strlen(line));
        dbi(trim_len(line)); 
    }

    return EXIT_SUCCESS; 
}

