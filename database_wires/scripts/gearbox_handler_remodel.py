#!/usr/bin/env python
# coding=utf-8
"""
This nodes is a testing node to show CAD Library tools to Load XML files and show result in RVIZ and as STL files
"""
from database_wiress.partdb.cad import Scene, Shape, Face, STLUtils, Transform, Part, Terminal, PartCollection, Gearbox, Channel, ItemType
from database_wires.srv import *
from database_wires.msg import *
import database_wiress.utils.transformations as transformations
import database_wiress.utils.visualization as visualization
from database_wiress.partdb.data_dictionaries import GearboxPartsData

import PyKDL
import numpy as np
import rospy
import json
import re
import pprint
import copy
from visualization_msgs.msg import Marker
from visualization_msgs.msg import MarkerArray
from geometry_msgs.msg import Point, Pose
from std_msgs.msg import String
from std_srvs.srv import Trigger, TriggerResponse
import random
import math
import tf
import time

from enum import Enum

COMPLETE_KEYWORD = 'COMPLETE: '
ERROR_KEYWORD = 'ERROR: '

# Definition of the state of the system
class State(Enum):
    STAND_BY = 1            #idle system
    VISUAL = 2              #visualization of component in list
    CONNECTION = 3          #visualization of connection in list
    TARGET = 4              #visualization of target cad 
    FULL_TARGET = 5         #visualization of target cad + screws/hole
    SCREW = 6               #visualization of screws w.r.t. target
    SCREW_CORRECTED = 7     #visualization of screws adapted w.r.t. visual servoing control of the robot

#ROS COMPONENT INITIALIZATION
#
#
#
#
#
# MARKER ARRAY TOPIC                                                                
topic = 'visualization_marker_array'
publisher = rospy.Publisher(topic, MarkerArray, queue_size=1)

old_ID = ''

# Initialize system to stand-by
state = State.STAND_BY

# Initialize ROS node
rospy.init_node('gearbox_handler')
rate = rospy.Rate(10)  # 10hz

#Create broacaster and listener
br = tf.TransformBroadcaster()
listener = tf.TransformListener()

# Set if the software must evaluate the cad stl models ( false if already present)
output_stl = rospy.get_param('~output_stl', False)

#DATABASE AND REAL MODEL INITIALIZATION (data file path from launch files)
#
#
#
#
#
#
#
# Create gearbox object: it represents the actual configuration of the system. (It has all system transforms)

files_path = rospy.get_param('~files_path', "")
cad_name = rospy.get_param('~cad_name', "")
#robot_servoing =  rospy.get_param('~robot_servoing',"")
robot_servoing = '/tool_camera'
gearbox = Gearbox(cad_name, files_path)

# Create dictionary with all known components

components_file_name = rospy.get_param('~component_file', "")
gearbox_file_name = cad_name + "_ids.wri"
wiring_file_name = rospy.get_param('~wiring_file', "")
terminal_file_name = rospy.get_param('~terminal_file',"")
terminal_sheet_name = rospy.get_param('~terminal_sheet',"")
gearbox_display = rospy.get_param('~gearbox_display',"")
dict_gearbox = GearboxPartsData(files_path, gearbox_file_name, components_file_name, wiring_file_name,terminal_file_name,terminal_sheet_name)

# Control for activation of services according to available files
full_database = True
connection_database = True
terminal_database = True
if not components_file_name:
    full_database = False
if not wiring_file_name:
    full_database = False
    connection_database = False
if not terminal_file_name or not terminal_sheet_name:
    full_database = False
    terminal_database = False

# Create components file
#
#
#
#
#
#
# Output STL files
if output_stl:
    gearbox.scene.exportSTLData(files_path + 'out', base_transform=False)
    gearbox.scene.exportSTLData(files_path + 'out', False,
                                prefix='corrected_', correction=True)
    gearbox.scene.exportSTLData(files_path + 'out', False,
                                prefix='boxed_', correction=True, box_rf=True)

# Define transform broadcaster
#
#
#
#
#
#


def broadcastTransform(br, frame, frame_id, parent_frame, time=rospy.get_rostime()):
    br.sendTransform((frame.p.x(), frame.p.y(), frame.p.z()),
                     frame.M.GetQuaternion(),
                     time,
                     frame_id,
                     parent_frame)

