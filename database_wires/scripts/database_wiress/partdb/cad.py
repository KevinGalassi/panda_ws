""" Testing MOdule """
import struct
import sys
import xml.etree.ElementTree as ET
import numpy as np
import PyKDL
import math
import rospy
from PyKDL import Frame
import database_wiress.utils.transformations as transformations
import database_wiress.utils.gc as gc
import copy
import os

from database_wiress.param.global_parameters import Parameters
from database_wiress.partdb.data_dictionaries import GearboxPartsData  # to check

###############################################################
###############################################################
###############################################################


class ItemType:
    PART, PART_POI, CHANNEL, CHANNEL_POI, BRACKET, BRACKET_POI, BASE, GROUND, UNKNOWN = range(
        9)

    #PART_SUB_ID = ['-X', '-T', '-Q', '-F']
    PART_SUB_ID = ['-X', '-T', '-Q', '-F', '-0', '-1', '-4' ,'-5']
    #CHANNEL_SUB_ID = ['-U']
    CHANNEL_SUB_ID = ['-CN']
    #BRACKET_SUB_ID = ['-U']
    BRACKET_SUB_ID = ['-BD']
    BASE_SUB_ID = ['-PE']

    @staticmethod
    def getTypeFromItemID(item_id):
        if item_id == None:
            return ItemType.UNKNOWN

        if item_id.getSheetID() == None:
            return ItemType.UNKNOWN

        if any(ids in item_id.getSheetID() for ids in ItemType.CHANNEL_SUB_ID):
            if item_id.getCommercialID() == None:
                return ItemType.CHANNEL_POI
            if 'BOCCH' in item_id.getCommercialID():  # TODO: cambiare questo check con uno migliore
                return ItemType.CHANNEL
          
        if any(ids in item_id.getSheetID() for ids in ItemType.BRACKET_SUB_ID):
            if item_id.getCommercialID() == None:
                return ItemType.BRACKET_POI
            if 'WEI' in item_id.getCommercialID():
                return ItemType.BRACKET

        if any(ids in item_id.getSheetID() for ids in ItemType.PART_SUB_ID):
            if item_id.getCommercialID() != None:
                return ItemType.PART
            else:
                return ItemType.PART_POI

        if any(ids in item_id.getSheetID() for ids in ItemType.BASE_SUB_ID):
            if item_id.getCommercialID() != None:
                return ItemType.GROUND
            else:
                return ItemType.BASE


###############################################################
###############################################################
###############################################################


class ItemID(object):
    """ Transforms complex id """

    def __init__(self, cad_id, ref_id=''):
        self.cad_id = cad_id
        self.ref_id = ref_id
        self.id_list = [self.cad_id]
        chunks = self.ref_id.split()
        for pid in chunks:
            self.id_list.append(pid)

    def getCadID(self):
        if len(self.id_list) > 0:
            return self.id_list[0]
        return None

    def getSheetID(self):
        if len(self.id_list) > 1:
            return self.id_list[1]
        return None

    def getCommercialID(self):
        if len(self.id_list) > 2:
            return self.id_list[2]
        return None

    def getType(self):
        return ItemType.getTypeFromItemID(self)

    def __str__(self):
        s = "["
        for pid in self.id_list:
            s += pid + ","
        s += "]"
        return s

###############################################################
###############################################################
###############################################################


class STLUtils(object):

    """ Utils for STL files manipulation """
    @staticmethod
    def buildSTLHeader():
        """ Builds void header """
        header = bytearray()
        for i in range(0, 80):
            header.extend(struct.pack('B', 0))
        return header

    @staticmethod
    def buildSTLFileBytes(shape_list):
        stl = STLUtils.buildSTLHeader()
        data = bytearray()
        face_counter = 0
        for shape in shape_list:
            face_counter = face_counter + shape.getFacesCount()
            data.extend(STLUtils.toSTLBytes(shape))

        stl.extend(struct.pack('<I', face_counter))
        stl.extend(data)
        return stl

    @staticmethod
    def toSTLBytes(obj):
        """ Converts generic object to stl byte array """
        if isinstance(obj, Face):
            arr = bytearray()
            pt0 = obj.vertices[0]
            pt1 = obj.vertices[1]
            pt2 = obj.vertices[2]
            diff1 = pt1 - pt0
            diff2 = pt2 - pt1
            norm = np.cross(diff1, diff2)
            norm = norm / np.linalg.norm(norm)

            for i in range(0, 3):
                arr.extend(struct.pack("<f", norm[i]))
            for p in range(0, 3):
                for i in range(0, 3):
                    arr.extend(struct.pack("<f", obj.vertices[p][i]))
            arr.extend(struct.pack("!H", 0))
            return arr
        if isinstance(obj, Shape):
            data = bytearray()
            for fid in range(0, obj.getFacesCount()):
                data.extend(STLUtils.toSTLBytes(obj.getFace(fid)))
            return data
        else:
            print "STLUtils: conversion not implemented for " + obj

    @staticmethod
    def toSTLString(obj):
        """ Converts generic object to stl byte array """
        if isinstance(obj, Face):
            pt0 = obj.vertices[0]
            pt1 = obj.vertices[1]
            pt2 = obj.vertices[2]
            diff1 = pt1 - pt0
            diff2 = pt2 - pt1
            norm = np.cross(diff1, diff2)
            norm = norm / np.linalg.norm(norm)
            stl = ''
            stl += '\tfacet normal '
            stl += str(norm[0]) + " "
            stl += str(norm[1]) + " "
            stl += str(norm[2]) + "\n"
            stl += '\t\touter loop\n'
            stl += '\t\t\tvertex '
            stl += str(pt0[0]) + ' '
            stl += str(pt0[1]) + ' '
            stl += str(pt0[2]) + '\n'
            stl += '\t\t\tvertex '
            stl += str(pt1[0]) + ' '
            stl += str(pt1[1]) + ' '
            stl += str(pt1[2]) + '\n'
            stl += '\t\t\tvertex '
            stl += str(pt2[0]) + ' '
            stl += str(pt2[1]) + ' '
            stl += str(pt2[2]) + '\n'
            stl += '\t\tendloop\n'
            stl += '\tendfacet\n'
            return stl
        else:
            print "STLUtils: conversion not implemented for " + obj


