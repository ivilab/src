/* $Id: sift_keypoints.c 25592 2020-06-29 19:12:01Z kobus $
 */
#include "slic/sift_keypoints.h"
#include "i/i_float_io.h"
#include "i/i_draw.h"

int get_target_SIFT_kp(SIFT_kp **kpp, float row, float col, float scale, 
                       float orientation, Int_vector* vp)
{
    SIFT_kp *kp;

    if (*kpp ==  NULL)
    {
        *kpp = (SIFT_kp*)ivi_malloc(sizeof(SIFT_kp));
    }
    kp = *kpp;

    kp->row = row;
    kp->col = col;
    kp->scale = scale;
    kp->orientation = orientation;
    kp->elements = vp;

    return NO_ERROR;
}


int get_target_SIFT_Kp_list(SIFT_kp_list **sift_kp_list_pp, int num_elements)
{
    if (*sift_kp_list_pp == NULL)
    {
        free_SIFT_kp_list(*sift_kp_list_pp);
        *sift_kp_list_pp = NULL;
    }
    (*sift_kp_list_pp)->elements = (SIFT_kp **)ivi_malloc(sizeof(SIFT_kp*) * num_elements);

    return NO_ERROR;
}


/* =============================================================================
 *                              read_SIFT_kp_list
 *
 *  Reads SIFT keypoints from a file.
 *
 *  This method reads in SIFT keypoints from a file (as generated by David 
 *  Lowe's sift program)
 *
 *  Returns:
 *    On error, this routine returns ERROR, with an error message being set.
 *    On success,  it returns a NO_ERROR 
 *
 *  Author: Qiyam Tung
 *  
 *  Index: sift keypoints
 *
 * -----------------------------------------------------------------------------
 */

int read_SIFT_kp_list(SIFT_kp_list **sift_kp_list_pp, char *keypoint_filename)
{
    int items_read;
    int num_keypoints;
    float row, col, scale, orientation;
    /* It is 128, but SIFT does not need to return 128-bit vectors,
     * so I'm leaving it as a variable*/
    int kp_desc_len;
    int i,j;
    Int_vector *vp= NULL;
    SIFT_kp *sift_kp = NULL;
    SIFT_kp_list *sift_kp_list_p = NULL;
    FILE *fp = NULL;


    NRE(fp = ivi_fopen(keypoint_filename, "r"));

    items_read = fscanf (fp, "%d %d", &num_keypoints, &kp_desc_len);  
    if (items_read != 2)
    {
        set_error("fscanf did not read in 2 items (num keypoints and length of keypoint descriptor");
        return ERROR;
    }    


    /* List creation/checks*/
    if (*sift_kp_list_pp == NULL)
    {
        free_SIFT_kp_list(*sift_kp_list_pp);
        *sift_kp_list_pp = NULL;
    }
    *sift_kp_list_pp = (SIFT_kp_list*)ivi_malloc(sizeof(SIFT_kp_list));
    sift_kp_list_p = *sift_kp_list_pp;
    sift_kp_list_p->elements = (SIFT_kp **)ivi_malloc(sizeof(SIFT_kp*) * num_keypoints);  

    /* Read in all the SIFT keypoints from the file*/
    for (i = 0; i < num_keypoints; i++)
    { 
        items_read = fscanf(fp, "%f %f %f %f", &row, &col, &scale, &orientation);

        if (items_read != 4)
        {
            set_error("fscanf did not read in the position, scale, and orientation.");
            return ERROR;
        } 
        ERE(get_target_int_vector(&vp, kp_desc_len));

        /* Read in all the descriptor values */
        for (j = 0; j < kp_desc_len; j++)
        {
            items_read = fscanf(fp, "%d", &(vp->elements[j]));
            if (items_read != 1)
            {
                set_error("fscanf could not read descriptor value");
                return ERROR;  
            }
        }
        ERE(get_target_SIFT_kp(&sift_kp, row, col, scale, orientation, vp));

        sift_kp_list_p->elements[i] = sift_kp;
    }

    return NO_ERROR;
}

/* =============================================================================
 *                              free_SIFT_kp_list
 *
 *  Frees SIFT keypoints list.
 *
 *  Frees SIFT keypoints list.
 *
 * Returns:
 *    On error, this routine returns ERROR, with an error message being set.
 *    On success,  it returns a NO_ERROR 
 *
 *  Author: Qiyam Tung
 *  
 *  Index: sift keypoints
 *
 * -----------------------------------------------------------------------------
 */
int free_SIFT_kp_list(SIFT_kp_list *sift_kp_p)
{
    int i;
    for (i = 0; i < sift_kp_p->length; i++)
    {
        ERE(free_SIFT_kp(sift_kp_p->elements[i]));
    }

    ivi_free(sift_kp_p);
    return NO_ERROR;
}

/* =============================================================================
 *                              free_SIFT_kp
 *
 *  Frees a SIFT keypoint.
 *
 *  Frees a SIFT keypoint.
 *
 * Returns:
 *    On error, this routine returns ERROR, with an error message being set.
 *    On success,  it returns a NO_ERROR 
 *
 *  Author: Qiyam Tung
 *  
 *  Index: sift keypoints
 *
 * -----------------------------------------------------------------------------
 */

int free_SIFT_kp(SIFT_kp *sift_kp)
{
    ivi_free(sift_kp);

    return NO_ERROR;
}


#ifdef MIGHT_BE_DEAD

/* =============================================================================
 *                              draw_keypoints_on_image
 *
 *  Draws SIFT keypoints on an image. 
 *
 * Returns:
 *    On error, this routine returns ERROR, with an error message being set.
 *    On success,  it returns a NO_ERROR 
 *
 *  Author: Qiyam Tung
 *  
 *  Index: sift keypoints
 *
 * -----------------------------------------------------------------------------
 */
    
int draw_keypoint (IVI_image* ip, int x, int y ) 
{
    /* How can this possibly do what it is supposed to? !!! */

    /* All the code in this directory might be dead anyway. Perhaps lib/kpt has 
     * what one is looking for. 
     */
    UNTESTED_CODE();

    EPETE(image_draw_point(ip, 200, 60, 5, 200, 0, 0));

    EPETE(image_draw_circle(ip, 200, 60, 70, 1, 200, 0, 0));

    /* EPETE(ivi_display_image(ip, NULL)); */

    return NO_ERROR;
} 



int draw_keypoints_on_image(char *image_filename, char *keypoints_filename)
{
    int length,i;
    IVI_image *ip =NULL;

    SIFT_kp_list *sift_kp_list =NULL;

    ERE(ivi_read_image(&ip, image_filename));
    ERE(read_SIFT_kp_list(&sift_kp_list, keypoints_filename));
    length = sift_kp_list->length;
    for (i = 0; i < length; i++)
    {
        /* The following line generates an error that states
         * "invalid argument type of '->' (have 'SIFT_kpt_list') 
         *
         * draw_keypoint(ip, sift_kp_list[i]->row, sift_kp_list[i]->col); 
         */
        draw_keypoint(ip, sift_kp_list->elements[i]->row, sift_kp_list->elements[i]->col); 
    }


    return NO_ERROR;
}

#endif
