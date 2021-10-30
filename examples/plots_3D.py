import csv
# from mpl_toolkits.mplot3d import Axes3D
# import matplotlib.pyplot as plt
import numpy as np
import math
import time
from termcolor import colored

import matplotlib.animation as animation
import matplotlib.pyplot as plt
import matplotlib.patches as ptch
import matplotlib.cm as cm
import matplotlib
from mpl_toolkits.mplot3d import Axes3D
from celluloid import Camera

from mpl_toolkits.mplot3d import art3d
from mpl_toolkits.mplot3d import proj3d
from matplotlib.patches import Circle
from itertools import product
from scipy.spatial.transform import Rotation as R

import sys
import os
from Config import Config

path = os.path.dirname(os.path.realpath('__file__'))
address = path + Config.Data_file

nd = 8 # num of data: [x, y, z, roll, pitch, Vx, Vy, Vz, x, y, ...]

# top and y-axis
az = -8
ele = 28

# top and x-axis
az = -77
ele = 28

#top with good ang
az = -110
ele = 41

# # xyz
# az = -108
# ele = 44

az = -99
ele = 10

def read_csv(address):
    global trans_time, exe_time, all_pos, goals, n
    trans_time, exe_time, all_pos, goals = [], [], [], []
    with open(address) as csvfile:
        readCSV = csv.reader(csvfile, delimiter=',')
        for row in readCSV:
            if 'Trans_Time' not in row:
                if 'Goals_x' in row:
                    goals.append([float(num) for num in row[2:]])
                else:
                    trans_time.append(float(row[0]))
                    exe_time.append(float(row[1]))
                    all_pos.append([float(num) for num in row[2:]])
        # print(len(trans_time))
        # print(np.shape(all_pos))
        goals = np.array(goals)
        all_pos = np.array(all_pos)
    n = int(len(all_pos[1])/nd)
    # print(len(all_pos))
    print('Length of video: ', trans_time[-1])

def plot_path3D():
    global ax
    start_time = time.time()
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    norm = matplotlib.colors.Normalize(vmin=0, vmax=n)
    for j in range(0, n):
        if Config.PLOT_DRONE:
            pos = [goals[0, nd*j], goals[0, nd*j+1], goals[0, nd*j+2]]
            ang = [0, 0, 0]
            plot_drone(pos, Config.radius, ang, cm.hsv(norm(j)))
            plot_num = Config.plot_num
            for i in range(plot_num):
                plot_time = int(i/plot_num*len(trans_time))
                pos = [all_pos[plot_time, nd*j], all_pos[plot_time, nd*j+1], all_pos[plot_time, nd*j+2]]
                ang = [all_pos[plot_time, nd*j+3], all_pos[plot_time, nd*j+4], 0]
                plot_drone(pos, Config.radius, ang, cm.hsv(norm(j)))
                # if Config.PLOT_CYL:
                #     plot_drone(pos, Config.radius, ang, cm.hsv(norm(j)), Config.PLOT_CYL_R, -Config.PLOT_CYL_H)
                # else:
        else:
            ax.scatter(goals[0, nd*j], goals[0, nd*j+1], goals[0, nd*j+2], s = 400, color = cm.hsv(norm(j)), marker="*", edgecolors= 'k')
    for j in range(0, n):
        ax.scatter(all_pos[0, nd*j], all_pos[0, nd*j+1], all_pos[0, nd*j+2], color = cm.hsv(norm(j)), marker="o", edgecolors= 'k')
        ax.plot(all_pos[:,nd*j], all_pos[:,nd*j+1],all_pos[:, nd*j+2], color = cm.hsv(norm(j)))
        # if Config.CYL_HEIGHT:
        #     u = np.linspace(0,2*np.pi,50) # divide the circle into 50 equal parts
        #     h = np.linspace(all_pos[0, nd*j+2] - Config.h_commun/2, all_pos[0, nd*j+2] + Config.h_commun/2,20) # divide the height 1 into 20 parts
        #     x = np.outer(Config.radius*np.sin(u)+all_pos[0, nd*j], np.ones(len(h))) # x value repeated 20 times
        #     y = np.outer(Config.radius*np.cos(u)+all_pos[0, nd*j+1],np.ones(len(h))) # y value repeated 20 times
        #     z = np.outer(np.ones(len(u)),h) # x,y corresponding height
        #     ax.plot_surface(x, y, z, color = cm.hsv(norm(j)))

    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.view_init(azim=az, elev=ele)
    print('Time to generate figure: ',  time.time() - start_time)
    if sys.argv[1] == str(110):
        plt.savefig(path + '/result' + Config.File_name +'.png')
        print(colored('Figure saved!',"red"))
    # set_axes_equal(ax)
    plt.show()