###############################################################
###############################################################
###############################################################

class Face(object):
    """ Triangle face """

    def __init__(self, vertices):
        self.vertices = vertices

###############################################################
###############################################################
###############################################################


class Shape(object):
    """ Shape object with mesh information """

    def __init__(self, node=None, parent=None):
        if node != None:
            self.node = node
            self.color = np.array([0, 0, 0])
            self.coordinates = np.array([])
            self.faces_indices = np.array([])
            self.points = []
            self.faces = []
            self.parent = parent
            self.manageAppearance(self.node)
            self.manageGeometry(self.node)
        else:
            self.node = None
            self.color = np.array([0, 0, 0])
            self.points = []
            self.faces = []
            self.parent = None

    def scale(self, factor):
        """ Rescales points """
        new_points = copy.deepcopy(self.points)
        for i in range(0, len(new_points)):
            new_points[i] = new_points[i] * factor
        self.points = new_points

    def transformedShape(self, frame=PyKDL.Frame()):
        """ Transforms a Shape by a PyKDL frame """
        newshape = Shape(self.node, self.parent)
        newshape.faces = list(self.faces)
        new_points = []
        for point in self.points:
            v = PyKDL.Vector(
                point[0], point[1], point[2]
            )
            v = frame * v
            new_point = np.array([v.x(), v.y(), v.z()])
            new_points.append(new_point)
        newshape.points = new_points
        return newshape

    def getMinimumPoint(self):
        min_x = 1000000
        min_y = 1000000
        min_z = 1000000
        if self.points == 0:
            return np.array([0, 0, 0])
        for point in self.points:
            if point[0] < min_x:
                min_x = point[0]
            if point[1] < min_y:
                min_y = point[1]
            if point[2] < min_z:
                min_z = point[2]
        return np.array([min_x, min_y, min_z])

    def getXYZSize(self):
        min_x = 1000000
        max_x = -1000000   
        min_y = 1000000
        max_y = -1000000      
        min_z = 1000000
        max_z = -1000000 
        if self.points == 0:
            return 0
        for point in self.points:
            if point[0] < min_x:
                min_x = point[0]
            if point[0] > max_x:
                max_x = point[0]

            if point[1] < min_y:
                min_y = point[1]
            if point[1] > max_y:
                max_y = point[1]

            if point[2] < min_z:
                min_z = point[2]
            if point[2] > max_z:
                max_z = point[2]
            
        return [ abs(max_x - min_x), abs(max_y - min_y), abs(max_z - min_z)]
    
    def getMinimumPointTransform(self):
        point = self.getMinimumPoint()
        frame = PyKDL.Frame()
        frame.p = PyKDL.Vector(point[0], point[1], point[2])
        return frame

    def getFacesCount(self):
        return len(self.faces)

    def getFace(self, index):
        if index < len(self.faces):
            i0 = self.faces[index][0]
            i1 = self.faces[index][1]
            i2 = self.faces[index][2]
            return Face([
                self.points[i0],
                self.points[i1],
                self.points[i2]
            ])
        return None

    def manageAppearance(self, node):
        """ retrieve Appearance infos """
        if node.find("Appearance") != None:
            appearance_node = node.find("Appearance")
            if appearance_node.find("Material") != None:
                material_node = appearance_node.find("Material")
                if 'diffuseColor' in material_node.attrib:
                    self.color = np.array(
                        material_node.attrib['diffuseColor'].split(' '))

    def manageGeometry(self, node):
        """ retrieve Geometry infos """
        if node.find("IndexedFaceSet") != None:
            face_node = node.find("IndexedFaceSet")
            if 'coordIndex' in face_node.attrib:
                self.faces_indices = np.array(
                    [int(i)
                     for i in face_node.attrib['coordIndex'].split(' ')])
                for i in range(0, len(self.faces_indices), 4):
                    self.faces.append(np.array([
                        self.faces_indices[i],
                        self.faces_indices[i + 1],
                        self.faces_indices[i + 2]
                    ]))

            if face_node.find("Coordinate") != None:
                coordinate_node = face_node.find("Coordinate")
                if 'point' in coordinate_node.attrib:
                    self.coordinates = np.array(
                        [float(i)
                         for i in coordinate_node.attrib['point'].split(' ')])
                    for i in range(0, len(self.coordinates), 3):
                        self.points.append(np.array([
                            self.coordinates[i],
                            self.coordinates[i + 1],
                            self.coordinates[i + 2]
                        ]))

    def __add__(self, s):
        shape = Shape(None, None)
        shape.points = copy.deepcopy(self.points)
        shape.points.extend(copy.deepcopy(s.points))
        new_faces = copy.deepcopy(s.faces)
        start_index = len(self.points)
        shape.faces = copy.deepcopy(self.faces)
        for f in new_faces:
            f[0] += start_index
            f[1] += start_index
            f[2] += start_index
            shape.faces.append(f)
        return shape