#Create kdl interface functions - geometry_msgs pose for external services (for c++ interface)
#
#
#
#
#
#
#


def fromKdlToPose(kdl_frame):
    pose = Pose()
    pose.position.x = kdl_frame.p[0]
    pose.position.y = kdl_frame.p[1]
    pose.position.z = kdl_frame.p[2]
    ang = kdl_frame.M.GetQuaternion()
    pose.orientation.x = ang[0]
    pose.orientation.y = ang[1]
    pose.orientation.z = ang[2]
    pose.orientation.w = ang[3]
    return pose

def fromKdlAutoToPose(kdl_frame):
    pose = Pose()
    pose.position.x = kdl_frame.p.x()
    pose.position.y = kdl_frame.p.y()
    pose.position.z = kdl_frame.p.z()
    ang = kdl_frame.M.GetQuaternion()
    pose.orientation.x = ang[0]
    pose.orientation.y = ang[1]
    pose.orientation.z = ang[2]
    pose.orientation.w = ang[3]
    return pose

def fromPoseToKdl(pose):
    kdl_frame = PyKDL.Frame()
    kdl_frame.p = PyKDL.Vector(pose.position.x,pose.position.y,pose.position.z)
    kdl_frame.M = PyKDL.Rotation.Quaternion(pose.orientation.x,pose.orientation.y,pose.orientation.z,pose.orientation.w)
    return kdl_frame

# Create the marker array for visualization from transform list
#
#
#
#
#
#
#


transforms = gearbox.useful_transforms  # transforms are read by the Gearbox constructor
markerArray = MarkerArray()
package_path = 'file://' + files_path + 'out/'

def selection_sort(arr):        
    for i in range(len(arr)):
        minimum = i
        
        for j in range(i + 1, len(arr)):
            # Select the smallest value
            if arr[j]['corner_pose'].p.x() < arr[minimum]['corner_pose'].p.x():
                minimum = j

        # Place it at the front of the 
        # sorted end of the array
        arr[minimum], arr[i] = arr[i], arr[minimum]     
    return arr