def plot_2D(items):  
    fig = plt.figure()

    if len(sys.argv) == 3:
        agent_id = int(sys.argv[2])
    else:
        agent_id = 0

    count = 0
    element = []
    var = []
    for item in items:
        if item in sys.argv[1]:
            if item == 'V':
                count += 3
                element.extend([items.index(item), items.index(item)+1, items.index(item)+2])
                var.extend(['V_x', 'V_y', 'V_z'])
            else:
                count += 1
                element.extend([items.index(item)])
                var.extend([item])
    
    v = 0
    for i in element:
        plt.subplot(count, 1, v+1)
        plt.plot(trans_time, all_pos[:, nd*agent_id+i])
        plt.title(var[v] + ' vs Time ') #(' + Config.File_name + ')')
        plt.xlabel('Transition time')
        plt.ylabel(var[v])
        plt.grid()

        v += 1

    plt.subplots_adjust(hspace=1)
    if 'save' in sys.argv[1]:
        plt.savefig(path + '/result' + Config.File_name + str(var) + '.png')
        print(colored('Figure saved!',"red"))
    plt.show()

def plot_xy(num1, num2):
    fig = plt.figure()
    norm = matplotlib.colors.Normalize(vmin=0, vmax=n)
    for j in range(0, n):
        plt.scatter(goals[0, nd*j+num1], goals[0, nd*j+num2], s=400, color = cm.hsv(norm(j)), marker="*")
        plt.plot(all_pos[:,nd*j+num1], all_pos[:,nd*j+num2], color = cm.hsv(norm(j)))
    plt.grid()
    plt.show()


def video_3D():
    global ax
    start_time = time.time()
    fig = plt.figure()
    camera = Camera(fig)
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    norm = matplotlib.colors.Normalize(vmin=0, vmax=n)
    print('freq cut down by a factor of: ', Config.videocut)
    for i in range(0, len(trans_time), Config.videocut):
        for j in range(0, n):
            if Config.PLOT_DRONE:
                pos = [all_pos[i, nd*j], all_pos[i, nd*j+1], all_pos[i, nd*j+2]]
                ang = [all_pos[i, nd*j+3], all_pos[i, nd*j+4], 0]
                plot_drone(pos, Config.radius, ang, cm.hsv(norm(j)))                
                # if Config.PLOT_CYL:
                #     plot_drone(pos, Config.radius, ang, cm.hsv(norm(j)), Config.radius*1.5, -Config.h_commun)
                # else:
            else:
                ax.scatter(all_pos[i, nd*j], all_pos[i, nd*j+1], all_pos[i, nd*j+2], color = cm.hsv(norm(j)), ec = 'k', marker="o")
                if Config.CYL_HEIGHT:
                    u = np.linspace(0,2*np.pi,50) # divide the circle into 50 equal parts
                    h = np.linspace(all_pos[i, nd*j+2] - Config.h_commun/2, all_pos[i, nd*j+2] + Config.h_commun/2,20) # divide the height 1 into 20 parts
                    x = np.outer(Config.radius*np.sin(u)+all_pos[i, nd*j], np.ones(len(h))) # x value repeated 20 times
                    y = np.outer(Config.radius*np.cos(u)+all_pos[i, nd*j+1],np.ones(len(h))) # y value repeated 20 times
                    z = np.outer(np.ones(len(u)),h) # x,y corresponding height
                    ax.plot_surface(x, y, z, color = cm.hsv(norm(j)))
            ax.scatter(goals[0, nd*j], goals[0, nd*j+1], goals[0, nd*j+2], color = cm.hsv(norm(j)), marker="*")
        # set_axes_equal(ax)
        ax.view_init(azim=az, elev=ele)

        camera.snap()
    
    animation = camera.animate(interval = Config.DeltaT*1000*Config.videocut)
    animation.save(path + '/result' + Config.File_name + '.mp4')
    print('Time to save video: ',  time.time() - start_time)

