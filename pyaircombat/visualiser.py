import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from typing import Dict
import numpy as np
import os

os.environ['KMP_DUPLICATE_LIB_OK'] = 'True'

class FigureVisualiser(object):
    """ Class for manging a matplotlib Figure displaying agent state and actions """
    PLOT_PAUSE_SECONDS = 0.0001

    def __init__(self):
        self.figure = None
        self.axes = None
        self.previous_props = {}
        self.agent_texts = {}

    def plot(self, planes: Dict) -> None:
        """
        Creates or updates a 3D plot of the episode.

        :param sim: Simulation that will be plotted
        """
        if not self.figure:
            self.figure, self.axes = self._plot_configure()
        while self.axes.lines:
            data = self.axes.lines.pop()
            del data
        self._plot_trajectorys(planes, self.axes)
        plt.pause(self.PLOT_PAUSE_SECONDS)  # voodoo pause needed for figure to update

    def reset(self):
        self.previous_props = {}
        self.agent_texts = {}
        self.agent_shapes = {}
        while self.axes.lines:
            data = self.axes.lines.pop()
            del data
        while self.axes.texts:
            data = self.axes.texts.pop()
            del data

    def _plot_configure(self):
        plt.ion()
        figure = plt.figure(figsize=(8, 8))
        axes_trajectory = plt.axes(projection='3d')
        axes_trajectory.set_xlabel('y')
        axes_trajectory.set_ylabel('x')
        axes_trajectory.set_zlabel('z')
        axes_trajectory.set_zlim(0, 15000)
        plt.show()
        plt.pause(self.PLOT_PAUSE_SECONDS)  # voodoo pause needed for figure to appear

        return figure, axes_trajectory

    def _plot_trajectorys(self, planes, axes_trajectory: plt.Axes):
        for plane_id, plane in planes.items():
                agentid = plane.tag
                position_x, position_y, altitude = plane.get_xyz_pos()
                plane_marker = 'o' if plane.is_alive() else 'x'
                if agentid in self.previous_props:
                    self.previous_props[agentid].append([position_x, position_y, altitude])
                    a = np.array(self.previous_props[agentid])
                    axes_trajectory.plot(a[:, 0], a[:, 1], a[:, 2], agentid[0], mfc='none', markersize=1, clip_on=False,
                                         linewidth=2,
                                         alpha=0.6)
                    axes_trajectory.plot(position_x, position_y, altitude, f'{agentid[0]}' + plane_marker, mfc='none',
                                         markersize=5,
                                         clip_on=False)
                    if plane.is_locked():
                        axes_trajectory.plot(position_x, position_y, altitude, f's',
                                             color='gray',
                                             mfc='none', markersize=10,
                                             clip_on=False)
                    self.agent_texts[agentid].set_position_3d((position_x, position_y, altitude))
                else:
                    self.previous_props[agentid] = []
                    axes_trajectory.plot(position_x, position_y, altitude, 'bo', mfc='none', markersize=3,
                                         clip_on=False)
                    self.previous_props[agentid].append([position_x, position_y, altitude])
                    self.agent_texts[agentid] = axes_trajectory.text(position_x, position_y, altitude, s=f'{agentid}',
                                                                     fontsize=10)

                for missile_id, missile in enumerate(plane.targeted_missiles):
                    agentid = missile.tag
                    position_x, position_y, altitude = missile.get_pos()
                    if agentid in self.previous_props:
                        self.previous_props[agentid].append([position_x, position_y, altitude])
                        a = np.array(self.previous_props[agentid])
                        axes_trajectory.plot(a[:, 0], a[:, 1], a[:, 2], agentid[0] + '--', mfc='none', markersize=1,
                                             clip_on=False,
                                             alpha=0.4)
                        axes_trajectory.plot(position_x, position_y, altitude, f'{agentid[0]}^', mfc='none',
                                             markersize=2,
                                             clip_on=False)
                        self.agent_texts[agentid].set_position_3d((position_x, position_y, altitude))
                    else:
                        self.previous_props[agentid] = []
                        axes_trajectory.plot(position_x, position_y, altitude, 'bo', mfc='none', markersize=3,
                                             clip_on=False)
                        self.previous_props[agentid].append([position_x, position_y, altitude])
                        self.agent_texts[agentid] = axes_trajectory.text(position_x, position_y, altitude,
                                                                         s=f'{agentid[3:]}',
                                                                         fontsize=8)