# Create complete library for all the component of the gearbox
#
#
#
#
#
#
#
#
def create_gearbox_struct():
    child = {}
    temp_dict = {}
    temp_dict['pin']={}
    terminal_list =[]
    terminal_dict ={}
    complete_dict = {}
    tf_gearbox = listener.lookupTransform('/base_link', '/gearbox_rf', rospy.Time(0))
    correction = gearbox.scene.getCorrection(gearbox.base, box_correction=False)
    for trans in transforms:
        temp_dict = {}
        temp_dict['pin']={}
        root = trans.getRoot()
        cad = trans.getID().getCadID()
        if cad=="ID000204":
            continue
        label = dict_gearbox.dict_board[cad]['label']
        commercial = dict_gearbox.dict_board[cad]['product']
        if label in terminal_dict:
            terminal_list = terminal_dict[label]
        else:
            terminal_list =[]
        #if label[0]!="F" and label[0]!="Q":
        #    continue
        if commercial not in dict_gearbox.component_dict.keys():
            continue
        if dict_gearbox.component_dict[commercial]['parent']=='child':
            child[label] = commercial
            continue
        else:
            original_pose = transformations.tfToKDL(tf_gearbox) *trans.getKDLFrameFromBase()

            check_trans = gearbox.scene.retrieveTransform(trans, False, True, False)
                #broadcastTransform(br, back_trans, 'back_trans', root.getName(), time=current_time)

            corrected_check = correction.Inverse() * check_trans  # gearbox.scene.retrieveTransform(back_trans)#,True,True,False)# * back_trans.Inverse()
                #broadcastTransform(br, corrected_transform, 'back_trans_gearbox', 'gearbox', time=current_time)

            corner_trans_check = trans.getKDLFrameFromBoundingBox(corrected_check.Inverse())
                #broadcastTransform(br, corner_trans_gearbox, 'corner_trans_gearbox', 'gearbox', time=current_time)

            corner_check = corrected_check.Inverse() * corner_trans_check
            corner_pose  = original_pose * corner_check

            tf = PyKDL.Frame()
            dim = trans.getWholeShape().transformedShape(corrected_check.Inverse()).getXYZSize()
            #[x_dim, y_dim, z_dim] = dim
            tf.p = PyKDL.Vector(dim[0] / 2, dim[1] / 2, dim[2] + 0.15)
            tf.M = root.M
            tf.M.DoRotX(math.pi)
            target_pose = corner_pose * tf
            temp_dict['corner_pose'] = corner_pose
            temp_dict['target_pose'] = target_pose
            if dict_gearbox.component_dict[commercial]['parent']=='terminal':
                dim[0] = float(dict_gearbox.component_dict[commercial]["xdim"])
                dim[1] = float(dict_gearbox.component_dict[commercial]["ydim"])
                dim[2] = float(dict_gearbox.component_dict[commercial]["zdim"])
                temp_dict['dim'] = dim
                temp_dict['commercial'] = commercial
                terminal_list.append(temp_dict)
                terminal_dict[label] = terminal_list
                continue
            #print(label)
            #print(commercial)
            #print(dict_gearbox.component_dict[commercial]['screw'].keys())
            for item in dict_gearbox.component_dict[commercial]['screw'].keys():
                temp_dict['pin'][item]={}
                temp_dict['pin'][item]['screw'] = corner_pose*dict_gearbox.component_dict[commercial]['screw'][item]
                temp_dict['pin'][item]['hole'] = corner_pose*dict_gearbox.component_dict[commercial]['hole'][item]
            complete_dict[label] = temp_dict
    #look for screws for child components
    for label in child.keys():
        for item in dict_gearbox.component_dict[child[label]]['screw'].keys():
            complete_dict[label]['pin'][item]={}
            complete_dict[label]['pin'][item]['screw'] = complete_dict[label]['corner_pose']*dict_gearbox.component_dict[child[label]]['child']*dict_gearbox.component_dict[child[label]]['screw'][item]
            complete_dict[label]['pin'][item]['hole'] = complete_dict[label]['corner_pose']*dict_gearbox.component_dict[child[label]]['child']*dict_gearbox.component_dict[child[label]]['hole'][item]
    #terminal block definition
    for label in terminal_dict:
        #print(label)
        terminal_dict[label] = selection_sort(terminal_dict[label])
        #print(terminal_dict[label])
        if label not in complete_dict.keys():
            complete_dict[label]={}
        iterator = 0
        complete_dict[label]['terminal_length']=0
        for item in dict_gearbox.terminal_dict[label]:
            #print(item)
            if item == 'start':
                #print(terminal_dict[label][0]['dim'][0])
                complete_dict[label]['corner_pose'] = terminal_dict[label][0]['corner_pose']
                complete_dict[label]['terminal_height'] =terminal_dict[label][0]['dim'][2] + terminal_dict[label][0]['corner_pose'].p.z()
                complete_dict[label]['terminal_length'] = complete_dict[label]['terminal_length'] + terminal_dict[label][0]['dim'][0]
                separator = terminal_dict[label][0]['dim'][0]
                iterator = iterator + 1
                continue
            elif item =='division':
                complete_dict[label]['terminal_length'] = complete_dict[label]['terminal_length'] + 0.0036
                iterator = iterator + 1
                continue
            elif item =='end':
                complete_dict[label]['terminal_length'] = complete_dict[label]['terminal_length'] + separator
                iterator = iterator + 1
                continue
            else:
                complete_dict[label]['terminal_length'] = complete_dict[label]['terminal_length'] + terminal_dict[label][iterator]['dim'][0]
                #print(terminal_dict[label][iterator]['dim'][0])
                for pins in dict_gearbox.component_dict[terminal_dict[label][iterator]['commercial']]['screw'].keys():
                    pin_tag = item +':'+ pins
                    pose_offset = PyKDL.Frame()
                    pose_offset.M = PyKDL.Rotation.Identity()
                    pose_offset_x = terminal_dict[label][iterator]['corner_pose'].p.x()-complete_dict[label]['corner_pose'].p.x()
                    pose_offset_y = terminal_dict[label][iterator]['corner_pose'].p.y()-complete_dict[label]['corner_pose'].p.y()
                    pose_offset_z = terminal_dict[label][iterator]['corner_pose'].p.z()-complete_dict[label]['corner_pose'].p.z()
                    pose_offset.p = PyKDL.Vector(pose_offset_x,pose_offset_y,pose_offset_z)
                    if 'pin' not in complete_dict[label].keys():
                        complete_dict[label]['pin']={}
                    complete_dict[label]['pin'][pin_tag]={}
                    complete_dict[label]['pin'][pin_tag]['screw'] = complete_dict[label]['corner_pose']*pose_offset*dict_gearbox.component_dict[terminal_dict[label][iterator]['commercial']]['screw'][pins]
                    complete_dict[label]['pin'][pin_tag]['hole'] = complete_dict[label]['corner_pose']*pose_offset*dict_gearbox.component_dict[terminal_dict[label][iterator]['commercial']]['hole'][pins]
                iterator = iterator +1
                continue
        complete_dict[label]['target_pose'] = terminal_dict[label][0]['target_pose']
        complete_dict[label]['target_pose'].p[0] = complete_dict[label]['target_pose'].p[0] + complete_dict[label]['terminal_length']/2.0
    

    #print(complete_dict)
    return complete_dict



