/* =========================================================================== *
   |
   |  Copyright (c) 1994-2010 by Kobus Barnard (author)
   |
   |  Personal and educational use of this code is granted, provided that this
   |  header is kept intact, and that the authorship is not misrepresented, that
   |  its use is acknowledged in publications, and relevant papers are cited.
   |
   |  For other use contact the author (kobus AT cs DOT arizona DOT edu).
   |
   |  Please note that the code in this file has not necessarily been adequately
   |  tested. Naturally, there is no guarantee of performance, support, or fitness
   |  for any particular task. Nonetheless, I am interested in hearing about
   |  problems that you encounter.
   |
   |  Author:  Jinyan
 * =========================================================================== */

/* $Id$ */

#include <gr_cpp/gr_opengl.h>
#include <gr_cpp/gr_glut.h>
#include <gr_cpp/gr_primitive.h>
#include <m_cpp/m_vector.h>
#include <l_cpp/l_exception.h>
#include <string>
#include <vector>
#include <algorithm>
#include <exception>
#include <functional>
#include <cmath>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include "dbn_cpp/linear_state_space.h"

#ifndef IVI_HAVE_GLUT
#error "visualize_clo  program requires glut library"
#endif 

using namespace ivi;
using namespace std;
using namespace ivi::ties;

// declarations
//// glut functions
void reshape_window(int, int);
void display();
void process_key(unsigned char, int, int);

//// other crap
void update_points();
void draw_gp();
void draw_sidebar();

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

// variables that need to be global (accessed from several places)
std::vector<Vector> Ys;
Double_v times;
const size_t num_oscs = 2;
double T_1 = 10.0;
double T_2 = 20.0;
double w_1 = 2 * M_PI/T_1;
double w_2 =  2 * M_PI/T_2;
double theta_1 = 0.0;
double theta_2 = M_PI/3.0;
double d_1 = 0.0;
double d_2 = 0.0;


State_type init_state;

const int info_vp_width = 320;
const int main_vp_initial_width = 800;
int win_width = main_vp_initial_width + info_vp_width;
int win_height = main_vp_initial_width;
const double param_val_inc = 0.1;
const double param_val_inc_big = 0.5;
const double period_val_inc = 1.0;
const double period_val_inc_big = 5.0;
const double grid_sz = 1.0;
const double min_x = -0.1;
const double max_x = 50.1;
double min_y;
double max_y;
const double T = 50;
Linear_state_space lss;

// state variables
string param_names[] = { "theta_1", "theta_2", "w_1", "w_2", "d_1", "d_2"};
double* params[] = { &theta_1, &theta_2, &w_1, &w_2, &d_1, &d_2};
int cur_param = 0;
int cur_init_param = 0;
int param_type = 0;
size_t num_params;

bool points_up_to_date = false;

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

