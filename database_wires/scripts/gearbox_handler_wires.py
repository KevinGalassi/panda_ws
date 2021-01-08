#!/usr/bin/env python
# coding=utf-8
"""
This nodes is a testing node to show CAD Library tools to Load XML files and show result in RVIZ and as STL files
"""
from universal_wiress.partdb.cad import Scene, Shape, Face, STLUtils, Transform, Part, Terminal, PartCollection, Gearbox, Channel, ItemType
from universal_wires.srv import *
from universal_wires.msg import *
import universal_wiress.utils.transformations as transformations
import universal_wiress.utils.visualization as visualization
from universal_wiress.partdb.data_dictionaries import GearboxPartsData

import PyKDL
import numpy as np
import rospy
import json
import re
import pprint
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

# Definisco stato sistema
class State(Enum):
    STAND_BY = 1            #sistema idle
    VISUAL = 2              #visualizzo componente in lista
    CONNECTION = 3          #visualizzo connessione in lista
    TARGET = 4              #visualizzo target cad specifico
    SCREW = 5               #visualizzo viti rispetto target
    SCREW_CORRECTED = 6     #visualizzo viti adattate rispetto servoing del robot

#INIZIALIZZAZIONE COMPONENTI ROS
#
#
#
#
#
# MARKER ARRAY TOPIC                                                                
topic = 'visualization_marker_array'
publisher = rospy.Publisher(topic, MarkerArray, queue_size=1)

old_ID = ''

# Metto il sistema in stand by
state = State.STAND_BY

# Inizializzo il nodo ROS
rospy.init_node('gearbox_handler')
rate = rospy.Rate(10)  # 10hz

#Creo broadcaster e listener
br = tf.TransformBroadcaster()
listener = tf.TransformListener()

# Setto se voglio fare uscire i modelli cad stl  ( falso se sono già presenti)
output_stl = rospy.get_param('~output_stl', False)

#INIZIALIZZO MODELLO REALE E BANCA DATI (percosi file dati da launch file)
#
#
#
#
#
#
#
# Creo un oggetto gearbox: rappresenta la mia configurazione attuale del sistema. (Ha salvato tutte le transform del sistema)

files_path = rospy.get_param('~files_path', "")
cad_name = rospy.get_param('~cad_name', "")
#robot_servoing =  rospy.get_param('~robot_servoing',"")
robot_servoing = '/tool_camera'
gearbox = Gearbox(cad_name, files_path)

# Creo dizionario con tutti componenti noti

components_file_name = rospy.get_param('~component_file', "")
gearbox_file_name = cad_name + "_ids.wri"
wiring_file_name = rospy.get_param('~wiring_file', "")
terminal_file_name = rospy.get_param('~terminal_file',"")
terminal_sheet_name = rospy.get_param('~terminal_sheet',"")
dict_gearbox = GearboxPartsData(files_path, gearbox_file_name, components_file_name, wiring_file_name,terminal_file_name,terminal_sheet_name)


# Creo file dei componenti
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

# Definisco transform broadcaster
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

#Creo funzioni di interfaccia kdl - geometry_msgs pose per i servizi esterni (per interfaccia c++)
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

#Creo il marker array per la visualizzazione partendo dalla lista delle transform
#
#
#
#
#
#
#


transforms = gearbox.useful_transforms  # transforms are read by the Gearbox constructor
# gearbox.scene.deepSearch(transforms, only_useful=True) # read x3d files and generates switchgear tree structure
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
#Creo libraria definitiva per i componenti della gearbox attuale
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
        #print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
        #pprint.pprint(complete_dict)
        root = trans.getRoot()
        cad = trans.getID().getCadID()
        if cad=="ID000099":
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
            print(label)
            print(commercial)
            print(dict_gearbox.component_dict[commercial]['screw'].keys())
            for item in dict_gearbox.component_dict[commercial]['screw'].keys():
                temp_dict['pin'][item] = corner_pose*dict_gearbox.component_dict[commercial]['screw'][item]
            complete_dict[label] = temp_dict
    #carico viti dei componenti figli
    for label in child.keys():
        for item in dict_gearbox.component_dict[child[label]]['screw'].keys():
            complete_dict[label]['pin'][item] = complete_dict[label]['corner_pose']*dict_gearbox.component_dict[child[label]]['child']*dict_gearbox.component_dict[child[label]]['screw'][item]
    #carico/creo terminali
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
                complete_dict[label]['target_pose'] = terminal_dict[label][0]['target_pose']
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
                    complete_dict[label]['pin'][pin_tag] = complete_dict[label]['corner_pose']*pose_offset*dict_gearbox.component_dict[terminal_dict[label][iterator]['commercial']]['screw'][pins]
                iterator = iterator +1
                continue
    

    print(complete_dict)
    return complete_dict
        
