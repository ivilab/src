
/* $Id: trim.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "l/l_incl.h" 


int main(int argc, char* argv[])
{
    char  line[ 3000 ];
    int   res;

    ivi_set_debug_level(1);
    
    ivi_init();

    while ((res=BUFF_STDIN_GET_LINE("str> ",line)) != EOF) 
    {
        char* line_ptr = line;
        size_t line_trim_len;
        int    trim_count;
    
        dbi(res);
        dbp("");
        dbs(line_ptr);
        dbi(strlen(line_ptr));
        line_trim_len = trim_len(line_ptr);
        dbi(line_trim_len);

        dbp("");
        dbp("gen_trim_beg");
        trim_count = gen_trim_beg(&line_ptr, " ");
        dbs(line_ptr);
        line_trim_len = trim_len(line_ptr);
        dbi(strlen(line_ptr));
        dbi(line_trim_len);
        dbi(trim_count);

        dbp("");
        dbp("gen_trim_end");
        trim_count = gen_trim_end(line_ptr, " ");
        dbs(line_ptr);
        line_trim_len = trim_len(line_ptr);
        dbi(strlen(line_ptr));
        dbi(line_trim_len);
        dbi(trim_count);

        dbp("-------------------------------Test Ends Here");
    }

    ivi_exit(EXIT_SUCCESS); 
}

