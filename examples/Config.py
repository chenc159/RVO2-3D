import os
import re
import numpy as np
import time
from termcolor import colored


class Config:

    ALG_MODE = 'Flocking'
    File_name = '/test' # File name used to save data
    Data_file = File_name + '.csv'
    videocut = 5 # freq to cut video shorter (e.g. 5 frame/sec to 1 frame/sec)
    env_dimention = 3 # Don't change this

    radius, height = 1.5, 1.5

    # radius, height, MaxVelo, MaxAcc = 0.47/(2**0.5), 0.11, 10.0, 10.0
    # # radius, height, MaxVelo, MaxAcc = 0.15, 0.10, 10.0, 10.0
    r_alpha, h_alpha = 1.16, 1.16 # 0.45, 0.57
    # # r_alpha, h_alpha = 1.16, 0.5 # 0.45, 0.57
    # r_alpha_max, r_alpha_min = r_alpha, radius*2+0.05
    # h_alpha_max, h_alpha_min = h_alpha, 0.20

    # MaxSimTime, DeltaT, EndMaxDis = 20.0, 0.01, 0.05
    # z_min = 0.0 # for crashing to the ground
    # RepulsiveGradient = 7.5*(10**10) # for flocking repulsive force
    # Loop = 5 # iterations to run (for random generated initial/final locations)
    # sed = 1 # random seed for initial/final locations generation

    # world = np.array([[0,1,0], [1,0,0], [0,0,-1]]) # rotation from world to body ...
    # init_gap = radius*3.0

    '''Setup agent number'''
    agent_number = 4

    '''Dynamics'''
    DYN = True
    DYN = False

    '''Bump Function'''
    BUMP = True
    BUMP = False
    h_bump = 0.5

    '''Repulsive force cylinder model?'''
    CYL_MODEL = True
    CYL_MODEL = False   
    '''Adaptive height'''
    ADAPT_H = True
    ADAPT_H = False
    '''Adaptive model (height+radius)''' # ADAPT_H and ADAPT_RH can't be both true
    ADAPT_RH = True
    ADAPT_RH = False

    '''Plot drones? for plots_3D.py'''
    PLOT_DRONE = True
    PLOT_DRONE = False
    PLOT_CYL = True
    PLOT_CYL = False
    PLOT_CYL_R = radius #r_alpha #radius
    PLOT_CYL_H = h_alpha #radius*4.0
    plot_num = 3 # number of drones image per drone

    '''Test dw force?'''
    TEST_DW_F = True
    # TEST_DW_F = False

    '''Test tilted dw model?'''
    DW_TILT_CYL = True
    DW_TILT_CYL = False

    '''Test torque produced by dw?'''
    TORQUE_FLIP = True
    TORQUE_FLIP = False

    '''Test zero navigational feedback?'''
    ZERO_NAV_F = True
    ZERO_NAV_F = False