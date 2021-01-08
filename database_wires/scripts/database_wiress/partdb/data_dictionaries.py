#!/usr/bin/env python
# -*- encoding: utf-8 -*-

import os
import re
import csv
import sys
import xml.etree.ElementTree as ET
import PyKDL
import xmltodict
from pyexcel_ods import get_data

from database_wiress.param.global_parameters import Parameters

# "PROGETTO WIRES_244_IEMA_HILL_Lista_cablaggio.csv"
# "gearbox00001_ids.txt"
# "gearbox00001_cad.x3d"
# "Component_tf.xml"


class GearboxPartsData(object):
    def __init__(self, folder_path, board_file_name, component_file_name=None, connection_file_name=None ,terminal_file_name=None, terminal_sheet_name=None):
        self.folder_path = folder_path
        self.board_file_path = os.path.join(folder_path, board_file_name)
        self.dict_board = GearboxPartsData._createBoardDict(self.board_file_path)

        if component_file_name:
            self.component_file_name = component_file_name
            self.component_file_path = os.path.join(folder_path, component_file_name)
            self.component_dict = GearboxPartsData._generateComponentDict(self.component_file_path)
        else:
            self.component_file_path = None

        if connection_file_name:
            self.connection_file_name = connection_file_name
            self.connection_file_path = os.path.join(folder_path, connection_file_name)
            self.connection_dict = GearboxPartsData._createConnectionDict(self.connection_file_path)
            self.connection_list = GearboxPartsData._createConnectionList(self.connection_file_path)
        else:
            self.connection_file_path = None
            self.connection_dict = None
            self.connection_list = None

        #self.connection_orig = []
        #self.connection_dest = []
        if component_file_name and connection_file_name:
            for connection in self.connection_list:
                self.setConnectionDetails(connection, 'orig')
                self.setConnectionDetails(connection, 'dest')

        if terminal_file_name and terminal_sheet_name:
            self.terminal_file_name = terminal_file_name
            self.terminal_sheet_name = terminal_sheet_name
            self.terminal_file_path = os.path.join(folder_path,terminal_file_name)
            self.terminal_dict = GearboxPartsData._generateTerminalDict(self.terminal_file_path,self.terminal_sheet_name)
        else:
            self.terminal_file_path = None
            self.terminal_dict = None

    def setConnectionDetails(self, connection, direction):

        for cad_ID in self.dict_board:
            if self.dict_board[cad_ID]["label"] == connection[direction]['part_label']:

                commercial_ID = self.dict_board[cad_ID]["product"]

                try:
                    for item in self.component_dict[commercial_ID]:
                        # print item
                        if item == "screw" or item == "hole":
                            # print self.component_dict[commercial_ID][item]

                            try:
                                for pin in self.component_dict[commercial_ID][item]:

                                    # print self.component_dict[commercial_ID][item][pin]
                                    if pin == connection[direction]['pin']:
                                        connection[direction]['part_commercial'] = commercial_ID
                                        connection[direction]['part_cadID'] = cad_ID
                                        # print "connection found!"
                                        # print pin
                                        # print item
                                        # print self.component_dict[commercial_ID][item][pin]
                                        connection[direction][item] = self.component_dict[commercial_ID][item][pin]
                                        break
                                    tag_pin = connection[direction]['pin']
                                    if ":" in tag_pin:
                                        tag_pin,pin_name = tag_pin.split(":")
                                        if pin_name == pin:
                                            connection[direction]['part_commercial'] = commercial_ID
                                            connection[direction]['part_cadID'] = cad_ID
                                            # print "connection found!"
                                            # print pin
                                            # print item
                                            # print self.component_dict[commercial_ID][item][pin]
                                            connection[direction][item] = self.component_dict[commercial_ID][item][pin]
                                            break
                            except KeyError:
                                print "connection not found:"

                            except TypeError:
                                print "not a connection:"
                except KeyError:
                    print "commercial_ID not found in component databases"

        return connection

    @staticmethod
    def _createBoardDict(file_path):
        """ this function read the txt file for create a dictionary
        with the cad code like a key and the connection code and the commercial code inside"""
        dic = {}
        f = open(file_path, 'r')   # open the file
        row = len(f.readlines())  # count the numbers of row
        f.seek(0)  # reset the index to read the file
        for index in range(1, row + 1):  # do it for each lines of the file
            cad, label, product = map(
                str, f.readline().split('\t'))  # \t is a tab
            # don't add the items that aren't a component
            if label[2] != "U" and label[2] != "P":
                # remove the equal symbols to the label str
                label = re.sub('=', '', label)
                label = re.sub("-","", label)
                product = re.sub('\W+', "", product)
                #label = re.sub('\W+', "", label)
                dic[cad] = {"label": label, "product": product, "pose": None}  # TODO
        f.close()
        return dic

    def getGearboxDict(self, folder_path=None, file_name=None):
        ''' returns the entire dictionary '''
        if folder_path is None:
            folder_path = self.folder_path
        if file_name is not None:
            self.board_file_path = os.path.join(folder_path, file_name)
            self.dict_board = GearboxPartsData._createBoardDict(self.board_file_path)
        return self.dict_board

    def getComponentIDs(self, cad_id):
        ''' returns the component "lable" and "product" ids, given the CAD ID'''
        try:
            return self.dict_board[cad_id].pop("pose")
        except:
            return None

    def getComponentPose(self, cad_id):
        ''' returns the component theoretical pose (from the CAD data) on the switch board '''
        try:
            return self.dict_board[cad_id]["pose"]
        except:
            return None

    def getComponentsList(self, sort=None):
        ''' returns the list of all the component's CAD IDs(sorted by position if sorted="position", or unsorted otherwise) '''
        if sort == "position":
            pass  # TODO
        cad_id_list = self.dict_board.keys()
        return cad_id_list

    # ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
    # ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
    # ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇

    def getComponentTFs(self, product_id, component_file_name=None, folder_path=None):
        ''' returns the component significant frames (holes, skrews, marker, visual) '''
        if folder_path is None:
            folder_path = self.folder_path
        if component_file_name is not None:
            self.component_file_name = component_file_name
        self.component_file_path = os.path.join(folder_path, self.component_file_name)

        return GearboxPartsData._tfComponentDict(product_id, self.component_file_path)

    @staticmethod
    def _generateComponentDict(path):
        """ this function read the xml file for create a dictionary that contain all the tf for each part
        of the component(hole,screw,visual,marker,corner
        component type is a str"""
        with open(path) as fd:
            doc = xmltodict.parse(fd.read())


        maindict = {}

        tree = ET.parse(path)
        root = tree.getroot()

        for code in root:
            dic = {}
            holedic = {}
            screwdic = {}

            dic["name"] = code.get('name')
            for blocks in doc['data']['code']:
                tag_name = blocks['@name']
                tag_name = tag_name.encode('ascii')
                if tag_name == dic["name"]:
                    dic["xdim"] = blocks['@xdim'].encode('ascii')
                    dic["ydim"] = blocks['@ydim'].encode('ascii')
                    dic["zdim"] = blocks['@zdim'].encode('ascii')
            for item in code.findall('item'):
                for pos in item.findall('pos'):
                    if pos.get('name') == 'x':
                        x = float(pos.text)
                    if pos.get('name') == 'y':
                        y = float(pos.text)
                    if pos.get('name') == 'z':
                        z = float(pos.text)
                for rot in item.findall('rot'):
                    if rot.get('name') == 'R':
                        R = float(rot.text)
                    if rot.get('name') == 'P':
                        P = float(rot.text)
                    if rot.get('name') == 'Y':
                        Y = float(rot.text)
                tf = PyKDL.Frame()
                tf.p = PyKDL.Vector(x, y, z)
                tf.M.DoRotX(R)
                tf.M.DoRotY(P)
                tf.M.DoRotZ(Y)
                dic[item.get('name')] = tf
            for couple in code.findall('couple'):
                for subitem in couple.findall('subitem'):
                    for pos in subitem.findall('pos'):
                        if pos.get('name') == 'x':
                            x = float(pos.text)
                        if pos.get('name') == 'y':
                            y = float(pos.text)
                        if pos.get('name') == 'z':
                            z = float(pos.text)
                    for rot in subitem.findall('rot'):
                        if rot.get('name') == 'R':
                            R = float(rot.text)
                        if rot.get('name') == 'P':
                            P = float(rot.text)
                        if rot.get('name') == 'Y':
                            Y = float(rot.text)
                    tf = PyKDL.Frame()
                    tf.p = PyKDL.Vector(x, y, z)
                    tf.M.DoRotX(R)
                    tf.M.DoRotY(P)
                    tf.M.DoRotZ(Y)
                    if subitem.get('name') == 'hole':
                        holedic[couple.get('name')] = tf
                    else:
                        screwdic[couple.get('name')] = tf
            dic["parent"] = code.get('parent')
            for parent in code.findall('child'):
                for subitem in parent.findall('subitem'):
                    for pos in subitem.findall('pos'):
                        if pos.get('name') == 'x':
                            x = float(pos.text)
                        if pos.get('name') == 'y':
                            y = float(pos.text)
                        if pos.get('name') == 'z':
                            z = float(pos.text)
                    for rot in subitem.findall('rot'):
                        if rot.get('name') == 'R':
                            R = float(rot.text)
                        if rot.get('name') == 'P':
                            P = float(rot.text)
                        if rot.get('name') == 'Y':
                            Y = float(rot.text)
                    tf = PyKDL.Frame()
                    tf.p = PyKDL.Vector(x, y, z)
                    tf.M.DoRotX(R)
                    tf.M.DoRotY(P)
                    tf.M.DoRotZ(Y)
                    dic["child"] = tf                            
            dic["screw"] = screwdic
            dic["hole"] = holedic



            # TODO dic["correction"]=...
            maindict[code.get('commercial')] = dic

        return maindict

    @staticmethod
    def _tfComponentDict(component, path):
        """ this function read the xml file for create a dictionary that contain all the tf for each part
        of the component(hole,screw,visual,marker,corner
        component type is a str"""

        dic = {}
        holedic = {}
        screwdic = {}

        tree = ET.parse(path)
        root = tree.getroot()

        for code in root:
            if code.get('commercial') == component:
                for item in code.findall('item'):
                    for pos in item.findall('pos'):
                        if pos.get('name') == 'x':
                            x = float(pos.text)
                        if pos.get('name') == 'y':
                            y = float(pos.text)
                        if pos.get('name') == 'z':
                            z = float(pos.text)
                    for rot in item.findall('rot'):
                        if rot.get('name') == 'R':
                            R = float(rot.text)
                        if rot.get('name') == 'P':
                            P = float(rot.text)
                        if rot.get('name') == 'Y':
                            Y = float(rot.text)
                    tf = PyKDL.Frame()
                    tf.p = PyKDL.Vector(x, y, z)
                    tf.M.DoRotX(R)
                    tf.M.DoRotY(P)
                    tf.M.DoRotZ(Y)
                    dic[item.get('name')] = tf
                for couple in code.findall('couple'):
                    for subitem in couple.findall('subitem'):
                        for pos in subitem.findall('pos'):
                            if pos.get('name') == 'x':
                                x = float(pos.text)
                            if pos.get('name') == 'y':
                                y = float(pos.text)
                            if pos.get('name') == 'z':
                                z = float(pos.text)
                        for rot in subitem.findall('rot'):
                            if rot.get('name') == 'R':
                                R = float(rot.text)
                            if rot.get('name') == 'P':
                                P = float(rot.text)
                            if rot.get('name') == 'Y':
                                Y = float(rot.text)
                        tf = PyKDL.Frame()
                        tf.p = PyKDL.Vector(x, y, z)
                        tf.M.DoRotX(R)
                        tf.M.DoRotY(P)
                        tf.M.DoRotZ(Y)
                        if subitem.get('name') == 'hole':
                            holedic[couple.get('name')] = tf
                        else:
                            screwdic[couple.get('name')] = tf
                dic["screw"] = screwdic
                dic["hole"] = holedic
                # TODO dic["correction"]=...
        return dic

    @staticmethod
    def _generateTerminalDict(path,sheet):
        temp_dict = dict()
        temp_list = []
        data = get_data(path)
        dict_length = len(data[sheet])
        for item in range(1,dict_length):
            if data[sheet][item]:
                label = data[sheet][item][0]
                label = label.encode('ascii')
                label = re.sub("=","",label)
                label = re.sub("-","",label)
                place = data[sheet][item][1]
                place = place.encode('ascii')
                if len(data[sheet][item])>2:
                    pin = data[sheet][item][2]
                    pin = pin.encode('ascii')
                else:
                    pin = None
                if label in temp_dict:
                    temp_list = temp_dict[label]
                else:
                    temp_list = []
                if place =='Q':
                    if not pin:
                        temp_list.append("division")
                        temp_dict[label] = temp_list
                        continue
                    if pin == '0':
                        temp_list.append("start")
                        temp_dict[label] = temp_list
                        continue
                    if pin == '999999':
                        temp_list.append("end")
                        temp_dict[label] = temp_list
                        continue
                    temp_list.append(pin)
                    temp_dict[label] = temp_list
        #print(temp_dict)
        return temp_dict

