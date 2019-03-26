import os
import time
import struct
import math

import bpy
import bpy_extras.io_utils
import mathutils
import random
import fnmatch
import collections

from array import *
from collections import namedtuple
from mathutils import Vector
from random import randint

#==============================================================================

def dump(obj):
 for attr in dir(obj):
  if hasattr(obj, attr):
   print("obj.%s = %r" % (attr, getattr(obj, attr)))

#==============================================================================

def get_obj_bounds(obj, local=False):
 local_coords = obj.bound_box[:]
 om = obj.matrix_world

 if not local:    
  worldify = lambda p: om * Vector(p[:]) 
  coords = [worldify(p).to_tuple() for p in local_coords]
 else:
  coords = [p[:] for p in local_coords]

 rotated = zip(*coords[::-1])

 push_axis = []
 for (axis, _list) in zip('xyz', rotated):
  info = lambda: None
  info.max = max(_list)
  info.min = min(_list)
  info.distance = info.max - info.min
  push_axis.append(info)

 originals = dict(zip(['x', 'y', 'z'], push_axis))
 o_details = collections.namedtuple('object_details', 'x y z')
 return o_details(**originals)    
 
#==============================================================================

def mesh_triangulate( me ):
 import bmesh
 bm = bmesh.new()
 bm.from_mesh( me )
 bmesh.ops.triangulate( bm, faces = bm.faces )
 bm.to_mesh( me )
 bm.free()

#==============================================================================

def write_bloc_size( file, offset, bloc_start ):
 current_position = file.tell()
 file.seek( offset, 0 )
 file.write( struct.pack( 'I', ( current_position - bloc_start ) ) )
 file.seek( current_position, 0 )
 
#==============================================================================

def write_entity_count( file, offset, count ):
 current_position = file.tell()
 file.seek( offset, 0 )
 file.write( struct.pack( 'I', count ) )
 file.seek( current_position, 0 )
 
#==============================================================================

def write_padding( file ):	
 while file.tell() % 16 != 0:
  file.write( struct.pack( 'B', 0xFF ) )
   
#==============================================================================

def write_header( file, version ):
 file.write( bytearray( 'I0RS', 'utf-8' ) )
 file.write( bytes( version ) )
 write_padding( file )

#==============================================================================

def write_spawn_point( file, global_matrix ):
 file.write( bytearray( 'SPWN', 'utf-8' ) )
 entity_count_pos = file.tell()
 file.write( struct.pack( 'I', 0 ) )
 entity_size_pos = file.tell()
 file.write( struct.pack( 'I', 0 ) )
 write_padding( file )
 
 spawn_count = 0
 entity_bloc_start = file.tell()
 
 for obj in bpy.context.scene.objects:
  if obj.entity == "Spawn":
   spawn_count += 1
   
   spawn_pos = global_matrix * obj.location
   for x in spawn_pos:
    file.write( struct.pack( 'f', x ) )
   
   spawn_look_at = mathutils.Euler( ( obj.rotation_euler[0], obj.rotation_euler[2], obj.rotation_euler[1] ) )
   
   for x in spawn_look_at:
    file.write( struct.pack( 'f',	x ) )
   
   write_padding( file )
 
 write_entity_count( file, entity_count_pos, spawn_count )
 write_bloc_size( file, entity_size_pos, entity_bloc_start )

#==============================================================================

def write_direc_light( file, global_matrix ):
 file.write( bytearray( 'LDIR', 'utf-8' ) )
 entity_count_pos = file.tell()
 file.write( struct.pack( 'I', 0 ) )
 entity_size_pos = file.tell()
 file.write( struct.pack( 'I', 0 ) )
 write_padding( file )
 
 dir_light_count = 0
 entity_bloc_start = file.tell()
 
 for obj in bpy.context.scene.objects:
  if obj.type == 'LAMP' and obj.data.type == 'SUN':
   dir_light_count += 1
   
   sun_dir_quaternion = mathutils.Quaternion( ( obj.rotation_quaternion[1], obj.rotation_quaternion[2], obj.rotation_quaternion[3], obj.rotation_quaternion[0] ) )
   
   # this should be read as a 4D vector ( where the 4 element is light energy )  
   for x in sun_dir_quaternion:
     file.write( struct.pack( 'f', x ) )
   file.write( struct.pack( 'f', obj.data.energy ) )
   
   for x in obj.data.color:
     file.write( struct.pack( 'f', x ) )
 
 write_entity_count( file, entity_count_pos, dir_light_count )
 write_bloc_size( file, entity_size_pos, entity_bloc_start )
 