def plot_drone(trans_pos, size, ang, colorrr = None, rad = 0, h = 0):
    '''
    Given:
        trans_pos = [x,y,z]: center of the drone
        size: radius of drone
        ang = [roll, pitch, yaw]: orientation of the drone
        rad: communication radius
        h: communication height / of cylinder model (shall be neg)
    '''

    if Config.PLOT_CYL:
        rad = Config.PLOT_CYL_R
        h = -Config.PLOT_CYL_H

    r = R.from_euler('yxz', ang) # x:pitch, y:roll, z:yaw, change to fit for body frame
    # M = r.as_dcm()
    M = r.as_matrix()
    # M = np.linalg.inv(M)
    xy = (size/3*2)/np.sqrt(2)

    # Plot area of the whole drone
    if rad != 0:
        p = Circle((0,0), rad, facecolor = colorrr, alpha = 0.2)
    else:
        p = Circle((0,0), size, facecolor = colorrr, alpha = 0.2)
    ax.add_patch(p)
    pathpatch_2d_to_3d(p, angle=ang) # angle=[pitch, roll, yaw]
    pathpatch_translate(p, trans_pos)

    # plot top right rotor
    p = Circle((0,0), size/3, facecolor = 'r', alpha = 1) 
    ax.add_patch(p)
    pathpatch_2d_to_3d(p, angle=ang)
    pathpatch_translate(p, trans_pos)
    trans_r1 = np.dot(M,[xy,xy,0])
    pathpatch_translate(p, trans_r1)

    # plot top left rotor
    p = Circle((0,0), size/3, facecolor = 'r', alpha = 1) 
    ax.add_patch(p)
    pathpatch_2d_to_3d(p, angle=ang)
    pathpatch_translate(p, trans_pos)
    trans_r2 = np.dot(M,[-xy,xy,0])
    pathpatch_translate(p, trans_r2)

    # plot botton left rotor
    p = Circle((0,0), size/3, facecolor = 'b', alpha = 1) 
    ax.add_patch(p)
    pathpatch_2d_to_3d(p, angle=ang)
    pathpatch_translate(p, trans_pos)
    trans_r3 = np.dot(M,[-xy,-xy,0])
    pathpatch_translate(p, trans_r3)

    # plot botton right rotor
    p = Circle((0,0), size/3, facecolor = 'b', alpha = 1) 
    ax.add_patch(p)
    pathpatch_2d_to_3d(p, angle=ang)
    pathpatch_translate(p, trans_pos)
    trans_r4 = np.dot(M,[xy,-xy,0])
    pathpatch_translate(p, trans_r4)

    line1_pt1 = trans_pos+trans_r1
    line1_pt2 = trans_pos+trans_r3
    ax.plot([line1_pt1[0], line1_pt2[0]], [line1_pt1[1], line1_pt2[1]], [line1_pt1[2], line1_pt2[2]], color='k', linewidth=0.15*25)

    line2_pt1 = trans_pos+trans_r2
    line2_pt2 = trans_pos+trans_r4
    ax.plot([line2_pt1[0], line2_pt2[0]], [line2_pt1[1], line2_pt2[1]], [line2_pt1[2], line2_pt2[2]], color='k', linewidth=0.15*25)

    # Cylinder model (w/ tilted)
    u = np.linspace(0,2*np.pi, 50) # divide the circle into 50 equal parts
    h = np.linspace(0, h, 5)   # divide the height 1 into 20 parts
    x = np.outer(rad*np.sin(u), np.ones(len(h))) # x value repeated 20 times
    y = np.outer(rad*np.cos(u), np.ones(len(h))) # y value repeated 20 times
    z = np.outer(np.ones(len(u)), h) # x,y corresponding height

    xx = np.zeros(np.shape(x))
    yy = np.zeros(np.shape(y))
    zz = np.zeros(np.shape(z))

    for i in range(np.shape(xx)[0]):
        for j in range(np.shape(xx)[1]):
            # rotate + translate
            res = np.dot(M,[x[i,j], y[i,j], z[i,j]]) + trans_pos
            xx[i,j], yy[i,j], zz[i,j] = res[0], res[1], res[2]
            
    ax.plot_surface(xx, yy, zz, color = colorrr, alpha=0.5)