# Create array for visualization of the gearbox
#
#
#
#
#
#
#


for trans in transforms:

    parent = trans.parent
    path = package_path + trans.getName() + ".stl"
    color = visualization.Color(0.5, 0.5, 0.5, 1)
    # visualize red components
    # if trans.getName() == 'ID000074' or trans.getName() == 'ID000039':
    #    color = visualization.Color(1, 0, 0, 1)

    # check for the component list in wri file if 3 columns are present
    if len(trans.item_id.id_list) == 3:
        marker = visualization.createMesh(parent.getName(), mesh_path=path,
                                          transform=trans, color=color)
        marker.id = len(markerArray.markers)
        marker.text = trans.item_id.id_list[0]
        markerArray.markers.append(marker)

        #part = gearbox.parts.getPartById(trans.item_id.id_list[2])  # select the component commercial ID
        part=None
        if part != None:
            # print rospy.get_caller_id() + " Part " + trans.item_id.id_list[2] + ' with cad ID ' + trans.item_id.id_list[0] + ' and name ' + trans.item_id.id_list[1] + ' found'
            for terminal in part.terminals:
                # print 'get terminal for node ' + trans.item_id.id_list[2]
                arrow = visualization.createArrow(
                    trans.getName() + "_final", terminal, 0.02)
                arrow.id = len(markerArray.markers)
                markerArray.markers.append(arrow)

# Define the routines for services
#
#
#
#
#

part_trans = None
part_index = 0
source = 'orig'
connection_index = 0
connection_iterator = 0
connection = {}
connection_source = ''
original_pose = PyKDL.Frame()
corner_pose = PyKDL.Frame()
target_pose = PyKDL.Frame()
pin_targets = []
connection_service = 'gearbox_handler/connection_tf_service'
connection_list_service = 'gearbox_handler/connection_provider_service'
cad_reference = 'gearbox_handler/cad_reference'
cad_service = 'gearbox_handler/cad_service'
full_cad_service = 'gearbox_handler/full_cad_service'
target_screw_service = 'gearbox_handler/set_screw_target'
corrected_screw_service = 'gearbox_handler/set_corrected_screw_targets'
get_item_screw_service = 'gearbox_handler/get_item_screw_targets'
target_service = 'set_cad_target'
gearbox_service = 'gearbox_handler/gearbox_display'
# Define callback functions for services
#
#
#
#
#
#
#
#

# Activate/deactivate the visualization of all the tfs of the gearbox
#
#
#
def enable_gearbox_display(msg):
    global gearbox_display 
    resp = TriggerResponse()
    resp.success = True
    if gearbox_display == False:
        gearbox_display = True
    else:
       gearbox_display = False
    return resp



# Return pose info on requested component label
#
#
#
#
#
def get_cad_reference(req):
    global state
    global original_pose
    global target_pose
    global corner_pose
    resp = cad_poseResponse()
    resp.success = False
    if complete_dict[req.label]:
        resp.success = True
        target_pose = copy.deepcopy(complete_dict[req.label]['target_pose'])
        resp.target_pose=fromKdlToPose(target_pose)
        resp.corner_pose=fromKdlToPose(complete_dict[req.label]['corner_pose'])
        state = State.TARGET
    return resp



