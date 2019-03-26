# <pep8-80 compliant>

version = ( 1, 1, 0 )

bl_info = {
	"name": "i0rTech Scene",
	"author": "i0r",
	"version": ( 1, 1, 0 ),
	"blender": (2, 77, 0),
	"location": "File > Import-Export",
	"description": "Export .its (I0rTech Scene)",
	"warning": "",
	"wiki_url": ""
				"",
	"support": 'OFFICIAL',
	"category": "Import-Export"}

if "bpy" in locals():
	import imp
	if "export_i0r" in locals():
		imp.reload(export_i0r)

import bpy
from bpy.props import (EnumProperty,StringProperty)
from bpy_extras.io_utils import (ImportHelper,
							 ExportHelper,
							 path_reference_mode,
							 axis_conversion,
							 )

class Exporti0rScene(bpy.types.Operator, ExportHelper):
	bl_idname = "export_i0r.its"
	bl_label = 'Export i0rTech Scene'
	bl_options = {'PRESET'}

	filename_ext = ".its"
	filter_glob = StringProperty(
			default="*.its;",
			options={'HIDDEN'},
			)
 
	path_mode = path_reference_mode
	check_extension = True

	def execute(self, context):
		from . import export_i0r
		from mathutils import Matrix
		keywords = self.as_keywords(ignore=("filter_glob","path_mode","filepath","check_existing",))
		global_matrix = (Matrix.Scale(1.0, 4) * axis_conversion(to_forward='-Z',to_up='Y',).to_4x4())

		keywords["global_matrix"] = global_matrix
		keywords["version"] = version
		return export_i0r.save(self.filepath, **keywords)
		
	def draw(self, context):
		layout = self.layout
		row = layout.row(align=True)

		
def menu_func_export(self, context):
	self.layout.operator(Exporti0rScene.bl_idname, text="i0rTech Scene (.its)")

def register():
	bpy.utils.register_module(__name__)
	bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
	bpy.utils.unregister_module(__name__)
	bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
	register()