###############################################################
###############################################################
###############################################################


class Transform(PyKDL.Frame):
    """ Transform object with RF, attributes and children RFs """

    def __init__(self, node, recursive, parent=None, build_whole_shape=True):
        super(PyKDL.Frame, self).__init__()
        self.node = node
        self.shapes = []
        self.shape = None
        self.children_transforms = []
        self.id_string = ''
        self.item_id = None
        self.parent = parent

        self.translation = np.array([0.0, 0.0, 0.0])
        # rotation = 'x y z angle'
        self.rotation = np.array([1.0, 0.0, 0.0, 0.0])
        self.frame = None
        self.parseAttributes(node)

        for element in self.node:
            if element.tag.lower() == 'shape':
                self.shapes.append(Shape(element, self))
            elif element.tag.lower() == 'transform':
                if recursive:
                    self.children_transforms.append(
                        Transform(element, True, self))
            else:
                print "@@@@ UNKNOWN TAG!!! " + element.tag
        if build_whole_shape:
            self.shape = self.buildWholeShape()

    def getID(self):
        return self.item_id

    def isUseful(self):
        return self.item_id != None

    def rebuild(self):
        self.buildKDLFrame()
        self.buildWholeShape()

    def scale(self, factor):
        """ Scales translational part of transform. Rebuilds KDL Frame. """
        self.translation = self.translation * factor
        for shape in self.shapes:
            if len(shape.points) > 0:
                shape.scale(factor)
        self.rebuild()

    def buildKDLFrame(self):
        """ Gets KDL Frame representation. """
        self.p = PyKDL.Vector(
            float(self.translation[0]),
            float(self.translation[1]),
            float(self.translation[2])
        )
        quat = transformations.quaternion_about_axis(
            self.rotation[3], self.rotation[0:3])
        self.M = PyKDL.Rotation.Quaternion(
            quat[0], quat[1], quat[2],
            quat[3]
        )

    def buildWholeShape(self):
        full_shape = None
        for s in self.shapes:
            if full_shape == None:
                full_shape = s
            else:
                full_shape = full_shape + s
        self.shape = full_shape

    def getWholeShape(self):
        if self.shape == None:
            self.buildWholeShape()
        return self.shape

    def getTransformationsChain(self, discard_root=True):
        """ Collects recursively all back transformation from base to current. """
        chain = [self]
        back = self.parent
        while back != None:
            if back.isUseful():
                chain.insert(0, back)
            else:
                break
            back = back.parent
        if discard_root:
            chain.pop(0)
        return chain

    def getKDLFrameFromBase(self, frame_correction=PyKDL.Frame(), discard_root=True):
        """ Gets KDL Frame from base """
        chain = self.getTransformationsChain(discard_root=discard_root)
        t = PyKDL.Frame()
        for transform in chain:
            t = t * transform  # .getKDLFrame()
        t = t * frame_correction
        return t

    def getKDLFrameFromBaseTalking(self, frame_correction=PyKDL.Frame(), discard_root=True):
        """ Gets KDL Frame from base """
        chain = self.getTransformationsChain(discard_root=discard_root)
        t = PyKDL.Frame()
        for transform in chain:
            #print transform.getName()
            t = t * transform  # .getKDLFrame()
            #print t
        t = t * frame_correction
        #print "frame_correction"
        #print t
        return t

    def getKDLFrameFromBoundingBox(self, correction_transform):
        """ Gets the bouding box trasform of Lowest Corner  """
        shape = self.getWholeShape().transformedShape(correction_transform.Inverse())
        return shape.getMinimumPointTransform()

    def parseAttributes(self, node):
        """ Parses attributes of xml node, T,R,ID.. """

        attrib = node.attrib
        if 'translation' in attrib:
            self.translation = np.array(
                map(float, attrib['translation'].split(' ')))
        if 'rotation' in attrib:
            self.rotation = np.array(map(float, attrib['rotation'].split(' ')))
        if 'DEF' in attrib:
            self.id_string = attrib['DEF']

    def deepItemIDMapping(self, id_map):
        """ Deep visit on transform tree to update ItemIDs """
        if self.id_string in id_map:
            self.item_id = id_map[self.id_string]
        for transform in self.children_transforms:
            transform.deepItemIDMapping(id_map)

    def deepSearch(self, transform_list, only_useful=False):
        """ Search for all Transform """
        if only_useful:
            if self.isUseful():
                transform_list.append(self)
        else:
            transform_list.append(self)
        for transform in self.children_transforms:
            transform.deepSearch(transform_list, only_useful)

    def findTransform(self, cad_id):
        """ Search for Tranfsorm with target ID. wrapper of Deep search"""
        transforms = []
        self.deepItemIdSearch(cad_id, transforms)
        if len(transforms) > 1:
            print "##### STRANGE!!!!"
        if len(transforms) > 0:
            return transforms[0]
        return None

    def deepItemIdSearch(self, cad_id, result):
        """ Iterative search for Transform with target ID """
        if self.item_id != None:
            if self.item_id.cad_id == cad_id:
                result.append(self)
        for transform in self.children_transforms:
            transform.deepItemIdSearch(cad_id, result)

    def getRoot(self):
        """ Iterative search for Root Transform """
        if self.parent != None:
            if self.parent.isUseful():
                return self.parent.getRoot()
            else:
                return self
        else:
            return self

    def isRoot(self):
        return self == self.getRoot()

    def getName(self):
        if self.isUseful():
            return self.item_id.getCadID()
        else:
            return "NONAME"

    #def __str__(self):
    #    return "Transform[" + str(self.item_id) + "]"

