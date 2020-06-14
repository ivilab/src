
/* $Id: ivi_err3.c 25499 2020-06-14 13:26:04Z kobus $ */



#include "l/l_gen.h" 

int main(int argc, char **argv)
{
    char *test_str; 
    char buff[ 1000 ]; 




    test_str = "Another very looooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooong message";

    ivi_clear_error();
    add_error("%s", test_str);  

    add_error("%50.30s", 
              "A looooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooong message");

    add_error("Trouble with %F.", stdin);

    /*
     *  add_error("Test bad format element with: -->%Z<--.", stdin);
     */

    add_error("%s", test_str);  

    add_error("%50.30s", 
              "A looooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooong message");

    add_error("Trouble with %F.", stdin);
    add_error("Trouble with %hd.", ((ivi_int16)666));
    add_error("Trouble with %+*.*hd.", 30,5, ((ivi_int16)666));
    add_error("Trouble with %*.*Lf.", 10,5, ((long_double) 1.2345678));

    p_stderr("Printing.\n");
    ivi_print_error(); 

    p_stderr("Printing nothing.\n");
    ivi_print_error(); 

    ivi_clear_error();
    insert_error("%s", test_str);  

    insert_error("%50.30s", 
                 "A looooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooong message");

    insert_error("Trouble with %F.", stdin);

    /*
     *  insert_error("Test bad format element with: -->%Z<--.", stdin);
     */

    insert_error("%s", test_str);  
    insert_error("%50.30s", 
                 "A looooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooong message");

    insert_error("Trouble with %F.", stdin);
    insert_error("Trouble with %hd.", ((ivi_int16)666));
    insert_error("Trouble with %+*.*hd.", 30,5, ((ivi_int16)666));
    insert_error("Trouble with %*.*Lf.", 10,5, ((long_double) 1.2345678));

    p_stderr("Printing.\n");
    ivi_print_error(); 

    p_stderr("Printing nothing.\n");
    ivi_print_error(); 

    ivi_clear_error();
    add_error("Set error");
    add_error("After set error");
    insert_error("Before set error");
    cat_error(" .... AND MORE");
    add_error("After After set error");
    cat_error(" .... AND MORE");

    p_stderr("Printing.\n");

    ivi_get_error(buff, 1000);
    p_stderr("\n\nBuff is ->%s<-\n\n", buff);

    set_error("Another user message");

    p_stderr("Printing.\n");
    ivi_print_error(); 

    p_stderr("Printing nothing.\n");
    ivi_print_error(); 

    return EXIT_SUCCESS;
}