def pathpatch_2d_to_3d(pathpatch, angle=[0,0,0], trans = [0,0,0]):
    """
    Code from: https://stackoverflow.com/questions/18228966/how-can-matplotlib-2d-patches-be-transformed-to-3d-with-arbitrary-normals
    Transforms a 2D Patch to a 3D patch using the given normal vector.

    The patch is projected into they XY plane, rotated about the origin
    and finally translated by z.
    """
    path = pathpatch.get_path() #Get the path and the associated transform
    trans = pathpatch.get_patch_transform()

    path = trans.transform_path(path) #Apply the transform
    
    pathpatch.__class__ = art3d.PathPatch3D #Change the class
    pathpatch._code3d = path.codes #Copy the codes
    pathpatch._facecolor3d = pathpatch.get_facecolor #Get the face color    


    r = R.from_euler('yxz', angle) # x:pitch, y:roll, z:yaw
    # M = r.as_dcm()
    M = r.as_matrix()
    # pathpatch._segment3d = trans
    verts = path.vertices #Get the vertices in 2D

    # pathpatch._segment3d = np.array([np.dot(M, (x, y, 0)) + (0, 0, z) for x, y in verts])
    pathpatch._segment3d = np.array([np.dot(M, (x, y, 0)) for x, y in verts])

def set_axes_equal(ax):
    '''Make axes of 3D plot have equal plot_num so that spheres appear as spheres,
    cubes as cubes, etc..  This is one possible solution to Matplotlib's
    ax.set_aspect('equal') and ax.axis('equal') not working for 3D.

    Input
      ax: a matplotlib axis, e.g., as output from plt.gca().
    '''

    x_limits = ax.get_xlim3d()
    y_limits = ax.get_ylim3d()
    z_limits = ax.get_zlim3d()

    x_range = abs(x_limits[1] - x_limits[0])
    x_middle = np.mean(x_limits)
    y_range = abs(y_limits[1] - y_limits[0])
    y_middle = np.mean(y_limits)
    z_range = abs(z_limits[1] - z_limits[0])
    z_middle = np.mean(z_limits)

    # The plot bounding box is a sphere in the sense of the infinity
    # norm, hence I call half the max range the plot radius.
    plot_radius = 0.5*max([x_range, y_range, z_range])

    ax.set_xlim3d([x_middle - plot_radius, x_middle + plot_radius])
    ax.set_ylim3d([y_middle - plot_radius, y_middle + plot_radius])
    ax.set_zlim3d([z_middle - plot_radius, z_middle + plot_radius])


def pathpatch_translate(pathpatch, delta):
    """
    Translates the 3D pathpatch by the amount delta.
    """

    pathpatch._segment3d += delta

if __name__ == '__main__':
    print(Config.File_name)
    read_csv(address)

    items = ['x', 'y', 'z', 'roll', 'pitch', 'V']

    if len(sys.argv) == 1:
        print('Input needed!!')
        print('1: Save video.')
        print('110: Show and Save figure.')
        print('2D+xyzrollpitch: 2D plot, 3rd entry for agent id')
        print('xy: xy plane 2D plot of all agent')
        print('Otherwise, show figure')
    elif sys.argv[1] == str(1):
        video_3D()
    elif '2D' in sys.argv[1]:
        plot_2D(items)
    elif 'xy' in sys.argv[1]:
        plot_xy(0,1)
    elif 'xz' in sys.argv[1]:
        plot_xy(0,2)
    elif 'yz' in sys.argv[1]:
        plot_xy(1,2)
    else:
        plot_path3D()