###############################################################
###############################################################
###############################################################


class SceneConfiguration(object):

    def __init__(self):
        demo_type = rospy.get_param('~demo_type', "")
        # Component correction, is a rotation about 90deg over x-axis
        corr = PyKDL.Frame()
        # .DoRotX(math.radians(90))
        #if demo_type == "wires":
        corr.M = PyKDL.Rotation.RotX(1.57)
        #if demo_type == "remodel":
            #corr.M = PyKDL.Rotation.RotZ(1.57)
        self.component_correction = corr
        # Scales
        self.scale = 0.001

###############################################################
###############################################################
###############################################################


class Scene(Transform):
    """ Extension of Transform representing a Scene """

    def __init__(self, node, id_map, scene_configuration=SceneConfiguration()):
        self.id_map = id_map
        self.scene_config = scene_configuration
        super(Scene, self).__init__(node, True, None)
        self.deepItemIDMapping(id_map)
        self.rescaleScene(self.scene_config.scale)

    def rescaleScene(self, factor):
        """ Rescale all transforms in Scene """
        transforms = []
        self.deepSearch(transforms)
        for ts in transforms:
            ts.scale(factor)

    def retrieveTransform(self, target_transform, apply_correction=True, from_base=False, box_rf=False):
        correction = PyKDL.Frame()
        if apply_correction:
            correction = self.scene_config.component_correction

        transform = target_transform
        if from_base:
            transform = target_transform.getKDLFrameFromBase(correction)
        else:
            transform = target_transform * correction

        box_correction = PyKDL.Frame()
        if box_rf:
            box_correction = target_transform.getKDLFrameFromBoundingBox(
                correction)
        transform = transform * box_correction
        return transform

    def getCorrection(self, transform, cad_correction=True, box_correction=True):
        correction = PyKDL.Frame()
        if cad_correction:
            correction = correction * self.scene_config.component_correction

        if box_correction:
            correction = correction * transform.getKDLFrameFromBoundingBox(
                correction)
        return correction

    def exportSTLData(self, folder, base_transform=True, correction=False, prefix="", box_rf=False):
        s = 0
        shapes = []
        for cad_id in self.id_map:
            trans = self.findTransform(cad_id)
            if trans == None:
                continue
            t_shapes = []
            t = trans.getKDLFrameFromBase()

            whole_shape = trans.getWholeShape()
            shapes.append(whole_shape.transformedShape(t))

            if base_transform:
                whole_shape = whole_shape.transformedShape(t)
            else:
                if correction:
                    corr = self.scene_config.component_correction
                    whole_shape = whole_shape.transformedShape(
                        corr.Inverse())
                    if box_rf:
                        box_rf = whole_shape.getMinimumPointTransform()
                        whole_shape = whole_shape.transformedShape(
                            box_rf.Inverse())

            stl = STLUtils.buildSTLFileBytes([whole_shape])
            f1 = open(folder + '/' + prefix +
                      trans.item_id.cad_id + ".stl", 'wb')
            print "Saving part " + trans.item_id.cad_id + " ..."
            f1.write(stl)

        allstl = STLUtils.buildSTLFileBytes(shapes)
        f1 = open(folder + '/full.stl', 'wb')
        print "Saving gearbox ..."
        f1.write(allstl)

    @staticmethod
    def buildScene(x3d_file, wri_file, scene_configuration=SceneConfiguration()):
        """ Builds scene from a file pair <xml,wri> """
        tree = ET.parse(x3d_file)
        root = tree.getroot()

        #Creo dizionario che collega nome cad di ogni oggetto a label e commerciale
        id_map = {}
        with open(wri_file) as rfile:
            content = rfile.readlines()
            for line in content:
                chunks = line.split('=')
                id_map[chunks[0].strip()] = ItemID(
                    chunks[0].strip(), chunks[1].strip())
        for child in root:
            if child.tag.lower() == 'scene':
                return Scene(child, id_map, scene_configuration)