# Return all info on requested component label
#
#
#
#
#
def get_cad_service(req):
    global state
    global target_pose
    global pin_targets
    pin_targets = []
    temp_pins = cad_pins()
    resp = cad_dataResponse()
    resp.success = False
    if complete_dict[req.label]:
        resp.success = True
        target_pose = copy.deepcopy(complete_dict[req.label]['target_pose'])
        resp.target_pose=fromKdlToPose(target_pose)
        resp.corner_pose=fromKdlToPose(complete_dict[req.label]['corner_pose'])
        state = State.FULL_TARGET
        for pin in complete_dict[req.label]['pin']:
            temp_pins.pin = pin
            temp_pins.screw_pose = fromKdlToPose(complete_dict[req.label]['pin'][pin]['screw'])
            temp_pins.hole_pose = fromKdlToPose(complete_dict[req.label]['pin'][pin]['hole'])
            resp.pins.append(copy.deepcopy(temp_pins))
            pin_targets.append(copy.deepcopy(temp_pins))
    return resp



# Return cad and connection list info on requested cad item
#
#
#
#
#
def get_cad_data(req):
    global state
    global original_pose
    global corner_pose
    global target_pose
    resp = cad_infoResponse()
    resp.success = False
    for check in gearbox.useful_transforms:

        if check.getID().getCadID() == req.cad_item:
            resp.success = True
            original_pose = transformations.tfToKDL(tf_gearbox) *check.getKDLFrameFromBase()

            resp.original_pose=fromKdlToPose(original_pose)
            check_trans = gearbox.scene.retrieveTransform(check, False, True, False)
                #broadcastTransform(br, back_trans, 'back_trans', root.getName(), time=current_time)

            corrected_check = correction.Inverse() * check_trans  # gearbox.scene.retrieveTransform(back_trans)#,True,True,False)# * back_trans.Inverse()
                #broadcastTransform(br, corrected_transform, 'back_trans_gearbox', 'gearbox', time=current_time)

            corner_trans_check = check.getKDLFrameFromBoundingBox(corrected_check.Inverse())
                #broadcastTransform(br, corner_trans_gearbox, 'corner_trans_gearbox', 'gearbox', time=current_time)

            corner_check = corrected_check.Inverse() * corner_trans_check
            corner_pose  = original_pose * corner_check
            resp.corner_pose=fromKdlAutoToPose(corner_pose )

            tf = PyKDL.Frame()
            dim = check.getWholeShape().transformedShape(corrected_check.Inverse()).getXYZSize()
            #[x_dim, y_dim, z_dim] = dim

            tf.p = PyKDL.Vector(dim[0] / 2, dim[1] / 2, dim[2] + 0.15)
            tf.M = root.M
            tf.M.DoRotX(math.pi)
            resp.target_pose=fromKdlToPose(corner_pose * tf)
            break
    
    for connection in dict_gearbox.connection_list:
        new_connection = cad_connection()
        if connection['orig']['part_cadID'] == req.cad_item:
            new_connection.source = "orig"
            new_connection.pin = connection['orig']['pin']
            new_connection.target_item = connection['dest']['part_cadID']
            new_connection.target_pin = connection['dest']['pin']
            new_connection.screw_pose=fromKdlToPose(corner_pose * connection['orig']['screw'])
            new_connection.hole_pose=fromKdlToPose(corner_pose * connection['orig']['hole']) 
            resp.connection.append(new_connection)
        if connection['dest']['part_cadID'] == req.cad_item:
            new_connection.source = "dest"
            new_connection.pin = connection['dest']['pin']
            new_connection.target_item = connection['orig']['part_cadID']
            new_connection.target_pin = connection['orig']['pin']
            new_connection.screw_pose=fromKdlToPose(corner_pose * connection['dest']['screw']) 
            new_connection.hole_pose=fromKdlToPose(corner_pose * connection['dest']['hole']) 
            resp.connection.append(new_connection)
    state = State.TARGET
    target_pose = fromPoseToKdl(resp.target_pose)
    return resp