int main(int argc, char** argv)
{
    try
    {
        // create Linear_state_space
        std::vector<std::string> obs_names(1);
        obs_names[0] = "dial";
        times.resize(T);
        for(size_t i = 0; i < T; i++)
        {
            times[i] = i;
        }
        init_state.resize(2*num_oscs, 0.0);
        const int poly_degree = -1; 
        bool use_modal = true;
        size_t i = 0;
        init_state[i++] = 1.0;
        init_state[i++] = -1.0; 
        init_state[i++] = 0.0; 
        init_state[i++] = 0.0; 
        num_params = param_length(num_oscs, use_modal);
        std::cout << "num_params: " << num_params << std::endl;
        Double_v com_params((int)num_params, 0.0);
        for(size_t i = 0; i < com_params.size(); i++)
        {
            com_params[i] = *params[i];
            std::cout << i << ": " << com_params[i] << std::endl;
        }


        Coupled_oscillator_v clos(times.size() - 1, 
                                  Coupled_oscillator(com_params, use_modal));
        
        lss = Linear_state_space(times, 
                                 init_state, 
                                 clos, 
                                 obs_names, 
                                 Vector(1, 1.0), 
                                 poly_degree);
        update_points();

        // set up glut window
        opengl::Glut_window wnd(win_width, win_height, "CLO model");
        wnd.set_display_callback(display);
        wnd.set_reshape_callback(reshape_window);
        wnd.set_keyboard_callback(process_key);

        glutMainLoop();
    }
    catch(const Exception& kex)
    {
        kex.print_details();
        return EXIT_FAILURE;
    }
    catch(const exception& sex)
    {
        cerr << sex.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

void reshape_window(int w, int h)
{
    win_width = w;
    win_height = h;
}

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

void display()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    draw_gp();
    draw_sidebar();

    glutSwapBuffers();
}

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

void process_key(unsigned char key, int, int)
{
    switch(key)
    {
        case 'x':
        {
            if(param_type == 0)
            {
                if(cur_param == 2 || cur_param == 3)
                {
                    double period = 2 * M_PI/(*params[cur_param]);
                    period -= period_val_inc;
                    *params[cur_param] = 2 * M_PI/period;
                }
                else
                {
                    *params[cur_param] -= param_val_inc;
                }
            }
            else
            {
                init_state[cur_init_param] -= param_val_inc;
            }
            points_up_to_date = false;
            break;
        }

        case 'X':
        {
            if(param_type == 0)
            {
                if(cur_param == 2 || cur_param == 3)
                {
                    double period = 2 * M_PI/(*params[cur_param]);
                    period += period_val_inc;
                    *params[cur_param] = 2 * M_PI/period;
                }
                else
                {
                    *params[cur_param] += param_val_inc;
                }
            }
            else
            {
                init_state[cur_init_param] += param_val_inc;
            }
            points_up_to_date = false;
            break;
        }

        case 'z':
        {
            if(param_type == 0)
            {
                if(cur_param == 2 || cur_param == 3)
                {
                    double period = 2 * M_PI/(*params[cur_param]);
                    period -= period_val_inc_big;
                    *params[cur_param] = 2 * M_PI/period;
                }
                else
                {
                    *params[cur_param] -= param_val_inc_big;
                }
            }
            else
            {
                init_state[cur_init_param] -= param_val_inc;
            }
            points_up_to_date = false;
            break;
        }

        case 'Z':
        {
            if(param_type == 0)
            {
                if(cur_param == 2 || cur_param == 3)
                {
                    double period = 2 * M_PI/(*params[cur_param]);
                    period += period_val_inc;
                    *params[cur_param] = 2 * M_PI/period;
                }
                else
                {
                    *params[cur_param] += param_val_inc_big;
                }
            }
            else
            {
                init_state[cur_init_param] += param_val_inc;
            }
            points_up_to_date = false;
            break;
        }

        case 'k':
        {
            if(param_type == 0)
            {
                --cur_param;
                cur_param = (cur_param + num_params) % num_params;
            }
            else
            {
                --cur_init_param;
                cur_init_param = (cur_init_param + 4) % 4;
            }
            break;
        }

        case 'j':
        {
            if(param_type == 0)
            {
                ++cur_param;
                cur_param = cur_param % num_params;
            }
            else
            {
                ++cur_init_param;
                cur_init_param = cur_init_param % 4;
            }
            break;
        }
        case 't':
        {
            param_type = param_type == 0 ? 1 : 0; 
            break;
        }

        case 'r':
        {
            update_points();
            break;
        }

        case 's':
        {
            lss.write("lss.txt");
            break;
        }

        case 'q':
        {
            exit(EXIT_SUCCESS);
        }

        default:
            break;
    }

    if(*params[0] > M_PI/2.0)
    {
        *params[0] -= M_PI;
    }
    if(*params[0] < -M_PI/2.0)
    {
        *params[0] += M_PI;
    }
    if(*params[1] > M_PI/2.0)
    {
        *params[1] -= M_PI;
    }
    if(*params[1] < -M_PI/2.0)
    {
        *params[1] += M_PI;
    }

    //wnd.redisplay();
    glutPostRedisplay();
}

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

void update_points()
{
    // update the parameters
    if(param_type == 0)
    {
        BOOST_FOREACH(Coupled_oscillator& clo, lss.coupled_oscillators())
        {
            clo.set_param(cur_param, *params[cur_param]); 
        }
    }
    else
    {
        lss.init_state()[cur_init_param] = init_state[cur_init_param];
    }

    lss.changed_index() = 0;
    const State_vec_vec& states = lss.get_states();
    size_t length = lss.get_times().size();
    double min_val = DBL_MAX;
    double max_val = -DBL_MAX;
    assert(states.size() == T);
    Ys.resize(num_oscs, Vector((int)length, 0.0));
    
    for(size_t c = 0; c < num_oscs; c++)
    {
        for(size_t t = 0; t < T; t++) 
        {
            Ys[c][t] = states[t][0][c];
        }
        double cur_min_val = min(Ys[c]);
        double cur_max_val = max(Ys[c]);
        if(cur_min_val < min_val)
        {
            min_val = cur_min_val;
        }
        if(cur_max_val > max_val)
        {
            max_val = cur_max_val;
        }
    }
    //min_y = min_val - 0.1;
    //max_y = max_val + 0.1;
    min_y = -3.6;
    max_y = 3.6;

    points_up_to_date = true;
}

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

void draw_gp()
{
    // set up main viewport and stuff
    int main_vp_width = max(0, win_width - info_vp_width);
    glViewport(0, 0, (GLsizei)main_vp_width, (GLsizei)win_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(min_x, max_x, min_y, max_y);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw axes
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
        // x-axis
        glVertex2d(min_x, 0.0);
        glVertex2d(max_x, 0.0);

        // y-axis
        glVertex2d(0.0, min_y);
        glVertex2d(0.0, max_y);
    glEnd();

    // draw grid
    glColor3f(0.1, 0.1, 0.1);
    glLineWidth(1.0);
    double mnx = floor(min_x / grid_sz) * grid_sz;;
    double mxx = ceil(max_x / grid_sz) * grid_sz;;
    double mny = floor(min_y / grid_sz) * grid_sz;;
    double mxy = ceil(max_y / grid_sz) * grid_sz;;
    opengl::render_xy_plane_grid(mnx, mxx, mny, mxy, grid_sz);

    // draw gt curve
    for(size_t c = 0; c < num_oscs; c++)
    {
        double r = c == 0 ? 1.0 : 0.0;
        double g = c == 0 ? 0.0 : 1.0;
        glColor3f(r, g, 0.0);
        glLineWidth(1.0);
        glBegin(GL_LINE_STRIP);
        for(size_t i = 0; i < Ys[c].size(); ++i)
        {
            glVertex2d(times[i], Ys[c][i]);
        }
        glEnd();
    }

    // draw min_y and max_y
    string temp_str;
    boost::format fmt("%0.3f");
    using opengl::bitmap_string;
    glColor3f(0.75, 0.75, 0.75);
    temp_str = boost::str(fmt % min_y);
    bitmap_string(temp_str, 0, min_y + 0.1);
    temp_str = boost::str(fmt % max_y);
    bitmap_string(temp_str, 0, max_y - 0.1);
}

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

void draw_sidebar()
{
    // set up sidebar viewport
    int main_vp_width = max(0, win_width - info_vp_width);
    glViewport(main_vp_width, 0, (GLsizei)info_vp_width, (GLsizei)win_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, info_vp_width, win_height, 0.0);
    glMatrixMode(GL_MODELVIEW);

    // draw text
    using opengl::bitmap_string;
    boost::format fmt("%0.3f");

    glLoadIdentity();
    glColor3f(0.75, 0.75, 0.75);
    glBegin(GL_QUADS);
    glVertex2d(0.0, 0.0);
    glVertex2d(0.0, win_height);
    glVertex2d(info_vp_width, win_height);
    glVertex2d(info_vp_width, 0.0);
    glEnd();

    glColor3f(0.2, 0.2, 0.7);

    const int margin_size = 12;
    const int line_width = 20;
    const int str_pos_x = margin_size;
    int str_pos_y = line_width;
    string temp_str;

    // draw text for current parameter information
    for(size_t p = 0; p < num_params; p++)
    {
        if(param_type == 0 && cur_param == p) glColor3f(1.0, 0.0, 0.0);
        else glColor3f(0.2, 0.2, 0.7);
        temp_str = param_names[p] + ": " + boost::str(fmt % *params[p]);
        bitmap_string(temp_str, str_pos_x, str_pos_y);
        str_pos_y += line_width;
    }
    for(size_t p = 0; p < 4; p++)
    {
        if(param_type == 1 && cur_init_param == p) glColor3f(1.0, 0.0, 0.0);
        else glColor3f(0.2, 0.2, 0.7);
        temp_str = boost::lexical_cast<std::string>(init_state[p]);
        bitmap_string(temp_str, str_pos_x, str_pos_y);
        str_pos_y += line_width;
    }

    // draw separating line
    glColor3f(0.2, 0.2, 0.7);
    glBegin(GL_LINES);
    glVertex2i(0.0, str_pos_y);
    glVertex2i(info_vp_width, str_pos_y);
    glEnd();
    str_pos_y += line_width;

    // draw text for control information
    bitmap_string("HELP", str_pos_x, str_pos_y);
    str_pos_y += line_width;

    temp_str = "Change parameter: j/k";
    bitmap_string(temp_str, str_pos_x, str_pos_y);
    str_pos_y += line_width;

    temp_str = "Modify current parameter by "
                    + boost::str(fmt % param_val_inc) + ": x/X";
    bitmap_string(temp_str, str_pos_x, str_pos_y);
    str_pos_y += line_width;

    temp_str = "Modify current parameter by "
                    + boost::str(fmt % param_val_inc_big) + ": z/Z";
    bitmap_string(temp_str, str_pos_x, str_pos_y);
    str_pos_y += line_width;

    if(!points_up_to_date)
    {
        glColor3f(1.0, 0.0, 0.0);
    }
    bitmap_string("Re-draw: r", str_pos_x, str_pos_y);
    str_pos_y += line_width;

    temp_str = "Generate data: g";
    bitmap_string(temp_str, str_pos_x, str_pos_y);
    str_pos_y += line_width;

    glColor3f(0.2, 0.2, 0.7);
    bitmap_string("Quit: q", str_pos_x, str_pos_y);
    str_pos_y += line_width;

    // draw separating line
    glColor3f(0.2, 0.2, 0.7);
    glBegin(GL_LINES);
    glVertex2i(0.0, str_pos_y);
    glVertex2i(info_vp_width, str_pos_y);
    glEnd();
    str_pos_y += line_width;

}