###############################################################
###############################################################
###############################################################

class Terminal(PyKDL.Frame):
    """ Object representing a Terminal on a Part """

    def __init__(self, node=None, scale_factor=1, part=None):
        super(PyKDL.Frame, self).__init__()
        if node == None:
            return
        self.node = node
        self.part = part

        self.xdir = float(node.attrib.get('xdir')) if node.attrib.get(
            'xdir') != None else 1.0
        self.ydir = float(node.attrib.get('ydir')) if node.attrib.get(
            'ydir') != None else 1.0
        self.zdir = float(node.attrib.get('zdir')) if node.attrib.get(
            'zdir') != None else 1.0

        self.xpos = float(node.attrib.get('xpos')) if node.attrib.get(
            'xpos') != None else 0.0
        self.ypos = float(node.attrib.get('ypos')) if node.attrib.get(
            'ypos') != None else 0.0
        self.zpos = float(node.attrib.get('zpos')) if node.attrib.get(
            'zpos') != None else 0.0

        self.name = node.attrib.get(
            'name') if node.attrib.get('name') != None else ''
        self.typeofterminal = node.attrib.get(
            'typeofterminal') if node.attrib.get('typeofterminal') else 1
        self.direction = node.attrib.get(
            'direction') if node.attrib.get('direction') else 1

        dir_toward_z = self.zdir > 0.5

        az = PyKDL.Vector(
            self.xdir,
            self.ydir,
            self.zdir
        )

        ax = transformations.perpendicular_vector(az)
        ay = az * ax

        self.M = PyKDL.Rotation(
            ax.x(), ay.x(), az.x(),
            ax.y(), ay.y(), az.y(),
            ax.z(), ay.z(), az.z()
        )
        self.p = PyKDL.Vector(
            self.xpos,
            self.ypos,
            self.zpos
        )
        self.p = self.p * scale_factor

    def clone(self):
        cp = type(self)()
        cp.__dict__.update(self.__dict__)
        cp.M = self.M
        cp.p = self.p
        return cp

    def getName(self):
        return self.name


###############################################################
###############################################################
###############################################################

class GearboxItem(object):

    def __init__(self):
        self.gearbox = None
        self.transform = None
        self.linked_transforms = {}

    def linkTransform(self, transform):
        if isinstance(transform, Transform):
            print(str(self), "Linked with", str(transform))
            self.linked_transforms[transform.getName()] = transform

    def setGearbox(self, gearbox):
        self.gearbox = gearbox

    def getRelativeFrame(self):
        if self.gearbox != None:
            return self.gearbox.getRelativeFrame(self)
        else:
            return None

    def getProjectedFrame(self):
        if self.gearbox != None:
            return self.gearbox.getProjectedFrame(self)
        else:
            return None

###############################################################
###############################################################
###############################################################


class Part(GearboxItem):
    """ Object representing a Part component """

    def __init__(self, node, scale_factor=1, copy_of_part=None):
        super(Part, self).__init__()
        self.node = node
        self.transform = None
        self.terminals = []
        self.part_id = node.attrib.get("P_PART_TERMINAL_NAME") if node.attrib.get(
            "P_PART_TERMINAL_NAME") else ''
        for terminal in node:
            self.terminals.append(Terminal(terminal, scale_factor, self))

    def getPartID(self):
        return self.part_id

    def __str__(self):
        return "Part[" + self.part_id + "]"

    def clone(self):
        cp = copy.copy(self)
        cp.terminals = []
        for t in self.terminals:
            t_clone = t.clone()
            t_clone.part = cp
            cp.terminals.append(t_clone)
        return cp

    @staticmethod
    def loadParts(part_file, scale_factor=1):
        """ Load Parts from XML file """
        tree = ET.parse(part_file)
        root = tree.getroot()

        parts = []
        for child in root:
            parts.append(Part(child, scale_factor))
        return parts