# Return screws and hole position corrected through the provided pose
#
#
#
#
#
def get_item_pin_callback(req):
    global pin_targets
    global state
    pin_targets =[]
    resp = pin_dataResponse()
    new_screw = Pose()
    temp_list = PyKDL.Frame()
    temp_pins = cad_pins()
    cad_detected_pose = PyKDL.Frame()
    cad_detected_pose=fromPoseToKdl(req.cad_pose)
    target_pose = complete_dict[req.label]['target_pose']
    pin_list = complete_dict[req.label]['pin']
    for item in pin_list.keys():
        temp_list.p[0] = pin_list[item]['screw'].p[0] - target_pose.p[0] + cad_detected_pose.p[0]
        temp_list.p[1] = pin_list[item]['screw'].p[1] - target_pose.p[1] + cad_detected_pose.p[1]
        temp_list.p[2] = pin_list[item]['screw'].p[2]
        temp_list.M = pin_list[item]['screw'].M
        new_screw=fromKdlToPose(temp_list)
        resp.screws.append(new_screw)
        temp_list.p[0] = pin_list[item]['hole'].p[0] - target_pose.p[0] + cad_detected_pose.p[0]
        temp_list.p[1] = pin_list[item]['hole'].p[1] - target_pose.p[1] + cad_detected_pose.p[1]
        temp_list.p[2] = pin_list[item]['hole'].p[2]
        temp_list.M = pin_list[item]['hole'].M
        new_hole=fromKdlToPose(temp_list)
        resp.holes.append(new_hole)
        resp.pins.append(item)
        temp_pins.pin = item
        temp_pins.screw_pose = new_screw
        temp_pins.hole_pose = new_hole
        pin_targets.append(copy.deepcopy(temp_pins))
    resp.success = True
    state = State.SCREW
    return resp



# Resets the connection list
#
#
#
#
#
def reset_provider_callback(msg):
    global connection_iterator
    global source
    resp = TriggerResponse()
    connection_iterator = 0
    source = "orig"
    return resp



# Reset the connection list to the chosen position
#
#
#
#
#
def reset_to_provider_callback(msg):
    global connection_iterator
    global source
    resp = DataSrvResponse()
    connection_iterator = int(msg.data)
    source = msg.source
    resp.output = "true"
    return resp



# Old service version used to skip selected components
#
#
#
#
#
def connection_provider_callback(req):

    global connection_iterator
    global state
    global source

    resp = connection_dataResponse()
    resp.end = False
    resp.missing_pin_data = False
    state = State.CONNECTION

    while True:
        try:
            connection = dict_gearbox.connection_list[connection_iterator]
            resp.label = connection[source]['part_label']
            resp.list_position = connection_iterator
        except IndexError:
            connection_iterator = 0
            print("ciao")
            resp.end = True
            return resp

        if resp.label[0] !='T' and resp.label!='X0030.1' and resp.label!='X0005.2' and resp.label[0]!='P':
            break
        
        if source == 'orig':
            source = 'dest'
        else:
            source = 'orig'
            connection_iterator = connection_iterator + 1


    if connection[source]['screw'] and connection[source]['hole']:
        resp.label = connection[source]['part_label']
        resp.pin = connection[source]['pin']
        resp.source = source
        resp.component_pose=fromKdlToPose(complete_dict[resp.label]['target_pose'])
    else:
        resp.missing_pin_data = True

    
    if source == 'orig':
        source = 'dest'
    else:
        source = 'orig'
        connection_iterator = connection_iterator + 1

    return resp




# At each call provides a new element in the connection list to be checked
#
#
#
#
#
def full_connection_provider_callback(req):

    global connection_iterator
    global state
    global source

    resp = connection_dataResponse()
    resp.end = False
    resp.missing_pin_data = False
    #state = State.CONNECTION


    try:
        connection = dict_gearbox.connection_list[connection_iterator]
        resp.label = connection[source]['part_label']
        resp.pin = connection[source]['pin']
        resp.source =source
        resp.list_position = connection_iterator
    except IndexError:
        connection_iterator = 0
        resp.end = True
        return resp

    if connection[source]['screw'] and connection[source]['hole']:       
        resp.component_pose=fromKdlToPose(complete_dict[resp.label]['target_pose'])
    else:
        resp.missing_pin_data = True
    
    if source == 'orig':
        source = 'dest'
    else:
        source = 'orig'
        connection_iterator = connection_iterator + 1

    return resp

