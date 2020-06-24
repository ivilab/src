/* $Id: is_interactive.c 25587 2020-06-24 02:28:42Z kobus $
 */
#include <l/l_sys_lib.h>
#include <l/l_sys_io.h>

int main(void)
{
	ivi_printf("is_interactive() returns %d.\n", is_interactive());
	return EXIT_SUCCESS;
}