###############################################################
###############################################################
###############################################################

class PartCollection(object):
    """ Collection of Parts """

    def __init__(self):
        self.part_map = {}
        self.part_list = []

    def setGearbox(self, gearbox):
        for key, part in self.part_map.iteritems():
            part.setGearbox(gearbox)

    def loadFromFile(self, filename, scale_factor=1):
        parts = Part.loadParts(filename, scale_factor)
        for part in parts:
            self.addPart(part.part_id, part)

    def addPart(self, part_id, part):
        self.part_map[part_id] = part

    def getPartById(self, part_id):
        return self.part_map.get(part_id)

    def getPartByCadId(self, cad_id):
        for p in self.part_list:
            if p.transform:
                if p.transform.getID().getCadID() == cad_id:
                    return p
        return None

    def getPartsById(self, part_id):
        parts = []
        for p in self.part_list:
            if p.getPartID() == part_id:
                parts.append(p)
        return parts


###############################################################
###############################################################
###############################################################
class Channel2TerminalIntersection(object):
    def __init__(self, channel, terminal):
        self.channel = channel
        self.terminal = terminal
        if self.channel.gearbox:
            channel_seg = self.channel.get2DStartEndPoint()

    def getTerminal2DStartEndPoint(self, terminal):
        if terminal.part:
            if terminal.part.gearbox:
                proj = terminal.part.gearbox.getProjectedFrame(terminal)

###############################################################
###############################################################
###############################################################


class ChannelIntersection(object):
    MAX_INTERSECTION_NORM = 10000000

    def __init__(self, channel1, channel2):
        self.channel1 = channel1
        self.channel2 = channel2
        #seg1 = self.channel1.get2DStartEndPoint()
        #seg2 = self.channel2.get2DStartEndPoint()
        #self.intersection_point = gc.seg_intersect(
        #    seg1[0], seg1[1], seg2[0], seg2[1])
        #int_norm = np.linalg.norm(self.intersection_point)
        #self.valid = int_norm < ChannelIntersection.MAX_INTERSECTION_NORM
        dir1 = copy.deepcopy(self.channel1.getChannelDirection())
        dir2 = copy.deepcopy(self.channel2.getChannelDirection())
        point1 = copy.deepcopy(self.channel1.getChannelStart())
        point2 = copy.deepcopy(self.channel2.getChannelStart())
        if not np.array_equal(dir1,dir2):
            self.intersection_point = np.multiply(dir2,point1) + np.multiply(dir1,point2)
            self.valid = True
        else:
            self.valid =False

    def isValid(self):
        return self.valid

    def getFrame(self):
        int_trans = PyKDL.Frame()
        int_trans.p.x(self.intersection_point[0])
        int_trans.p.y(self.intersection_point[1])
        int_trans.p.z(0)
        return int_trans

    def getName(self):
        return "I_" + self.channel1.getName() + "_" + self.channel2.getName()


###############################################################
###############################################################
###############################################################


class Channel(GearboxItem):
    """ Channel representation """
    CHANNEL_PRINCIPAL_LINE_EXTENSION = 100000

    def __init__(self, name, transform):
        super(Channel, self).__init__()
        self.name = name
        self.transform = transform
        self.transforms_poi = []
        self.intersection_list = {}
        

    def setConfig(self):
        self.transform = self.getRelativeFrame()
        self.channel_pose = np.array([self.transform.p.x(), self.transform.p.y()])
        self.channel_main_direction = np.absolute(np.array([round(self.transform.M.UnitZ()[0]), round(self.transform.M.UnitZ()[1])]))

    def getChannelFrame(self):
        int_trans = PyKDL.Frame()
        int_trans.p.x(self.channel_pose[0])
        int_trans.p.y(self.channel_pose[1])
        int_trans.p.z(0)
        return int_trans
    #def get2DStartEndPoint(self):
        #transform = self.getRelativeFrame()
        #P = np.array([transform.p.x(), transform.p.y()])
        #V = np.array([transform.M.UnitZ()[0], transform.M.UnitZ()[1])
        # TODO: Cambiare questo metodo impostando come P1,P2 i minimi e massimi
        # nel ConvexHull della gearbox
        #P1 = P - V * Channel.CHANNEL_PRINCIPAL_LINE_EXTENSION
        #P2 = P + V * Channel.CHANNEL_PRINCIPAL_LINE_EXTENSION
        #return [P1, P2]

    def getChannelStart(self):
        return self.channel_pose

    def getChannelDirection(self):
        return self.channel_main_direction

    def __str__(self):
        return "Channel[" + self.name + "]"

    def getName(self):
        return self.name