#==============================================================================

def write_point_light( file, global_matrix ):
 file.write( bytearray( 'LPNT', 'utf-8' ) )
 entity_count_pos = file.tell()
 file.write( struct.pack( 'I', 0 ) )
 entity_size_pos = file.tell()
 file.write( struct.pack( 'I', 0 ) )
 write_padding( file )
 
 point_light_count = 0
 entity_bloc_start = file.tell()
 
 for obj in bpy.context.scene.objects:
  if obj.type == 'LAMP' and obj.data.type == 'POINT':
   point_light_count += 1
   
   light_position = global_matrix * obj.location

   # this should be read as a 4D vector ( where the 4 element is light energy ) 
   for x in light_position:
     file.write( struct.pack( 'f', x ) )
   file.write( struct.pack( 'f', obj.data.energy ) )
     
   for x in obj.data.color:
     file.write( struct.pack( 'f', x ) )	 
   file.write( struct.pack( 'I', 0xFFFFFFFF ) )

   file.write( struct.pack( 'f', obj.data.quadratic_attenuation ) )
   file.write( struct.pack( 'f', obj.data.linear_attenuation ) )
   file.write( struct.pack( 'f', obj.data.distance ) )
   
   write_padding( file )
   
 write_entity_count( file, entity_count_pos, point_light_count )
 write_bloc_size( file, entity_size_pos, entity_bloc_start )

#==============================================================================

def texture_path_game_relative( path ):
 return path[path.find('Textures')+9:].replace( '\\', '/' )
 
#==============================================================================

def write_texture( file ):
 file.write( bytearray( 'TEXD', 'utf-8' ) )
 entity_count_pos = file.tell()
 file.write( struct.pack( 'I', 0 ) )
 entity_size_pos = file.tell()
 file.write( struct.pack( 'I', 0 ) )
 write_padding( file )
 
 tex_count = 0
 tex_list = {}
 entity_bloc_start = file.tell()
 
 for obj in bpy.context.scene.objects:
  for mat_slot in obj.material_slots:
   for mtex_slot in mat_slot.material.texture_slots:       
    if mtex_slot:
     texPath = texture_path_game_relative( mtex_slot.texture.image.filepath )
     texPathHash = hash( texPath ) % ( 10 ** 8 )
     
     if texPathHash not in tex_list:
      file.write( struct.pack( 'I', texPathHash ) )
      file.write( struct.pack( 'I', len( texPath ) ) )
      file.write( bytearray( texPath, 'utf-8' ) )
      
      tex_list[texPathHash] = texPath
      tex_count += 1

 write_entity_count( file, entity_count_pos, tex_count )
 write_bloc_size( file, entity_size_pos, entity_bloc_start )
 write_padding( file )
 
#==============================================================================
 
def material_get_surface_type( material ):
 mat_flags = 0x0
 
 if material.diffuse_shader == "LAMBERT":
  mat_flags = mat_flags | 2**0
 elif material.diffuse_shader == "OREN-NAYAR":
  mat_flags = mat_flags | 2**1
 elif material.diffuse_shader == "MINNAERT": # i0r Burley
  mat_flags = mat_flags | 2**0
  mat_flags = mat_flags | 2**1
 elif material.diffuse_shader == "FRESNEL": # i0r Gotanda
  mat_flags = mat_flags | 2**2
 else:
  print( "***FATAL ERROR >> UNKNOWN DIFFUSE MODEL %s***" % material.diffuse_shader )

 if material.specular_shader == "COOKTORR":
  mat_flags = mat_flags | 2**4
 elif material.specular_shader == "BLINN":
  mat_flags = mat_flags | 2**5
 elif material.specular_shader == "PHONG": # i0r GGX
  mat_flags = mat_flags | 2**4
  mat_flags = mat_flags | 2**5
 elif material.specular_shader == "WARDLSO": # i0r Beckmann
  mat_flags = mat_flags | 2**6
 else:
  print( "***FATAL ERROR >> UNKNOWN SPECULAR MODEL %s***" % material.specular_shader )
  
 return mat_flags