####################################################################
####################################################################
####################################################################

    def getConnection(self):
        pass  # TODO

    @staticmethod
    def _createConnectionDict(file_path):
        """ this function read the csv file of the connection and return a dictionary"""

        dic = {}  # create a dictionary
        subdic = {}
        csv.register_dialect('connections', delimiter='\t')
        with open(file_path, 'rb') as f:
            old_id = ""
            reader = csv.reader(f, dialect='connections')  # read all the file
            test = 0
            for row in reader:
                subdic = {}

                if row[1] == "QE" or row[1] == "Q":
                    component_id_ori = row[2]
                    component_id_dest = row[5]
                    wire = row[6]
                    wire_path = row[12]
                    # remove the equal to the string
                    component_id_ori = re.sub('=', '', component_id_ori)
                    #if component_id_ori[1] == 'X':
                    #print("conto")
                    #print(component_id_ori)
                    #print(component_id_ori[1].count(':'))
                    if component_id_ori.count(':') == 2:
                        component_id_ori, Pin_ori, part2_ori = component_id_ori.split(
                            ":")  # split the connecting id to the pin id
                        Pin_ori = Pin_ori + ":" + part2_ori
                    else:
                        component_id_ori, Pin_ori = component_id_ori.split(
                            ":")  # split the connecting id to the pin id
                    # remove the equal to the string
                    component_id_dest = re.sub('=', '', component_id_dest)
                    #if component_id_dest[1] == 'X':
                    if component_id_dest.count(':') == 2:
                        component_id_dest, Pin_dest, part2_dest = component_id_dest.split(
                            ":")  # split the connecting id to the pin id
                        Pin_dest = Pin_dest + ":" + part2_dest
                    else:
                        component_id_dest, Pin_dest = component_id_dest.split(
                            ":")  # split the connecting id to the pin id

                    subdic[Pin_ori] = [component_id_dest, Pin_dest, wire]
                    try:
                        dic[component_id_ori].update(subdic)
                    except KeyError:
                        dic[component_id_ori] = subdic

        return dic

    @staticmethod
    def _createConnectionList(file_path):
        """ this function read the csv file of the connection and return a dictionary"""

        connection_list = []  # create a dictionary

        csv.register_dialect('connections', delimiter='\t')
        with open(file_path, 'rb') as f:
            reader = csv.reader(f, dialect='connections')  # read all the file
            for row in reader:
                orig = {}
                dest = {}
                connection = {}

                if row[1] == "QE" or row[1] == "Q":
                    component_id_ori = row[2]
                    component_id_dest = row[5]
                    wire = row[6]
                    wire_path = row[12]
                    # remove the equal to the string
                    component_id_ori = re.sub('=', '', component_id_ori)
                    #if component_id_ori[1] == 'X':
                    if component_id_ori.count(':') == 2:
                        component_id_ori, Pin_ori, part2_ori = component_id_ori.split(
                            ":")  # split the connecting id to the pin id
                        Pin_ori = Pin_ori + ":" + part2_ori
                    else:
                        component_id_ori, Pin_ori = component_id_ori.split(
                            ":")  # split the connecting id to the pin id
                    component_id_ori = re.sub("-","",component_id_ori)
                    orig["part_label"] = component_id_ori
                    orig["part_commercial"] = ''
                    orig["part_cadID"] = ''
                    orig["pin"] = Pin_ori
                    orig["screw"] = None
                    orig["hole"] = None

                    # remove the equal to the string
                    component_id_dest = re.sub('=', '', component_id_dest)
                    #if component_id_dest[1] == 'X':
                    if component_id_dest.count(':') == 2:
                        component_id_dest, Pin_dest, part2_dest = component_id_dest.split(
                            ":")  # split the connecting id to the pin id
                        Pin_dest = Pin_dest + ":" + part2_dest
                    else:
                        component_id_dest, Pin_dest = component_id_dest.split(
                            ":")  # split the connecting id to the pin id
                    component_id_dest = re.sub("-","",component_id_dest)
                    dest["part_label"] = component_id_dest
                    dest["part_commercial"] = ''
                    dest["part_cadID"] = ''
                    dest["pin"] = Pin_dest
                    dest["screw"] = None
                    dest["hole"] = None

                    connection["orig"] = orig
                    connection["dest"] = dest
                    connection["wire"] = wire
                    connection["path"] = wire_path

                    #print("pin orig")
                    #print(connection["orig"]["pin"])
                    #print("pin dest")
                    #print(connection["dest"]["pin"])
                    connection_list.append(connection)

        return connection_list