complete_dict = create_gearbox_struct()

#Creo array per visualizzazione della gearbox
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

        part = gearbox.parts.getPartById(trans.item_id.id_list[2])  # select the component commercial ID
        if part != None:
            # print rospy.get_caller_id() + " Part " + trans.item_id.id_list[2] + ' with cad ID ' + trans.item_id.id_list[0] + ' and name ' + trans.item_id.id_list[1] + ' found'
            for terminal in part.terminals:
                # print 'get terminal for node ' + trans.item_id.id_list[2]
                arrow = visualization.createArrow(
                    trans.getName() + "_final", terminal, 0.02)
                arrow.id = len(markerArray.markers)
                markerArray.markers.append(arrow)

#Definisco le routine per i servizi
#
#
#
#
#

part_trans = None
part_index = 0

connection_service = 'gearbox_handler/connection_tf_service'
connection_list_service = 'gearbox_handler/connection_provider_service'
source = 'orig'
connection_index = 0
connection_iterator = 0
connection = {}
connection_source = ''
original_pose = PyKDL.Frame()
corner_pose = PyKDL.Frame()
target_pose = PyKDL.Frame()
screw_targets = []
cad_service = 'gearbox_handler/cad_service'
full_cad_service = 'gearbox_handler/full_cad_service'
target_screw_service = 'gearbox_handler/set_screw_target'
corrected_screw_service = 'gearbox_handler/set_corrected_screw_targets'
get_item_screw_service = 'gearbox_handler/get_item_screw_targets'
target_service = 'set_cad_target'
gearbox_service = 'gearbox_display'
gearbox_display = False
#Definisco funzioni callback per servizi
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

rospy.Service(gearbox_service,Trigger, enable_gearbox_display)

# Return info on requested cad item pose
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
        resp.target_pose=fromKdlToPose(complete_dict[req.label]['target_pose'])
        resp.corner_pose=fromKdlToPose(complete_dict[req.label]['corner_pose'])
    return resp

rospy.Service(cad_service,cad_pose, get_cad_reference)


def get_cad_data(req):
    global state
    global original_pose
    global corner_pose
    global target_pose
    resp = cad_dataResponse()
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
            resp.connection.append(new_connection)
        if connection['dest']['part_cadID'] == req.cad_item:
            new_connection.source = "dest"
            new_connection.pin = connection['dest']['pin']
            new_connection.target_item = connection['orig']['part_cadID']
            new_connection.target_pin = connection['orig']['pin']
            new_connection.screw_pose=fromKdlToPose(corner_pose * connection['dest']['screw']) 
            resp.connection.append(new_connection)

    return resp

rospy.Service(full_cad_service,cad_data, get_cad_data)


def get_item_screw_callback(req):
    resp = screw_dataResponse()
    new_screw = Pose()
    temp_list = PyKDL.Frame()
    cad_detected_pose = PyKDL.Frame()
    cad_detected_pose=fromPoseToKdl(req.cad_pose)
    print("cad_detected")
    print(cad_detected_pose)
    target_pose = complete_dict[req.label]['target_pose']
    print("cad_real")
    print(target_pose)
    pin_list = complete_dict[req.label]['pin']
    for item in pin_list.keys():
        print("prima x")
        print(pin_list[item].p[0])
        temp_list.p[0] = pin_list[item].p[0] - target_pose.p[0] + cad_detected_pose.p[0]
        temp_list.p[1] = pin_list[item].p[1] - target_pose.p[1] + cad_detected_pose.p[1]
        temp_list.p[2] = pin_list[item].p[2]
        temp_list.M = pin_list[item].M
        print("dopo x")
        print(temp_list.p[0])
        resp.pins.append(item)
        new_screw=fromKdlToPose(temp_list)
        print("new_screw")
        print(new_screw)
        resp.screws.append(new_screw)
    resp.success = True
    return resp