#==============================================================================

def material_get_flags( material ):
 mat_flags = 0x00000000
 
 if material.use_shadeless: # IsVisible
  mat_flags = mat_flags | 2**0
 
 if material.use_cast_shadows: # CastShadows
  mat_flags = mat_flags | 2**1
  
 if material.use_shadows: # ReceiveShadows
  mat_flags = mat_flags | 2**2
  
 if material.raytrace_mirror.use: # Cast Reflections
  mat_flags = mat_flags | 2**3

 if material.use_transparency: # IsAlphaTested
  mat_flags = mat_flags | 2**5
 
 # à implémenter...
 if False:
  mat_flags = mat_flags | 2**4 # IsGlobalIlluminated   
  mat_flags = mat_flags | 2**6 # IsStaticEnvMapped
  mat_flags = mat_flags | 2**7 # IsDynamicEnvMapped    
  mat_flags = mat_flags | 2**8 # IsPlannarReflected
   
 return mat_flags

#==============================================================================

def get_texture_type( tex ):
 if tex.use_map_diffuse:
  return 0
 elif tex.use_map_specular:
  return 1
 elif tex.use_map_normal:
  return 2
 elif tex.use_map_hardness:
  return 3
 elif tex.use_map_alpha:
  return 4
 elif tex.use_map_ambient:
  return 5
 elif tex.use_map_emission:
  return 6
 else:
  return 0

#==============================================================================

def write_material( file ):
 file.write( bytearray( 'MATE', 'utf-8' ) )
 entity_count_pos = file.tell()
 file.write( struct.pack( 'I', 0 ) )
 entity_size_pos = file.tell()
 file.write( struct.pack( 'I', 0 ) )
 write_padding( file )
 
 material_count = 0
 entity_bloc_start = file.tell()
 
 mat_list = []
 for obj in bpy.context.scene.objects:
  for mat_slot in obj.material_slots:
   matHash = hash( mat_slot.name ) % ( 10 ** 8 )
   
   if matHash not in mat_list:
    mat_list.append( matHash )
    file.write( struct.pack( 'I', matHash ) )   
    
    file.write( struct.pack( 'B', material_get_surface_type( mat_slot.material ) ) )
    file.write( struct.pack( 'B', int( mat_slot.material.diffuse_intensity * 255.0 ) ) )
    file.write( struct.pack( 'B', int( mat_slot.material.specular_intensity * 255.0 ) ) )
    file.write( struct.pack( 'B', int( mat_slot.material.specular_hardness / 511.0 * 255.0 ) ) )
    
    file.write( struct.pack( 'B', int( mat_slot.material.emit * 255.0 ) ) )
    file.write( struct.pack( 'B', int( mat_slot.material.ambient * 255.0 ) ) )
    file.write( struct.pack( 'B', int( mat_slot.material.alpha * 255.0 ) ) )
    file.write( struct.pack( 'B', 0 ) )
    
    file.write( struct.pack( 'I', material_get_flags( mat_slot.material ) ) )
    
    texmap_count_offset = file.tell()
    file.write( struct.pack( 'I', 0 ) )
    
    for x in mat_slot.material.diffuse_color:
     file.write( struct.pack( 'f', x ) )
     
    for x in mat_slot.material.specular_color:
     file.write( struct.pack( 'f', x ) )
     
    write_padding( file )
    
    texmap_count = 0
    for mtex_slot in mat_slot.material.texture_slots:
     if mtex_slot and mtex_slot.texture.image.name:
      texPath = texture_path_game_relative( mtex_slot.texture.image.filepath )
      file.write( struct.pack( 'I', hash( texPath ) % ( 10 ** 8 ) ) )
      
      file.write( struct.pack( 'I', get_texture_type( mtex_slot ) ) )
      
      file.write( struct.pack( 'B', int( mtex_slot.texture.intensity * 255.0 ) ) )
      file.write( struct.pack( 'B', 0 ) )
      file.write( struct.pack( 'B', 0 ) )
      file.write( struct.pack( 'B', 0 ) )
      
      texmap_count += 1
      
    material_count += 1
    
   write_entity_count( file, texmap_count_offset, texmap_count )
   write_padding( file ) 
   
 write_entity_count( file, entity_count_pos, material_count )
 write_bloc_size( file, entity_size_pos, entity_bloc_start )