###############################################################
###############################################################
###############################################################


class ChannelCollection(object):
    """ Channel representation """

    def __init__(self):
        self.channel_map = {}

    def setGearbox(self, gearbox):
        for key, channel in self.channel_map.iteritems():
            channel.setGearbox(gearbox)

    def setChannelConfig(self):
        for key, channel in self.channel_map.iteritems():
            channel.setConfig()

    def loadFromTransforms(self, transform_list):
        for trans in transform_list:
            if trans.isUseful():
                if trans.getID().getType() == ItemType.CHANNEL:
                    self.channel_map[trans.getID().getSheetID()] = Channel(
                        trans.getID().getSheetID(), trans)

        for trans in transform_list:
            if trans.isUseful():
                if trans.getID().getType() == ItemType.CHANNEL_POI:
                    self.channel_map.get(
                        trans.getID().getSheetID()).transforms_poi.append(trans)

    def getChannelById(self, sheet_id):
        return self.channel_map.get(sheet_id)

    def getIntersections(self):
        intersections = []
        for key1, channel1 in self.channel_map.iteritems():
            for key2, channel2 in self.channel_map.iteritems():
                if key1 != key2:
                    intersection = ChannelIntersection(channel1, channel2)
                    if intersection.isValid():
                        self.channel_map[key1].intersection_list[key2] = intersection.getFrame()
                        intersections.append(intersection)
        return intersections


###############################################################
###############################################################
###############################################################


class GearboxConfiguration(object):

    def __init__(self):
        self.scene_config = SceneConfiguration()

###############################################################
###############################################################
###############################################################


class Gearbox(object):
    """ Gearbox model """

    def __init__(self, name, folder, gearbox_config=GearboxConfiguration()):
        self.name = name                                                #name of the cad file of the gearbox
        self.folder = folder                                            #path per il file cad
        self.gearbox_config = gearbox_config                            #configurazione della scena, definisce rotazione default per sistemi di ref
        self.files = self.retrieveFilesNames()                          #crea lista nomi file (_cad.x3d / ids.wri / pins.xml)
        #print self.files                                                #display lista a schermo

        # Load Scene
        self.scene = Scene.buildScene(
            self.files['cad'], self.files['ids'], self.gearbox_config.scene_config) #genera la scena dato il modello cad e la lista componenti

        # Useful Transforms
        self.useful_transforms = []                                                 #definisce il vettore di tf valide nella scena
        self.scene.deepSearch(self.useful_transforms, only_useful=True)             #ricava ricorsivamente le varie classi transform

        # Load Parts
        #self.parts = PartCollection()
        #self.parts.loadFromFile(
        #    self.files['pins'], self.gearbox_config.scene_config.scale)
        #self.parts.setGearbox(self)

        # Load Channels
        self.channels = ChannelCollection()
        self.channels.loadFromTransforms(self.useful_transforms)
        self.channels.setGearbox(self)
        self.channels.setChannelConfig()
        #self.channels.getIntersections()
        # Graph Linking
        #self.linkPartsWithCad()

        # Base
        self.base = self.findBase()

    # get transformations for any part in wri
    def linkPartsWithCad(self):
        for key, part in self.parts.part_map.iteritems():
            # list of transformation associated with a specific commercial ID
            transforms = self.findTransformByCommercialId(key)
            for t in transforms:
                part_clone = part.clone()
                part_clone.transform = t
                # generate list of parts in the CAD
                self.parts.part_list.append(part_clone)

    def getRootTransform(self):
        if len(self.useful_transforms) > 0:
            return self.useful_transforms[0].getRoot()
        else:
            return None

    def getChannel(self, channel_sheet_id):
        return self.channels.getChannelById(channel_sheet_id)

    def getParts(self, part_commercial_id):
        return self.parts.getPartsById(part_commercial_id)

    def findTransformByCommercialId(self, commercial_id):
        """ Retrieves a list of Transforms with target commercial_id """
        transforms = []
        for t in self.useful_transforms:
            if t.getID().getCommercialID() == commercial_id:
                transforms.append(t)
        return transforms

    def findBase(self):
        for trans in self.useful_transforms:
            if trans.isUseful():
                if trans.getID().getType() == ItemType.BASE:
                    return trans

    def getRelativeFrame(self, element):
        """ Gets the relative transform of generic element w.r.t. the Gearbox RF """
        if isinstance(element, Channel):
            channel = element
            correction = self.scene.getCorrection(
                channel.transform.getRoot(), box_correction=False)
            transform = correction.Inverse() * channel.transform.getKDLFrameFromBase()
            transform.M.DoRotY(3.14 / 2.0)
            return transform

        if isinstance(element, Transform):
            correction = self.scene.getCorrection(
                element.getRoot(), box_correction=False)
            transform = correction.Inverse() * element.getKDLFrameFromBase()
            transform.M.DoRotY(3.14 / 2.0)
            return transform

        if isinstance(element, Part):
            if element.transform:
                correction = self.scene.getCorrection(
                    element.transform.getRoot(), box_correction=False)
                transform = correction.Inverse() * element.transform.getKDLFrameFromBase()
                boxed_transform = self.scene.getCorrection(element.transform)
                transform = transform * boxed_transform
                return transform

        if isinstance(element, Terminal):
            if element.part:
                part_transform = self.getRelativeFrame(element.part)
                return part_transform * element

    def getProjectedFrame(self, element):
        """
        Gets the projected transform of generic element w.r.t. the Gearbox RF.
        As a planar representation
        """
        if isinstance(element, Transform):
            correction = self.scene.getCorrection(
                element.getRoot(), box_correction=False)
            transform = correction.Inverse() * element.getKDLFrameFromBase()
            boxed_transform = self.scene.getCorrection(element)
            transform = transform * boxed_transform
            return transformations.selfProjection(transform)

        if isinstance(element, Part):
            if element.transform:
                return self.getProjectedFrame(element.transform)

        if isinstance(element, Terminal):
            terminal_frame = self.getRelativeFrame(element)
            return transformations.selfProjection(terminal_frame)

    def retrieveFilesNames(self):
        file_map = {
            "cad": os.path.join(self.folder, self.getFileNameForCad()),
            "ids": os.path.join(self.folder, self.getFileNameForIds()),
            "pins": os.path.join(self.folder, self.getFileNameForPins()),
        }
        return file_map

    def getFileNameForCad(self):
        return self.name + "_cad.x3d"

    def getFileNameForIds(self):
        return self.name + "_ids.wri"

    def getFileNameForPins(self):
        return self.name + "_pins.xml"