rospy.Service(get_item_screw_service,screw_data,get_item_screw_callback)

def reset_provider_callback(msg):
    global connection_iterator
    global source
    resp = TriggerResponse()
    connection_iterator = 0
    source = "orig"
    return resp

rospy.Service("gearbox_handler/reset_connection_list",Trigger,reset_provider_callback)


def connection_provider_callback(req):

    global connection_iterator
    global state
    global source

    resp = connection_dataResponse()
    resp.end = False
    resp.terminal = False
    #state = State.CONNECTION

    while True:
        try:
            connection = dict_gearbox.connection_list[connection_iterator]
            resp.label = connection[source]['part_label']

        except IndexError:
            connection_iterator = 0
            print("ciao")
            resp.end = True
            return resp

        if resp.label[0] =='F' or resp.label[0]=='Q':
            break
        
        if source == 'orig':
            source = 'dest'
        else:
            source = 'orig'
            connection_iterator = connection_iterator + 1


    if connection[source]['screw'] and connection[source]['hole']:
        resp.label = connection[source]['part_label']
        resp.pin = connection[source]['pin']
        resp.component_pose=fromKdlToPose(complete_dict[resp.label]['target_pose'])
    else:
        resp.terminal = True
    
    if source == 'orig':
        source = 'dest'
    else:
        source = 'orig'
        connection_iterator = connection_iterator + 1

    return resp


rospy.Service(connection_list_service, connection_data, connection_provider_callback)

def full_connection_provider_callback(req):

    global connection_iterator
    global state
    global source

    resp = connection_dataResponse()
    resp.end = False
    resp.terminal = False
    #state = State.CONNECTION


    try:
        connection = dict_gearbox.connection_list[connection_iterator]
        resp.label = connection[source]['part_label']

    except IndexError:
        connection_iterator = 0
        print("ciao")
        resp.end = True
        return resp

    if connection[source]['screw'] and connection[source]['hole']:
        resp.label = connection[source]['part_label']
        resp.pin = connection[source]['pin']
        print(connection[source]["pin"])
        resp.component_pose=fromKdlToPose(complete_dict[resp.label]['target_pose'])
    else:
        resp.terminal = True
    
    if source == 'orig':
        source = 'dest'
    else:
        source = 'orig'
        connection_iterator = connection_iterator + 1

    return resp


rospy.Service("gearbox_handler/full_connection_provider", connection_data, full_connection_provider_callback)

tf_service = 'gearbox_handler/part_tf_service'

def get_json_cad_data(req):
    dict_msg = StringSrvResponse()
    dict_msg.output = json.dumps(complete_dict)
    return dict_msg

rospy.Service("gearbox_handler/json_cad_data",StringSrv,get_json_cad_data)

def get_terminal_data(req):
    resp = terminal_dataResponse()
    new_screw = Pose()
    temp_list = PyKDL.Frame()
    pin_list = complete_dict[req.label]['pin']
    for item in pin_list.keys():
        resp.pins.append(item)
        new_screw=fromKdlToPose(pin_list[item])
        resp.screws.append(new_screw)
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

