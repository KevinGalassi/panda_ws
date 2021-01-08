""" Testing MOdule """
import struct
import rospy
import sys
import xml.etree.ElementTree as ET
import numpy as np
import PyKDL
import math
from PyKDL import Frame
import database_wiress.utils.transformations as transformations
import database_wiress.utils.gc as gc
import copy
import os
import tf

""" GLOBAL PARAMETERS"""

PARTDB_CAD_DEFAULT_WORLD_NAME = "world"

###############################################################
###############################################################
###############################################################


class GearboxProxy(object):

    tf_listener = None

    @staticmethod
    def init(tf_listener=None):
        if tf_listener == None:
            GearboxProxy.tf_listener = tf.TransformListener()
        else:
            GearboxProxy.tf_listener = tf_listener

    @staticmethod
    def filterInterestingTransforms(tf_list):
        filtered = []
        for tf in tf_list:
            if tf.startswith("I_"):
                filtered.append(tf)
            if tf.startswith("C_"):
                filtered.append(tf)
        return filtered

    @staticmethod
    def retrieveGearboxTFTree():
        if GearboxProxy.tf_listener == None:
            return None

        channels = []
        for i in range(1, 7):
            channels.append('-U{}'.format(i))

        intersections = GearboxProxy.filterInterestingTransforms(
            GearboxProxy.tf_listener.getFrameStrings())

        int_frames = {}
        for intersection in intersections:
            frame = transformations.retrieveTransform(
                GearboxProxy.tf_listener, PARTDB_CAD_DEFAULT_WORLD_NAME, intersection, print_error=True)
            if frame:
                int_frames[intersection] = frame
            else:
                return None

        temp = {}
        for key, value in int_frames.iteritems():
            temp[key + "#gateway"] = transformations.cloneFrame(value)
            temp[key + "#gateway"] = temp[key + "#gateway"] * \
                PyKDL.Frame(PyKDL.Vector(-0.3, 0.0, 0.0))

        int_frames.update(temp)

        temp = {}
        for key, value in int_frames.iteritems():
            temp[key + "#N"] = transformations.cloneFrame(value)
            temp[key + "#N"].M.DoRotZ(1.57)
            temp[key + "#W"] = transformations.cloneFrame(value)
            temp[key + "#W"].M.DoRotZ(3.14)
            temp[key + "#S"] = transformations.cloneFrame(value)
            temp[key + "#S"].M.DoRotZ(3.14 + 1.57)
            temp[key + "#E"] = value

        int_frames.update(temp)

        return int_frames