class Component(object):

    def __init__(self):
        self.name = []

    def parenttf(self, mycomponent, parent):
        """ this function correct the cad reference frame for the component composed by 2 sub coponent and return the tf in the both left corner"""

        path_to_data_folder = Parameters.get(param="PACKAGE_DATA_FOLDER")
        path = path_to_data_folder + '/gearbox_zero/Component_tf.xml'

        tree = ET.parse(path)
        root = tree.getroot()

        newparent = parent

        for code in root:
            if code.tag == 'code':
                if code.get('name') == mycomponent:
                    for item in code.findall('item'):
                        # crete a cornen reference system if is not present
                        if item.get('name') == 'corner':
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
                            newparent = PyKDL.Frame()
                            newparent.p = PyKDL.Vector(x, y, z)
                            newparent.M.DoRotX(R)
                            newparent.M.DoRotY(P)
                            newparent.M.DoRotZ(Y)

        return newparent

    # Read the xml file (Component_tf) and add the tf contained in the

    def addtf(self, cad_id, folder_path=None, cad_id_file_name=None, component_file_name=None):
        """ this function read the xml file for create a list with all the tf of the given component and return it. 
        this function don't return the corner tf"""

        if folder_path is None or cad_id_file_name is None or component_file_name is None:
            folder_path = Parameters.get(
                param="PACKAGE_DATA_FOLDER") + "/gearbox_zero/"
            cad_id_file_name = 'gearbox00001_ids.txt'
            component_file_name = 'Component_tf.xml'
        cad_id_file = os.path.join(folder_path, cad_id_file_name)
        component_file = os.path.join(folder_path, component_file_name)

        conversion = GearboxPartsData(folder_path, cad_id_file)
        dic = conversion.getComponentIDs(cad_id)
        if dic is None:
            return []

        mycomponent = dic["product"]

        tree = ET.parse(component_file)
        root = tree.getroot()

        tf_list = []

        for code in root:
            if code.get('name') == mycomponent:
                for item in code.findall('item'):
                    # create the reference system for all of part
                    if not item.get('name') == 'corner':
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
                        name = cad_id + '_' + item.get('name')
                        tf = PyKDL.Frame()
                        tf.p = PyKDL.Vector(x, y, z)
                        tf.M.DoRotX(R)
                        tf.M.DoRotY(P)
                        tf.M.DoRotZ(Y)
                        single_tf = []
                        single_tf.append(name)
                        single_tf.append(tf)
                        tf_list.append(single_tf)
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
                        if subitem.get('name') == 'hole':
                            name = cad_id + '_' + 'hole' + couple.get('name')
                        else:
                            name = cad_id + '_' + 'screw' + couple.get('name')
                        tf = PyKDL.Frame()
                        tf.p = PyKDL.Vector(x, y, z)
                        tf.M.DoRotX(R)
                        tf.M.DoRotY(P)
                        tf.M.DoRotZ(Y)
                        single_tf = []
                        single_tf.append(name)
                        single_tf.append(tf)
                        tf_list.append(single_tf)

        return tf_list