rospy.Service("gearbox_handler/get_terminal_data",terminal_data,get_terminal_data)



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
    # except (tf.ExtrapolationException, tf.LookupException) as e:
    #    print e
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
                    #broadcastTransform(br, back_trans, 'back_trans', root.getName(), time=current_time)

                    corrected_transform = correction.Inverse() * back_trans  # gearbox.scene.retrieveTransform(back_trans)#,True,True,False)# * back_trans.Inverse()
                    #broadcastTransform(br, corrected_transform, 'back_trans_gearbox', 'gearbox', time=current_time)

                    corner_trans_gearbox = trans_tf.getKDLFrameFromBoundingBox(corrected_transform.Inverse())
                    #broadcastTransform(br, corner_trans_gearbox, 'corner_trans_gearbox', 'gearbox', time=current_time)

                    corner_trans = corrected_transform.Inverse() * corner_trans_gearbox
                    broadcastTransform(br, corner_trans, tf_corner, tf_name, time=current_time)
                    while not parent_tf.isRoot():
                        broadcastTransform(br, parent_tf, parent_tf.getName(), parent_tf.parent.getName(), time=current_time)

                        parent_tf = parent_tf.parent

    if state == State.VISUAL:
        broadcastTransform(br, part_trans, 'part_origin', part_trans.parent.getName(), time=current_time)

        back_trans = gearbox.scene.retrieveTransform(part_trans, False, True, False)
        #broadcastTransform(br, back_trans, 'back_trans', root.getName(), time=current_time)

        corrected_transform = correction.Inverse() * back_trans  # gearbox.scene.retrieveTransform(back_trans)#,True,True,False)# * back_trans.Inverse()
        #broadcastTransform(br, corrected_transform, 'back_trans_gearbox', 'gearbox', time=current_time)

        corner_trans_gearbox = part_trans.getKDLFrameFromBoundingBox(corrected_transform.Inverse())
        #broadcastTransform(br, corner_trans_gearbox, 'corner_trans_gearbox', 'gearbox', time=current_time)

        corner_trans = corrected_transform.Inverse() * corner_trans_gearbox
        broadcastTransform(br, corner_trans, 'part_corner', 'part_origin', time=current_time)
        # try:

        #     commercial_id = dict_gearbox.dict_board[part_trans.getID().getCadID()]["product"]
        #     dic_tfs = dict_gearbox.component_dict[commercial_id]

        #     broadcastTransform(br, dic_tfs['visual'],
        #                             "target",
        #                             'part_corner',
        #                             time=current_time)
        # except KeyError:
        tf = PyKDL.Frame()
        dim = part_trans.getWholeShape().transformedShape(corrected_transform.Inverse()).getXYZSize()
        #[x_dim, y_dim, z_dim] = dim

        tf.p = PyKDL.Vector(dim[0] / 2, dim[1] / 2, dim[2] + 0.15)
        tf.M = root.M
        tf.M.DoRotX(math.pi)
        broadcastTransform(br, tf,
                            "target",
                            'part_corner',
                            time=current_time)

        parent = part_trans.parent

        while not parent.isRoot():
            broadcastTransform(br, parent, parent.getName(), parent.parent.getName(), time=current_time)

            #boxed_transform = gearbox.scene.getCorrection(parent)
            #broadcastTransform(br, boxed_transform, parent.getName() + "_final", parent.getName(), time=current_time)
            parent = parent.parent

    if state == State.CONNECTION:

        broadcastTransform(br, part_trans, 'part_origin', part_trans.parent.getName(), time=current_time)

        back_trans = gearbox.scene.retrieveTransform(part_trans, False, True, False)
        #broadcastTransform(br, back_trans, 'back_trans', root.getName(), time=current_time)

        corrected_transform = correction.Inverse() * back_trans  # gearbox.scene.retrieveTransform(back_trans)#,True,True,False)# * back_trans.Inverse()
        #broadcastTransform(br, corrected_transform, 'back_trans_gearbox', 'gearbox', time=current_time)

        corner_trans_gearbox = part_trans.getKDLFrameFromBoundingBox(corrected_transform.Inverse())
        #broadcastTransform(br, corner_trans_gearbox, 'corner_trans_gearbox', 'gearbox', time=current_time)

        corner_trans = corrected_transform.Inverse() * corner_trans_gearbox
        broadcastTransform(br, corner_trans, 'part_corner', 'part_origin', time=current_time)

        try:
            broadcastTransform(br, connection[connection_source]['hole'],
                                "target_h",
                                'part_corner',
                                time=current_time)
        except KeyError:
            print "hole data not available for part " + connection[connection_source]['part_label'] + " and terminal " + connection[connection_source]['pin']

        try:
            broadcastTransform(br, connection[connection_source]['screw'],
                                "target_s",
                                'part_corner',
                                time=current_time)
        except KeyError:
            print "screw data not available for part " + connection[connection_source]['part_label'] + " and terminal " + connection[connection_source]['pin']
        parent = part_trans.parent

        while not parent.isRoot():
            broadcastTransform(br, parent, parent.getName(), parent.parent.getName(), time=current_time)

            #boxed_transform = gearbox.scene.getCorrection(parent)
            #broadcastTransform(br, boxed_transform, parent.getName() + "_final", parent.getName(), time=current_time)
            parent = parent.parent

    if state == State.TARGET:
        broadcastTransform(br, target_pose, "target", 'world', time=current_time)

    if state == State.SCREW:
        for item in screw_targets:
           broadcastTransform(br, item['pose'], item['source'] + '_' +  item['pin'], 'world', time=current_time) 



    rate.sleep()