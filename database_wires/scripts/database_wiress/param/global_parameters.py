#!/usr/bin/env python
# -*- encoding: utf-8 -*-

import rospy

"""Example: 
NODE:       node_rate = Parameters.get(obj=robot_name, param="NODE_FREQUENCY")
            or 
            world_id = Parametrs.get("WORLD_ID")
            or
            Parameters.change(obj=robot_name, 
                                param="NODE_FREQUENCY", 
                                value=hz) 
            or
            Parameters.change(param="WORLD_ID", 
                              value=tf_name) 
                                
LAUNCH:     <param name="$(arg robot_name)/TRAJECTORY_TIME" value="5"/>
"""


class Parameters:

    elements = {

        #⬢⬢⬢⬢⬢➤ Robots
        "COMAU_NAME":                   "comau_smart_six",
        "BOGIE_NAME":                   "bogie",
        "GRASSHOPPER_NAME":             "grasshopper",
        "SCHUNK_NAME":                  "schunk_pg70",
        "BONMET_NAME":                  "bonmetc60"

        #⬢⬢⬢⬢⬢➤ Sensors
        # TODO

        #⬢⬢⬢⬢⬢➤ Actions
        # TODO
    }

    default_params = {


        # ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇ ROBOTS ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇

        #⬢⬢⬢⬢⬢➤ COMAU
        elements["COMAU_NAME"] + "/TRAJECTORY_TIME":    5,
        elements["COMAU_NAME"] + "/NODE_FREQUENCY":     300,
        elements["COMAU_NAME"] + "/TOOLS":              {
            "none": [0, 0, 0, 0, 0, 0, 1],
            "gripper": [0, 0, -0.2965, 0, 0, 0, 1],
            "camera": [-0.003263, 0.141871, -0.216756, -3.404999, 0.040769, 0.006786, -0.052411],
            "dynamic": [0, 0, 0, 0, 0, 0, 1],
        },

        #⬢⬢⬢⬢⬢➤ BOGIE
        elements["BOGIE_NAME"] + "/TRAJECTORY_TIME":        20,
        elements["BOGIE_NAME"] + "/NODE_FREQUENCY":         100,
        elements["BOGIE_NAME"] + "/TOOLS":              {
            "none": [0, 0, 0, 0, 0, 0, 1],
            "gripper": [0, 0, 0, 0, 0, 0, 1],
            "camera": [0, 0, 0, 0, 0, 0, 1],
            "dynamic": [0, 0, 0, 0, 0, 0, 1],
        },


        #⬢⬢⬢⬢⬢➤ GRASSHOPPER
        elements["GRASSHOPPER_NAME"] + "/TRAJECTORY_TIME":  20,
        elements["GRASSHOPPER_NAME"] + "/NODE_FREQUENCY":   100,
        elements["GRASSHOPPER_NAME"] + "/TOOLS":              {
            "none": [0, 0, 0, 0, 0, 0, 1],
            "gripper": [0, 0, 0, 0, 0, 0, 1],
            "camera": [0, 0, 0, 0, 0, 0, 1],
            "dynamic": [0, 0, 0, 0, 0, 0, 1],
        },


        #⬢⬢⬢⬢⬢➤ SCHUNK
        elements["SCHUNK_NAME"] + "/TRAJECTORY_TIME":       1,
        elements["SCHUNK_NAME"] + "/NODE_FREQUENCY":        2,
        elements["SCHUNK_NAME"] + "/TOOLS":              {
            "none": [0, 0, 0, 0, 0, 0, 1],
            "gripper": [0, 0, 0, 0, 0, 0, 1],
            "camera": [0, 0, 0, 0, 0, 0, 1],
            "dynamic": [0, 0, 0, 0, 0, 0, 1],
        },


        #⬢⬢⬢⬢⬢➤ BONMET
        elements["BONMET_NAME"] + "/NODE_FREQUENCY":       300,     # [hz]
        elements["BONMET_NAME"] + "/TRAJECTORY_TIME":      5,       # [sec]
        elements["BONMET_NAME"] + "/TRAJECTORY_POINTS":    1,       # []
        elements["BONMET_NAME"] + "/TOOLS": {
            "none": [0, 0, 0, 0, 0, 0, 1],
            "gripper": [0, -0.068, 0.37, 0, 0, 1, 0],
            "camera": [-0.002746, -0.117399, 0.297752, 0.040049, 0.016541, 0.019843, 1.01652],
            "dynamic": [0, 0, 0, 0, 0, 0, 1],
        },


        # ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇ SENSORS ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
        # TODO

        # ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇ ACTIONS ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇  they may be "subtasks"
        # TODO

        # ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇ WIRES ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
        "IDS_SCAN_LIST":                ["ID000045", "ID000049", "ID000050", "ID000053", "ID000064", "ID000066", "ID000068"],  # "ID000051","ID000062",
        "CABLE_LENGTH":                 0.02,  # must be >0.01[m], this value may be changed by vision estimation

        # CONSTANTS 
        "STD_CABLE_LENGTH":             0.02,
        "MAX_CABLE_LENGTH":             0.03,
        "MIN_CABLE_LENGTH":             0.01, 
        "HOLE_X_OFFSET":                -0.001,  # horizontal
        "HOLE_Y_OFFSET":                0.003,    # vertical
        "HOLE_Z_OFFSET":                0.004,    # hole-axis
        "UNSCREW_TIME":                 0.8,  # [s]
        "BUFFER_VISION_WIRE_OFFSET":    10,
        "MEAN_VISION_WIRE_OFFSET":      True,

        "END_OF_STROKE_LOW":                        0.0011,
        "END_OF_STROKE_HIGH":                       0.0339,
        "CABLE_PUSH_ADVANCE_DIST":                  0.001,
        "CABLE_PULL_RETRIEVE_DIST":                 0.034,
        "CABLE_PUSH_RETRIEVE_DIST_TACTILE_STOP":    0.001,
        "CABLE_LENGTH_ADDITIONAL_OFFSET":           0.01,
        "GRASSHOPPER_POSITION_SISTEMATIC_OFFSET":   0.01,
        "THRESHOLD_PUSH":                           0.1, #0.22
        "THRESHOLD_PULL":                           0.08, #0.20
        "THRESHOLD_TACTILE_RESET":                  0.005, #0.02

        "GRIPPER_APERTURE":                         0.02,
        "GRIPPER_CLOSURE":                          0.004,
        "GRIPPER_CLOSURE_SOFT":                     0.004,
 

        # ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇ OTHERS ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
        "DATA_FOLDER":                  "/media/riccardo/87060f92-ea5c-42c7-bb3e-5662219d0bd0/projects/wires",
        "PACKAGE_DATA_FOLDER":          "/home/riccardo/ros/wires_ws/src/wires_robotic_platform/data",
        "WORLD_FRAME_ID":               "world"
    }

    # ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
    # ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
    # ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇

    @staticmethod
    def get(param, obj=""):
        """ Ritorna il valore del parametro richiesto.  """

        try:
            if param in Parameters.elements.keys():    # a name is requested
                return rospy.get_param(param, Parameters.elements[param])
            elif obj in Parameters.elements.values():   # a elements's parameter is requested
                param = "{}/{}".format(obj, param)
            return rospy.get_param(param, Parameters.default_params[param])
        except Exception as e:
            print(e)
            return None

    @staticmethod
    def change(param, value, obj=""):
        """ Ritorna il valore del parametro richiesto. """
        try:
            if obj in Parameters.elements.values():   # a elements's parameter is requested
                param = "{}/{}".format(obj, param)
            rospy.set_param(param, value)
            return True
        except Exception as e:
            print(e)
            return False
