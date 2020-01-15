#!/usr/bin/env python2.7

# $Id$
# Language: python 2.7
# This scripts is adapted from ties_plot.py writtien by Andrew Predoehl 
#

import argparse
import glob
import matplotlib.figure, matplotlib.backends.backend_agg
import numpy, numpy.ma
import os, os.path


def _draw_basic_plot(ax, title, x, y):
    '''
    Plot the four curves on the plot-axes, and label the axes.
    '''
    handles = ax.plot(x, y, 'r')
    ax.set_title(title)
    ax.set_xlabel('Iterations')
    ax.set_ylabel('Log posteriors')

    return handles


def _plot_(title, x, y): #, legend_str):
    'Generate and return a figure, on which we draw the basic plot.'
    #fig = matplotlib.figure.Figure(figsize=(30, 10))
    fig = matplotlib.figure.Figure()
    ax = fig.add_subplot(111)
    _draw_basic_plot(ax, title, x, y)
    #fig.legend(h, (legend_str), loc='center right')
    return fig


def _gen_a_plot(title, plot_path_stem, data_fp):
    '''
    Inputs correspond to one dyad, one flavor of model, one observable cateory,
    and we want to plot the timeseries of both, overlaid together.  The plot
    is saved to files, with path and filename glued together from the inputs,
    and in a few formats (currently PNG, EPS, and CSV).

    String 'plot_path_stem' is part of the pathname of the output file -- it
    still lacks a filetype extension.

    '''

    print data_fp
    print plot_path_stem
    ## read in the ll.txt file 
    ll = numpy.loadtxt(data_fp);
    x = numpy.arange(len(ll))
    y = ll
    fig = _plot_(title, x, ll)  
    canvas = matplotlib.backends.backend_agg.FigureCanvasAgg(fig)
    canvas.print_figure(filename=plot_path_stem + '.eps')
    canvas.print_figure(filename=plot_path_stem + '.png')

def generate_individual_plots(args):

    glob4d = '[0-9][0-9][0-9][0-9]' # used for dyad identification number
    fold_states = os.path.join('fold-[0-9][0-9]', 'testing', glob4d)
    model_dir = args.model_dir
    print os.path.join(model_dir, fold_states)
    fold_paths = glob.glob(os.path.join(model_dir, fold_states))
    for fold_dir in fold_paths:
        #_gen_a_plot('Training Posteriors changes',
            #os.path.join(fold_dir, 'training', 'll'), 
            #os.path.join(fold_dir, 'training', 'll.txt'))
        _gen_a_plot('Testing Posteriors changes',
            os.path.join(fold_dir, 'll'), 
            os.path.join(fold_dir, 'll.txt'))

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Plot the posteriors')
    parser.add_argument('--model-dir',
        help='Model directory(Required)')
    args = parser.parse_args()
    generate_individual_plots(args)
    print "finish" 

