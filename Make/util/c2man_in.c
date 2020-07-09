
/* =============================================================================
 *                             c2man_in
 *
 * Filter to massage comments for c2man
 *
 * This program filters library code to adjust comments for c2man. It also
 * creates an index file that can be combined with others to create an index for
 * all IVI library code. 
 *
 * This process does not work on program files (like this one). We need to do a
 * bit of work here. 
 *
 * -----------------------------------------------------------------------------
*/


#include "l/l_incl.h"


int main(int argc, char** argv)
{
    char  line[ 10000 ];
    char* line_pos;
    char  name[ 1000 ];
    int   slash_found;
    int   star_found;
    int   blank_found;
    int   in_typedef = FALSE;
    int   first_item_in_typedef = TRUE;
    int   comment_beg_col;
    int   comment_end_col;
    int   i;
    Queue_element* typdef_queue_head = NULL;
    Queue_element* typdef_queue_tail = NULL;
    char  author[ 1000 ];
    char  documenter[ 1000 ];
    Bool  have_disclaimer = FALSE;
    Bool  have_author = FALSE;
    Bool  have_documenter = FALSE;
    int   test_level = NOT_SET; 
    int   first_block_line = NOT_SET; 
    int   doing_formatted = NOT_SET; 
    Bool  in_comment = FALSE;
    Bool  in_block = FALSE;
    int   first_pound_found = FALSE; /* Ignore copyright header. */
    int   has_index_field = FALSE; 
    FILE* index_fp      = NULL;
    char  index_file_name[ MAX_FILE_NAME_SIZE ]; 
    int doing_html = FALSE;
    int option = NOT_SET;
    char value_buff[ MAX_FILE_NAME_SIZE ]; 
    extern int ivi_debug_level; 

    /* Defaults. */

    BUFF_CPY(name, "(unknown)");
    BUFF_CPY(author, "Kobus Barnard");
    BUFF_CPY(documenter, "Kobus Barnard");
    BUFF_CPY(index_file_name, "index"); 

    while ((option = ivi_getopts(argc, argv, "-hi:", NULL,
                                 value_buff, sizeof(value_buff)))
               != EOF
          )
    {
        switch (option)
        {
            case ERROR :
                add_error("Program has invalid arguments.");
                ivi_print_error();
                ivi_exit(EXIT_FAILURE);
            case 'h' :
                doing_html = TRUE;
                break;
            case 'i' :
                BUFF_CPY(index_file_name, value_buff);
                break;
        }
    }   

    while (BUFF_FGET_LINE(stdin, line) != EOF)
    {
        line_pos        = line;
        slash_found     = FALSE;
        star_found      = FALSE;
        blank_found     = FALSE;
        comment_beg_col = FALSE;
        comment_end_col = FALSE;

        if (line[ 0 ] == '#')
        {
            if (in_typedef)
            {
                p_stderr("Preprocessor directives inside typedef defeat c2man_in's documenting of them.\n"); 
                p_stderr("You need to to rewrite the code so that any # lines are outside the typedef block.\n"); 
                free_queue(&typdef_queue_head, &typdef_queue_tail, ivi_free);
                ivi_exit(EXIT_FAILURE); 
            }

            first_pound_found = TRUE;
            put_line(line); 
            continue;
        }
        else if (! first_pound_found) 
        {
            /* 
             * Assume nothing interesting happens until our first '#' line. In
             * particular, skip copyright header--it looks too much like a
             * documentation block.  Technically, we remove everything that
             * precedes the first '#' since, I believe all library files have a
             * '#' before any code. 
            */
            continue;
        }

        trim_beg(&line_pos);
        trim_end(line_pos);

        comment_beg_col = find_char_pair(line_pos, '/', '*');
        comment_end_col = find_char_pair(line_pos, '*', '/');

        /* Reasonable to use '1' because we trimmed before looking. */
        if (comment_beg_col == 1) 
        {
            /*
            // Check  for a complete comment in first column, followed by code
            // (as is often the case in complex #ifdef's in Kobus's code). These
            // screw up c2man, so replace the comments with blanks. If we only
            // have a comments on the line, but we have both the begin and the
            // end, then the line gets ignored implicitly. 
            */
            if (comment_end_col)
            {
                if  (line_pos[ comment_end_col + 1 ] != '\0') 
                {
                    for (i=0; i<comment_end_col + 1; i++)
                    {
                        line_pos[ i ] = ' ';
                    }
                    put_line(line);
                }
            }
            else if (HEAD_CMP_EQ(line_pos + 3, "========================================"))
            {
                in_block = TRUE; 
                first_block_line = TRUE; 
                doing_formatted = FALSE;
                have_documenter = FALSE;
                have_author = FALSE;
                has_index_field = FALSE; 

                /* We do not want extra stuff, like separator lines.  */ 
                put_line("/*");
            }
            else 
            {
                in_comment = TRUE;
            }
        }
        else if ((comment_beg_col > 1) && (in_comment))
        {
            p_stderr("Found comment start inside a comment.\n");
            p_stderr("If this is not true, it might be a bug in c2man_in.\n");
            p_stderr("Otherwise, clean up the code.\n");
            ivi_exit(EXIT_FAILURE); 
        }
#ifdef DEF_OUT
        {
        if (in_comment)
            {
            }
            else if (! comment_end_col) 
            {
                char* temp_line_pos = line_pos + comment_beg_col - 1; 

                while (*temp_line_pos != '\0')
                {
                    *temp_line_pos = ' ';
                    temp_line_pos++;
                }
                in_comment = TRUE; 
            }
            put_line(line);
        }
#endif 
        else if ((comment_end_col) && (in_block))
        { 
            in_block = FALSE;

            if (! have_disclaimer) 
            {
                put_line("Disclaimer:");

                /* Test level could be NOT_SET which is gaurenteed to be
                 * negative. 
                */
                if (test_level < 3)
                {
                    put_line("    This software is not adequately tested.");
                    put_line("    It is recommended that results are checked independently.");
                }
                else 
                {
                    put_line("    This software is somewhat tested.");
                    put_line("    Nonetheless, it is recommended that results are checked independently.");
                }
                ivi_puts("\n");
            }

            if (test_level == NOT_SET)
            {
                put_line("Test level:");
                put_line("    Not documented.");
                ivi_puts("\n");
            }

            if (! have_author) 
            {
                put_line("Author:");
                put_line("    Kobus Barnard");
                ivi_puts("\n");
            }

            if (! have_documenter) 
            {
                put_line("Documenter:"); 
                put_line("    Kobus Barnard");
                ivi_puts("\n");
            }

            /* We do not want extra stuff, like separator lines. But we may
             * risk loosing some content with ill-formatted blocks. Too bad. 
            */ 
            put_line("*/");
        } 
        else if ((comment_end_col) && (in_comment)) 
        { 
            /* We have dealt with the case that there is a starting comment
             * string on this line, so now there is not. However, we have an
             * ending one.  
            */ 
            in_comment = FALSE;

            /* If there is code after the the ending string, then we have an
             * issue. Check for this happening.  
            */ 
            line_pos += comment_end_col;
            line_pos++; 
            trim_beg(&line_pos); 

            if (*line_pos != '\0')
            {
                p_stderr("Apparent code after comment end in line:\n");
                fput_line(stderr, line);
                p_stderr("\n");
                ivi_exit(EXIT_FAILURE); 
            }
        }
        else if (in_comment) 
        {
            ; /* EMPTY */   
        }
        else if (in_block) 
        {
            if (*line_pos == '\0') blank_found = TRUE;

            if (*line_pos == '/') slash_found = TRUE;
            while (*line_pos == '/') line_pos++;

            trim_beg(&line_pos);

            if (*line_pos == '*') star_found = TRUE;
            while (*line_pos == '*') line_pos++;

            trim_beg(&line_pos);


            if (first_block_line)
            {
                first_block_line = FALSE; 

                if (isalpha(*line_pos))
                {
                    BUFF_CPY(name, line_pos); 
                }
                else 
                {
                    BUFF_CPY(name, "(unknown)");
                }
            }
            /* 
             * Test for lines with ==>. These are to be protected from c2man, so
             * we remove them. Similarly, hide the final separator from c2man.
             * Note that we are not checking that the separator is in fact at
             * the end. 
            */
            else if ((HEAD_CMP_EQ(line_pos, "==>")) || (HEAD_CMP_EQ(line_pos, "------")))
            {
                ; /* EMPTY */
            }
            /*
            // Test for "author" to over-ride default.
            */
            else if (     (IC_HEAD_CMP_EQ(line_pos, "author:"))
                      ||  (IC_HEAD_CMP_EQ(line_pos, "authors:"))
                      ||  (IC_HEAD_CMP_EQ(line_pos, "author :"))
                      ||  (IC_HEAD_CMP_EQ(line_pos, "authors :"))
                    )
            {
                have_author = TRUE; 
                put_line(line_pos);
            }
            /*
            // Test for "documenter" to over-ride default.
            // Documentor or Documenter? Usage varies. Seems to be a trend
            // towards Documenter. 
            */
            else if (    (IC_HEAD_CMP_EQ(line_pos, "documentor:"))
                      || (IC_HEAD_CMP_EQ(line_pos, "documentor :"))
                      || (IC_HEAD_CMP_EQ(line_pos, "documentors:"))
                      || (IC_HEAD_CMP_EQ(line_pos, "documentors :"))
                      || (IC_HEAD_CMP_EQ(line_pos, "documenter:"))
                      || (IC_HEAD_CMP_EQ(line_pos, "documenter :"))
                      || (IC_HEAD_CMP_EQ(line_pos, "documenters:"))
                      || (IC_HEAD_CMP_EQ(line_pos, "documenters :"))
                      || (IC_HEAD_CMP_EQ(line_pos, "documented:"))
                      || (IC_HEAD_CMP_EQ(line_pos, "documented :"))
                      || (IC_HEAD_CMP_EQ(line_pos, "documented by:"))
                      || (IC_HEAD_CMP_EQ(line_pos, "documented by :"))
                    )
            {
                have_documenter = TRUE; 
                put_line(line_pos);
            }
            /*
            // Test for "disclaimer" to over-ride default.
            */
            else if (     (IC_HEAD_CMP_EQ(line_pos, "disclaimer:"))
                      ||  (IC_HEAD_CMP_EQ(line_pos, "disclaimer :"))
                    )
            {
                have_author = TRUE; 
                put_line(line_pos);
            }
            else if (     (IC_HEAD_CMP_EQ(line_pos, "test level:"))
                      ||  (IC_HEAD_CMP_EQ(line_pos, "test level :"))
                    )
            {
                char* temp_line_pos = line_pos + 10; 

                trim_beg(&temp_line_pos);
                ASSERT(*temp_line_pos == ':'); 
                temp_line_pos++; 
                trim_beg(&temp_line_pos);

                EPETE(ss1pi(temp_line_pos, &test_level));

                put_line(line_pos);
            }
            /*
            // Fudge "see also" as to not conflict with c2man use.
            */
            else if (    (IC_HEAD_CMP_EQ(line_pos, "see also:"))
                      || (IC_HEAD_CMP_EQ(line_pos, "see also :"))
                    )
            {
                line_pos += 9;
                trim_beg(&line_pos);
                ASSERT(*line_pos == ':'); 
                line_pos++; 

                ivi_puts("Related:");
                put_line(line_pos); 
            }
            else if (    (IC_HEAD_CMP_EQ(line_pos, "index:"))
                      || (IC_HEAD_CMP_EQ(line_pos, "index :"))
                    )
            {
                char phrase[ 1000 ];

                has_index_field = TRUE;

                if (index_fp == NULL)
                {
                    NPETE(index_fp = ivi_fopen(index_file_name, "w"));
                }

                line_pos += 5;   /* len("index"); */

                trim_beg(&line_pos);

                if (*line_pos == ':')
                {
                    line_pos++;
                    trim_beg(&line_pos);
                }

                while (BUFF_GEN_GET_TOKEN_OK(&line_pos, phrase, ",."))
                {
                    ivi_fprintf(index_fp, "%-35s %s\n", name, phrase);
                }
            }
            else if (*line_pos == '|')
            {
                if (doing_html)
                {
                    /* 
                     * Now handled by hacking c2man code. 

                    if (! doing_formatted) 
                    {
                        put_line("<pre>");
                    }

                    */
                }
                else
                {
                    put_line(".br");
                }
                put_line(line_pos);
                doing_formatted = TRUE;
            }
            else if (doing_formatted)
            {
                if (doing_html)
                {
                    /* 
                     * Now handled by hacking c2man code. 

                    put_line("</pre>");

                    */
                }
                else
                {
                    put_line(".br");
                }
                put_line(line_pos);
                doing_formatted = FALSE;
            }
            else 
            {
                put_line(line_pos);
            }
        }
        else if (in_typedef)
        {
            insert_at_end_of_queue(&typdef_queue_head, &typdef_queue_tail,
                                   (void*)ivi_strdup(line));

            if (*line_pos == '}')
            {
                Queue_element* cur_elem = typdef_queue_head;

                put_line("\n);");
                in_typedef = FALSE;
                first_item_in_typedef = TRUE;


                while (cur_elem != NULL)
                {
                    put_line((char*)(cur_elem->contents));
                    cur_elem = cur_elem->next;
                }

                free_queue(&typdef_queue_head, &typdef_queue_tail, ivi_free);
                /* Swallow the next line. */
                /* BUFF_FGET_LINE(stdin, line); */
            }
            else if (*line_pos == '{')
            {
                put_line("(");
            }
            else if (blank_found || slash_found)
            {
                /*EMPTY*/
                ; /* Do nothing. */
            }
            else
            {
                char_for_char_translate(line, ';', ' ');
                trim_end(line);

                if (first_item_in_typedef)
                {
                    first_item_in_typedef = FALSE;
                }
                else if (line[0] != '\0') 
                {
                    ivi_puts(",\n");
                }

                ivi_puts(line);
            }
        }
        else if (HEAD_CMP_EQ(line_pos, "typedef"))
        {
            char typedef_type[ 100 ];

            line_pos += 7;
            trim_beg(&line_pos);
           
            /* Jump over "struct" or "union". */
            BUFF_GET_TOKEN(&line_pos, typedef_type); 

            if ((STRCMP_EQ(typedef_type, "struct")) || (STRCMP_EQ(typedef_type, "union")))
            {
                trim_beg(&line_pos);
                trim_end(line_pos);

                if ((FIND_CHAR_YES(line_pos, ';')) || (FIND_CHAR_YES(line_pos, '{')))
                {
                    p_stderr("\nWarning: Typedef struct or union needs to continue onto next line starting with the '{' to be documented.\n\n");
                    ivi_puts(line);
                    ivi_puts("\n");
                }
                else
                {
                    /* OK, this looks generic enough for dumb parsing. So, we
                     * translate 
                     *      typedef struct xxx
                     * to
                     *      typedef_struct xxx_mock_function
                    */

                    ivi_puts("typedef_");
                    ivi_puts(typedef_type);
                    ivi_puts(" ");

                    ivi_puts(line_pos);
                    ivi_puts("_mock_function");
                    ivi_puts("\n");

                    insert_at_end_of_queue(&typdef_queue_head, &typdef_queue_tail,
                                           (void*)ivi_strdup(line));
                    in_typedef = TRUE;
                }
            }
            else
            {
                put_line(line);
            }
        }
        else 
        {
            put_line(line);
        }
    }

    ivi_fclose(index_fp);

    return EXIT_SUCCESS;
}


