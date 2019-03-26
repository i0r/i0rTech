# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

# <pep8 compliant>
#

bl_info = {
	"name": "i0rTech UI",
	"author": "i0r",
	"version": (1, 0, 0),
	"blender": (2, 71, 0),
	"location": "View3D > Toolshelf",
	"warning": "",
	"description": "i0rTech UI",
	"wiki_url": "",
	"category": "3D View",
}

import bpy
from bpy.types import Menu, Panel, UIList, PropertyGroup
from bpy.props import StringProperty, BoolProperty, IntProperty, CollectionProperty, BoolVectorProperty, PointerProperty
from bpy.app.handlers import persistent
	
class LayoutDemoPanel(bpy.types.Panel):
	"""Creates a Panel in the scene context of the properties editor"""
	bl_label = "i0rTech"
	bl_idname = "SCENE_PT_layout"
	bl_context = "scene"
	bl_space_type = 'VIEW_3D'
	bl_region_type = 'UI'
	bl_category = 'Window'
	
	def draw(self, context):
		layout = self.layout
		scene = context.scene

		box_lvl = layout.box() 
		box_obj = layout.box()
		
# ***************************************************************************************	

		box_lvl.label(text="Level", icon='WORLD')
		row = box_lvl.row(align=True)
		row.prop(scene, "name")

		row = box_lvl.row(align=True)
		row.prop(scene, "weather")
		
# ***************************************************************************************
		if len(context.selected_objects):
			obj = context.selected_objects[0]
			
			box_obj.label(text="Object", icon='VIEW3D')
			row = box_obj.row(align=True)
			row.prop(obj, "name")
			row = box_obj.row(align=True)
			row.prop(obj, "entity")
			row = box_obj.row(align=True)
			row.prop(obj, "lod")

def registerProps():
	weather_types = [
				("Clear","Clear","Clear"),
				("Cloudy","Cloudy","Cloudy"),
				("Rainy","Rainy","Rainy"),
				("Greyish","Greyish","Greyish"),
				("Fog","Fog","Fog"),
				]
				
	entity_types = [
				("Mesh","Mesh","Mesh"),
				("Terrain","Terrain","Terrain"),
				("Spawn","Spawn","Spawn"),
				("Cubemap","Cubemap","Cubemap"),
				]
				
	bpy.types.Scene.weather = bpy.props.EnumProperty(name="Weather", description="Weather of the level (can be overridden with the console).", default="Clear", items=weather_types) 
	bpy.types.Scene.name = bpy.props.StringProperty(name = "Name", description="Name of the level.", default = "New Scene")
	bpy.types.Object.entity = bpy.props.EnumProperty(name="Entity Type", description="Entity type of the object.", default="Mesh", items=entity_types) 
	bpy.types.Object.lod = bpy.props.FloatProperty(name="LOD", description="Level Of Detail (in game units) of the object.", default=0.0, min=0.0) 

def register():
	bpy.utils.register_module(__name__)
	registerProps()
	
 
def unregister():
	bpy.utils.unregister_module(__name__)

if __name__ == "__main__":
	register()
