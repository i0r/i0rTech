# <pep8 compliant>
#
# 1.0

import os
import time
import struct
import math

import bpy
import bpy_extras.io_utils
import mathutils
import random

from array import *
from collections import namedtuple
from mathutils import Vector
from random import randint

mesh_vao_start = 0
mesh_vbo_start = 0

def mesh_triangulate( me ):
 import bmesh
 bm = bmesh.new()
 bm.from_mesh( me )
 bmesh.ops.triangulate( bm, faces = bm.faces )
 bm.to_mesh( me )
 bm.free()

def write_bloc_size( file, offset, bloc_start ):
 current_position = file.tell()
 file.seek( offset, 0 )
 file.write( struct.pack( 'I', ( current_position - bloc_start ) ) )
 file.seek( current_position, 0 )
 
def write_entity_count( file, offset, count ):
 current_position = file.tell()
 file.seek( offset, 0 )
 file.write( struct.pack( 'I', count ) )
 file.seek( current_position, 0 )
 
def write_padding( file ):	
 while file.tell() % 16 != 0:
  file.write( struct.pack( 'B', 0xFF ) )
   
def write_header( file, version ):
 file.write( bytearray( 'ISGE', 'utf-8' ) )
 file.write( bytes( version ) )
 file.write( struct.pack( 'B', 0xFF ) ) # explicit padding
 file.write( struct.pack( 'I', 0 ) )
 file.write( struct.pack( 'I', 0 ) )
 write_padding( file )

def write_mesh( file, global_matrix ):
 global mesh_vao_start
 global mesh_vbo_start
 
 for obj in bpy.context.scene.objects:
  bpy.context.scene.objects.active = obj
  obj.select = True
  
  if obj.type == 'MESH' and obj.entity == 'Mesh':
   mesh = obj.to_mesh( bpy.context.scene, True, 'PREVIEW', calc_tessface=True )
   transform_matrix = global_matrix * obj.matrix_world
   mesh.transform( transform_matrix ) # pretransform the model (so that we dont need a matrix)
   
   mesh_triangulate( mesh )
   mesh.calc_tessface()
   mesh.calc_tangents()
   
   mesh_indices = array('I')
   mesh_vbo_start = file.tell()
   i = 0 
   for face in mesh.polygons:
    for loop_index in face.loop_indices:
     vertex = mesh.vertices[mesh.loops[loop_index].vertex_index]
     mesh_indices.append( i )

     file.write( struct.pack( 'f', vertex.co.x ) )
     file.write( struct.pack( 'f', vertex.co.y ) )
     file.write( struct.pack( 'f', vertex.co.z ) )      
     file.write( struct.pack( 'f', mesh.uv_layers.active.data[loop_index].uv[0] ) )
     file.write( struct.pack( 'f', mesh.uv_layers.active.data[loop_index].uv[1] ) )
     
     i += 1

   write_bloc_size( file, 8, mesh_vbo_start )
 
   mesh_vao_start = file.tell()  
   for x in mesh_indices:
      file.write( struct.pack( 'I', x ) )
      
   write_bloc_size( file, 12, mesh_vao_start )
   
   break
   
def save(filepath, global_matrix, version):					
 bpy.ops.object.mode_set( mode='OBJECT' )
 file = open( filepath, 'w+b' )
 
 #=============================================================================
 
 write_header( file, version ) 
 write_mesh( file, global_matrix )

 #=============================================================================

 file.close()
 return {'FINISHED'}

