#!/usr/bin/env python
# coding=utf-8
"""
This class provides several ros routines and services to extract informations (and Tfs) from the cad files
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
from visualization_msgs.msg import Marker
from visualization_msgs.msg import MarkerArray
from geometry_msgs.msg import Point
from std_msgs.msg import String
from std_srvs.srv import Trigger, TriggerResponse
import random
import math
import tf
import time

from enum import Enum

# Definisco stato sistema
class State(Enum):
    STAND_BY = 1            #sistema idle
    VISUAL = 2              #visualizzo componente in lista
    CONNECTION = 3          #visualizzo connessione in lista
    TARGET = 4              #visualizzo target cad specifico
    SCREW = 5               #visualizzo viti rispetto target
    SCREW_CORRECTED = 6     #visualizzo viti adattate rispetto servoing del robot


class GearboxReferenceHandler(object):
    def __init__(self, dict_source, ros_topic ):
        self.gearbox = Gearbox(dict_source['cad_name'],dict_source['files_path'])
        self.dict_gearbox = GearboxPartsData(dict_source['files_path'], dict_source['gearbox_file_name'], dict_source['components_file_name'], dict_source['wiring_file_name'])
        self.package_path = 'file://' + dict_source['files_path'] + 'out/'
        self.transforms = self.gearbox.useful_transforms
        self.service = {}
        self.connection = {}
        self.connection_source = ''
        self.connection_index = 0 
        self.visualization_topic = ros_topic['visualization_topic']
        self.robot_servoing_topic = ros_topic['robot_pose']
        self.original_pose = PyKDL.Frame()
        self.corner_pose = PyKDL.Frame()
        self.target_pose = PyKDL.Frame()
        self.screw_targets = []
        self.part_trans = None
        self.part_index = 0
        self.gearbox_display = False
        self.tf_gearbox = None
        self.markerArray = MarkerArray()
        self.create_marker_array()
        self.initRos()
        self.initServiceList()

    def initRos(self):
        rospy.init_node('gearbox_handler')
        self.rate = rospy.Rate(10)
        self.br = tf.TransformBroadcaster()
        self.listener = tf.TransformListener()   
    
    def initServiceList(self):
        self.service['connection_service'] = "connection_tf_service"                        #it displays the next connection on the connection list
        self.service['component_pose'] = "cad_service"                                      #returns the position info about the requested cad item
        self.service['full_component_data'] = 'full_cad_service'                            #return all the data available about the requested cad item (pose + connections)
        self.service['display_target_component'] =  'set_cad_target'                        #displays the tf of the last cad item requested
        self.service['display_target_screws'] = 'set_target_screw'                          #displays the tf of the screws of the last cad item requested
        self.service['display_corrected_screws'] = 'set_screw_corrected'                    #displays the tf of the screws of the last cad item requested corrected wrt the vision estimation
        self.service['display_full_gearbox'] = 'display_full_gearbox'                       #enables the display of all the tfs of the components of the gearbox
        self.service['part_tf_service'] = 'part_tf_service'                                 #it displays the next cad item on the tf list

    def 
    
    def broadcastTransform(self, frame, frame_id, parent_frame, time=rospy.get_rostime()):
        self.br.sendTransform((frame.p.x(), frame.p.y(), frame.p.z()),
                     frame.M.GetQuaternion(),
                     time,
                     frame_id,
                     parent_frame)
                    
    def create_marker_array(self):
        for trans in self.transforms:
            parent = trans.parent
            path = self.package_path + trans.getName() + ".stl"
            color = visualization.Color(0.5, 0.5, 0.5, 1)
            if len(trans.item_id.id_list) == 3:
                marker = visualization.createMesh(parent.getName(), mesh_path=path,
                                                transform=trans, color=color)
                marker.id = len(self.markerArray.markers)
                marker.text = trans.item_id.id_list[0]
                self.markerArray.markers.append(marker)
                part = self.gearbox.parts.getPartById(trans.item_id.id_list[2])  # select the component commercial ID
                if part != None:
                    for terminal in part.terminals:
                        arrow = visualization.createArrow(
                            trans.getName() + "_final", terminal, 0.02)
                        arrow.id = len(self.markerArray.markers)
                        self.markerArray.markers.append(arrow)

    def get_requested_cad(self,cad_reference):
            self.original_pose = transformations.tfToKDL(self.tf_gearbox) *cad_reference.getKDLFrameFromBase()

            check_trans = gearbox.scene.retrieveTransform(cad_reference, False, True, False)
                #broadcastTransform(br, back_trans, 'back_trans', root.getName(), time=current_time)

            corrected_check = correction.Inverse() * check_trans  # gearbox.scene.retrieveTransform(back_trans)#,True,True,False)# * back_trans.Inverse()
                #broadcastTransform(br, corrected_transform, 'back_trans_gearbox', 'gearbox', time=current_time)

            corner_trans_check = check.getKDLFrameFromBoundingBox(corrected_check.Inverse())
                #broadcastTransform(br, corner_trans_gearbox, 'corner_trans_gearbox', 'gearbox', time=current_time)

            corner_check = corrected_check.Inverse() * corner_trans_check
            self.corner_pose  = original_pose * corner_check

            tf = PyKDL.Frame()
            dim = cad_reference.getWholeShape().transformedShape(corrected_check.Inverse()).getXYZSize()
            #[x_dim, y_dim, z_dim] = dim

            tf.p = PyKDL.Vector(dim[0] / 2, dim[1] / 2, dim[2] + 0.15)
            tf.M = root.M
            tf.M.DoRotX(math.pi)
            self.target_pose = corner_pose * tf

    def get_requested_screw_info(self, cad_reference):
        self.screw_targets = []
        for connection in self.dict_gearbox.connection_list:
            screw_temp = {}
        if connection['orig']['part_cadID'] == cad_reference:
            screw_temp['source'] = "orig"
            screw_temp['pin'] = connection['orig']['pin']
            screw_temp['target_item'] = connection['dest']['part_cadID']
            screw_temp['target_pin'] = connection['dest']['pin']
            screw_temp['pose'] = self.corner_pose * connection['orig']['screw']
            self.screw_targets.append(screw_temp)
        if connection['dest']['part_cadID'] == cad_reference:
            screw_temp['source'] = "dest"
            screw_temp['pin'] = connection['dest']['pin']
            screw_temp['target_item'] = connection['orig']['part_cadID']
            screw_temp['target_pin'] = connection['orig']['pin']
            screw_temp['pose'] = self.corner_pose * connection['dest']['screw']
            self.screw_targets.append(screw_temp)    

    def get_requested_screw_message(self,cad_reference,response)       
        for connection in self.dict_gearbox.connection_list:
        new_connection = cad_connection()
            if connection['orig']['part_cadID'] == req.cad_item:
                new_connection.source = "orig"
                new_connection.pin = connection['orig']['pin']
                new_connection.target_item = connection['dest']['part_cadID']
                new_connection.target_pin = connection['dest']['pin']
                fromKdlToPose(corner_pose * connection['orig']['screw'],new_connection.screw_pose) 
                response.connection.append(new_connection)
            if connection['dest']['part_cadID'] == req.cad_item:
                new_connection.source = "dest"
                new_connection.pin = connection['dest']['pin']
                new_connection.target_item = connection['orig']['part_cadID']
                new_connection.target_pin = connection['orig']['pin']
                fromKdlToPose(corner_pose * connection['dest']['screw'],new_connection.screw_pose) 
                response.connection.append(new_connection)

    #Service callbacks
    def enable_gearbox_display_callback(self,msg):
        resp = TriggerResponse()
        resp.success = True
        if self.gearbox_display == False:
            self.gearbox_display = True
        else:
            self.gearbox_display = False
        return resp
    
    def get_cad_reference_callback(self,req):
        resp = cad_poseResponse()
        resp.success = False
        for check in self.gearbox.useful_transforms:
            if check.getID().getCadID() == req.cad_item:
                resp.success = True
                self.get_requested_cad(check)
                resp.original_pose = transformations.KDLToPose(self.original_pose)
                resp.corner_pose = transformations.KDLToPose(self.corner_pose)
                resp.target_pose = transformations.KDLToPose(self.target_pose)
                break
        return resp

    def activate_target_reference_callback(self,req):
        resp = StringSrvResponse()
        self.state = State.TARGET
        for check in self.gearbox.useful_transforms:
            if check.getID().getCadID() == req.cad_item:
                self.get_requested_cad(check)
                break
        self.get_requested_screw_info(req.input)
        return resp

    def get_cad_data_callback(self,req):
        resp = cad_dataResponse()
        resp.success = False
        for check in self.gearbox.useful_transforms:
            if check.getID().getCadID() == req.cad_item:
                self.get_requested_cad(check)
                resp.original_pose = transformations.KDLToPose(self.original_pose)
                resp.corner_pose = transformations.KDLToPose(self.corner_pose)
                resp.target_pose = transformations.KDLToPose(self.target_pose)
                break
       self.get_requested_screw_message(req.cad_item,resp)
       return resp

    def screw_broadcaster_callback(self,msg):
        resp = TriggerResponse()
        resp.success = False
        if self.state == State.TARGET:
            self.state = State.SCREW
            resp.success = True
        return resp

    def screw_corrected_callback(self,msg):
        resp = TriggerResponse()
        resp.success = False
        if self.state == State.TARGET:
            resp.success = True
            self.state = State.SCREW
            cad_detected_pose = transformations.tfToKDL(listener.lookupTransform('/world', self.robot_servoing_topic, rospy.Time(0)))
            screw_temp = []
            for item in self.screw_targets:
                item['pose'].p = item['pose'].p - target_pose.p + cad_detected_pose.p
                screw_temp.append(item)
            self.screw_targets = screw_temp
        return resp