def group_connection_provider_callback(req):

    global connection_iterator
    global state
    global source

    resp = connection_groupResponse()
    resp.end = False
    resp.missing_pin_data = False
    #state = State.CONNECTION


    try:
        connection = dict_gearbox.connection_list[connection_iterator]
        resp.origin.label = connection['orig']['part_label']
        resp.origin.pin = connection['orig']['pin']
        resp.destination.label = connection['dest']['part_label']
        resp.destination.pin = connection['dest']['pin']
        resp.list_position = connection_iterator
        resp.wire = connection['wire']
        resp.wire_path = connection['path']
    except IndexError:
        connection_iterator = 0
        resp.end = True
        return resp

    if connection['orig']['screw'] and connection['orig']['hole']:       
        resp.origin.component_pose=fromKdlToPose(complete_dict[resp.origin.label]['target_pose'])
        resp.origin.screw_pose=fromKdlToPose(complete_dict[resp.origin.label]['pin'][resp.origin.pin]['screw'])
        resp.origin.hole_pose=fromKdlToPose(complete_dict[resp.origin.label]['pin'][resp.origin.pin]['hole'])
    else:
        resp.missing_pin_data = True

    if connection['dest']['screw'] and connection['dest']['hole']:       
        resp.destination.component_pose=fromKdlToPose(complete_dict[resp.destination.label]['target_pose'])
        resp.destination.screw_pose=fromKdlToPose(complete_dict[resp.destination.label]['pin'][resp.destination.pin]['screw'])
        resp.destination.hole_pose=fromKdlToPose(complete_dict[resp.destination.label]['pin'][resp.destination.pin]['hole'])
    else:
        resp.missing_pin_data = True
    
    connection_iterator = connection_iterator + 1

    return resp




tf_service = 'gearbox_handler/part_tf_service'

def get_json_cad_data(req):
    dict_msg = StringSrvResponse()
    dict_msg.output = json.dumps(complete_dict)
    return dict_msg



# Return all info on requested terminal label
#
#
#
#
#
def get_terminal_data(req):
    resp = terminal_dataResponse()
    new_screw = Pose()
    temp_list = PyKDL.Frame()
    pin_list = complete_dict[req.label]['pin']
    for item in pin_list.keys():
        resp.pins.append(item)
        new_screw=fromKdlToPose(pin_list[item]['screw'])
        resp.screws.append(new_screw)
        new_hole=fromKdlToPose(pin_list[item]['hole'])
        resp.screws.append(new_hole)
    ########################################################################################################
    ########################################################################################################
    #MALISSIMOOOOOO#########################################################################################
    #######################################################################################################
    ########################################################################################################
    resp.terminal_start = fromKdlToPose(complete_dict[req.label]['corner_pose'])
    resp.length.data = complete_dict[req.label]['terminal_length']
    resp.height.data = complete_dict[req.label]['terminal_height']
    resp.success = True
    return resp




#set up services and full database according to available data
rospy.Service(gearbox_service,Trigger, enable_gearbox_display)

if full_database:
    complete_dict = create_gearbox_struct()
    rospy.Service(get_item_screw_service,pin_data,get_item_pin_callback)
    rospy.Service(cad_service,cad_data, get_cad_service)
    rospy.Service(cad_reference,cad_pose, get_cad_reference)
    rospy.Service("gearbox_handler/json_cad_data",StringSrv,get_json_cad_data)
if connection_database:
    rospy.Service("gearbox_handler/reset_connection_list",Trigger,reset_provider_callback)
    rospy.Service("gearbox_handler/reset_to_connection_list",DataSrv,reset_to_provider_callback)
    rospy.Service("gearbox_handler/full_connection_provider", connection_data, full_connection_provider_callback)
    rospy.Service("gearbox_handler/group_connection_provider",connection_group, group_connection_provider_callback)
    rospy.Service(connection_list_service, connection_data, connection_provider_callback)
    rospy.Service(full_cad_service,cad_info, get_cad_data)

if terminal_database:
    rospy.Service("gearbox_handler/get_terminal_data",terminal_data,get_terminal_data)

