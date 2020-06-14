/* $Id: is_interactive.c 25499 2020-06-14 13:26:04Z kobus $
 */
#include <l/l_sys_lib.h>
#include <l/l_sys_io.h>

int main(int argc, char** argv)
{
	ivi_printf("is_interactive() returns %d.\n", is_interactive());
	return EXIT_SUCCESS;
}