#==============================================================================

def is_normal_mapped( texture_slots ):
 result = False
 
 for mtex_slot in texture_slots:
  if result:
   break
  if mtex_slot and mtex_slot.texture.image.name:
    result = mtex_slot.use_map_normal
    
 return result
 
#==============================================================================

def write_mesh( file, lod, mesh, isNormalMapped ):
 file.write( struct.pack( 'f', lod ) )
 
 mesh_vbo_offset = file.tell()
 file.write( struct.pack( 'I', 0 ) ) # vbo size
 
 mesh_vao_offset = file.tell()
 file.write( struct.pack( 'I', 0 ) ) # vao size
 
 write_padding( file )
 
 mesh_indices = array('I')
 i = 0
 
 mesh_vbo_start = file.tell()
 for face in mesh.polygons:
  for loop_index in face.loop_indices:
   vertex = mesh.vertices[mesh.loops[loop_index].vertex_index]
   mesh_indices.append( i )

   file.write( struct.pack( 'f', vertex.co.x ) )
   file.write( struct.pack( 'f', vertex.co.y ) )
   file.write( struct.pack( 'f', vertex.co.z ) )
   
   if mesh.uv_layers.active:
    file.write( struct.pack( 'f', mesh.uv_layers.active.data[loop_index].uv[0] ) )
    file.write( struct.pack( 'f', 1.0 - mesh.uv_layers.active.data[loop_index].uv[1] ) )
   
   file.write( struct.pack( 'f', vertex.normal.x ) )
   file.write( struct.pack( 'f', vertex.normal.y ) )
   file.write( struct.pack( 'f', vertex.normal.z ) )
   
   if isNormalMapped:
    file.write( struct.pack( 'f', mesh.loops[loop_index].tangent.x ) )
    file.write( struct.pack( 'f', mesh.loops[loop_index].tangent.y ) )
    file.write( struct.pack( 'f', mesh.loops[loop_index].tangent.z ) )
    
    bitangent = mesh.loops[loop_index].bitangent_sign * vertex.normal.cross( mesh.loops[loop_index].tangent )
    
    file.write( struct.pack( 'f', bitangent.x ) )
    file.write( struct.pack( 'f', bitangent.y ) )
    file.write( struct.pack( 'f', bitangent.z ) )
    
   i += 1
     
   write_bloc_size( file, mesh_vbo_offset, mesh_vbo_start )   
  
 mesh_vao_start = file.tell()  
 for x in mesh_indices:
  file.write( struct.pack( 'I', x ) )
      
 write_bloc_size( file, mesh_vao_offset, mesh_vao_start )
 write_padding( file )
   
#==============================================================================