###################################################################################################################################
#SYSTEM INITIALIZATION COMPLETE 
#START LOOP CONTROL CYCLE AND TF PUBLISHERS
###################################################################################################################################
print("database_ready")




# Create whole component tf map

while not rospy.is_shutdown():

    # Move Gearbox on gearbox_rf if it is available
    try:
        tf_gearbox = listener.lookupTransform('/base_link', '/gearbox_rf', rospy.Time(0))

        correction = gearbox.scene.getCorrection(gearbox.base, box_correction=False)

        tf_gearbox_frame = transformations.tfToKDL(tf_gearbox)
        tf_gearbox_frame = tf_gearbox_frame * correction.Inverse()
        gearbox.base.p = tf_gearbox_frame.p
        gearbox.base.M = tf_gearbox_frame.M
    except AttributeError:
        if not tf_gearbox_frame:
            print "tf_gearbox_frame empty"

    current_time = rospy.get_rostime()

    for marker in markerArray.markers:
        marker.header.stamp = current_time
    publisher.publish(markerArray)

    # Broadcast Channel tF
    # for intersection in gearbox.channels.getIntersections():
    #    broadcastTransform(br, intersection.getFrame(), intersection.getName(),
    #                       "gearbox", time=current_time)

    root = trans.getRoot()

    broadcastTransform(br, root, root.getName(),
                       "gearbox_rf", time=current_time)


    correction = gearbox.scene.getCorrection(
        root, box_correction=False)
    broadcastTransform(br, correction, "gearbox",
                       root.getName(), time=current_time)

    cont = 0
    for item in gearbox.channels.channel_map.keys():
        broadcastTransform(br, gearbox.channels.channel_map[item].transform, "channel"+str(cont),
                        "gearbox", time=current_time)
        cont = cont +1
    intersections = gearbox.channels.getIntersections()
    cont = 0
    for item in intersections:
        broadcastTransform(br, item.getFrame(), item.getName(),
                        "gearbox", time=current_time)
        cont = cont+1

    #Broadcast everything of the system.
    #
    #
    #
    #
    #
    #
    if gearbox_display:
        for trans_tf in gearbox.useful_transforms:

            if trans_tf.getID().getCadID()!= "":

                if trans_tf.getID().getType() == ItemType.PART:

                    tf_name = trans_tf.getID().getCadID() 
                    tf_corner = trans_tf.getID().getCadID() + '_corner'
                    parent_tf = trans_tf.parent

                    broadcastTransform(br, trans_tf, tf_name, parent_tf.getName(), time=current_time)

                    back_trans = gearbox.scene.retrieveTransform(trans_tf, False, True, False)

                    corrected_transform = correction.Inverse() * back_trans  
                    corner_trans_gearbox = trans_tf.getKDLFrameFromBoundingBox(corrected_transform.Inverse())
                    corner_trans = corrected_transform.Inverse() * corner_trans_gearbox
                    broadcastTransform(br, corner_trans, tf_corner, tf_name, time=current_time)
                    while not parent_tf.isRoot():
                        broadcastTransform(br, parent_tf, parent_tf.getName(), parent_tf.parent.getName(), time=current_time)

                        parent_tf = parent_tf.parent
    
    #Additional tf broadcast according to some service calls
    if state == State.TARGET:
        broadcastTransform(br, target_pose, "target", 'base_link', time=current_time)

    if state == State.FULL_TARGET:
        broadcastTransform(br, target_pose, "target", 'base_link', time=current_time)
        for item in pin_targets:
            broadcastTransform(br, fromPoseToKdl(item.screw_pose), item.pin + '_' + 'screw', 'base_link', time=current_time)
            broadcastTransform(br, fromPoseToKdl(item.hole_pose), item.pin + '_' + 'hole', 'base_link', time=current_time)


    if state == State.SCREW:
        for item in pin_targets:
           broadcastTransform(br, fromPoseToKdl(item.screw_pose), item.pin + '_' + 'corrected_screw', 'base_link', time=current_time)
           broadcastTransform(br, fromPoseToKdl(item.hole_pose), item.pin + '_' + 'corrected_hole', 'base_link', time=current_time)



    rate.sleep()