def write_geometry( file, global_matrix ):
 file.write( bytearray( 'GEOM', 'utf-8' ) )
 entity_count_pos = file.tell()
 file.write( struct.pack( 'I', 0 ) )
 entity_size_pos = file.tell()
 file.write( struct.pack( 'I', 0 ) )
 write_padding( file )

 mesh_count = 0
 entity_bloc_start = file.tell()
   
 #=============================================================================

 for obj in bpy.context.scene.objects:
  if obj.type == 'MESH' and obj.entity == 'Mesh':
   if obj.lod != 0.0: # skip if the mesh is a lod
    print( "Skipping LOD %s..." % obj.name )
    continue
    
   mesh_count += 1
   
   # prepare the mesh...
   mesh = obj.to_mesh( bpy.context.scene, True, 'PREVIEW', calc_tessface=True )
   mesh_triangulate( mesh )
   mesh.calc_tessface()
   mesh.calc_tangents()
   
   mesh_vbo_feature = 0
   
   if mesh.uv_layers.active:
    mesh_vbo_feature = mesh_vbo_feature | 2**0
 
   matHash = 0
   isNormalMapped = False
   for mat_slot in obj.material_slots:
    if mat_slot.name:
     matHash = hash( mat_slot.name ) % ( 10 ** 8 )
     
     isNormalMapped = is_normal_mapped( mat_slot.material.texture_slots )
     if isNormalMapped:
      mesh_vbo_feature = mesh_vbo_feature | 2**1
     
     break
   
   mesh_lods = [obj_lod for obj_lod in bpy.context.scene.objects if obj_lod.parent == obj and obj_lod.lod != 0.0]
   parent_hashcode = 0
   
   if obj.parent:
    print( "Parent %s " % obj.parent.name )
    parent_hashcode = hash( obj.parent.name ) % ( 10 ** 8 )
    
   file.write( struct.pack( 'I', hash( obj.name ) % ( 10 ** 8 ) ) )
   file.write( struct.pack( 'I', parent_hashcode ) )
   file.write( struct.pack( 'I', matHash ) ) 
   file.write( struct.pack( 'I', len( mesh_lods ) + 1 ) ) # total lod count + LOD0 (ofc)
   
   file.write( struct.pack( 'I', mesh_vbo_feature ) ) # vbo size  
   file.write( struct.pack( 'I', 0xFFFFFFFF ) )
   file.write( struct.pack( 'I', 0xFFFFFFFF ) ) 
   file.write( struct.pack( 'I', 0xFFFFFFFF ) )
      
   obj_bounds = get_obj_bounds( obj )
         
   ROTATE_X_PI2 = mathutils.Quaternion((0.0, 0.0, 1.0), math.radians(90.0)).to_matrix().to_4x4()
   matrix_world = ROTATE_X_PI2 * obj.matrix_world
   position, quaternion, scale = matrix_world.decompose()

   file.write( struct.pack( 'f', position.x ) )
   file.write( struct.pack( 'f', position.z ) )
   file.write( struct.pack( 'f', position.y ) )
   file.write( struct.pack( 'I', 0 ) )
   
   file.write( struct.pack( 'f', obj_bounds.x.distance ) )
   file.write( struct.pack( 'f', obj_bounds.z.distance ) )
   file.write( struct.pack( 'f', obj_bounds.y.distance ) )
   file.write( struct.pack( 'I', 0 ) )

   mesh_position = Vector( ( position[0], position[2], position[1] ) )
   for x in mesh_position:
     file.write( struct.pack( 'f', x ) )
   file.write( struct.pack( 'I', 0 ) )

   mesh_orientation = mathutils.Quaternion((quaternion[3], quaternion[2], quaternion[1], quaternion[0])) # wxyz to xyzw
   for x in mesh_orientation:
     file.write( struct.pack( 'f', x ) )
	 
   mesh_scale = Vector( ( scale[0], scale[2], scale[1] ) )
   for x in mesh_scale:
     file.write( struct.pack( 'f', x ) )
   file.write( struct.pack( 'I', 0 ) )
   
   
   file.write( struct.pack( 'I', 0 ) ) # poids obj
   file.write( struct.pack( 'I', 0 ) ) # resistance  
   file.write( struct.pack( 'I', 0 ) ) # resistance2
   file.write( struct.pack( 'I', 0 ) ) # resistance3
   
   file.write( struct.pack( 'I', 0 ) ) # velocite
   file.write( struct.pack( 'I', 0 ) )
   file.write( struct.pack( 'I', 0 ) )
   file.write( struct.pack( 'I', 0 ) ) # implicit alignement padding
   
   write_padding( file )
   
   write_mesh( file, obj.lod, mesh, isNormalMapped )
   
   for lod in mesh_lods:
    mesh_lod = lod.to_mesh( bpy.context.scene, True, 'PREVIEW', calc_tessface=True )
    mesh_triangulate( mesh_lod )
    mesh_lod.calc_tessface()
    mesh_lod.calc_tangents()
    write_mesh( file, lod.lod, mesh_lod, isNormalMapped )
 
 write_entity_count( file, entity_count_pos, mesh_count )
 write_bloc_size( file, entity_size_pos, entity_bloc_start )
 
#==============================================================================

def save(filepath, global_matrix, version):					
 bpy.ops.object.mode_set( mode='OBJECT' )
 file = open( filepath, 'w+b' )
 
 #=============================================================================
 
 write_header( file, version )
 
 #=============================================================================
 
 write_spawn_point( file, global_matrix )
 
 write_direc_light( file, global_matrix )
 write_point_light( file, global_matrix )
 #write_spot_light( file, global_matrix )
 #write_area_light( file, global_matrix )
 
 write_texture( file ) 
 write_material( file )
 write_geometry( file, global_matrix )
 
 #=============================================================================

 file.close()
 return {'FINISHED'}

