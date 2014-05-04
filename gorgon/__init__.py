#!BPY

bl_info = {
    'name': 'Gorgon Export',
    'author': 'green7ea',
    'version': (0, 1, 1),
    'blender': (2, 7, 0),
    'location': 'File > Import-Export',
    'description': 'Export a model into the gorgon format.',
    'warning': '',
    'wiki_url': 'http://',
    'tracker_url': 'http://',
    'category': 'Import-Export'}

import bpy
import gorgon.mesh
from bpy.props import StringProperty, BoolProperty
from bpy_extras.io_utils import ExportHelper, ImportHelper


class export_gorgon(bpy.types.Operator, ExportHelper):
    bl_idname = 'export.gorgon'
    bl_label = 'Export Gorgon'
    filename_ext = '.grg'
    filter_glob = StringProperty(
        default='model.grg',
        options={'HIDDEN'})

    def execute(self, context):
        props = self.properties
        filepath = bpy.path.ensure_ext(self.filepath, self.filename_ext)

        if len(bpy.context.selected_objects) != 1:
            raise NameError('Please select exactly one object.')

        scene = bpy.context.scene
        object = bpy.context.selected_objects[0]
        mesh = None
        mesh_tmp = False

        if object.name.startswith('~'):
            raise NameError('Object starts with ~.')

        if object.type != 'MESH':
            try:
                mesh = object.to_mesh(scene, True, 'PREVIEW')
                mesh_tmp = True
            except:
                mesh = None
        else:
            mesh = object.data

        mesh_obj = gorgon.mesh.Mesh(mesh, object.matrix_world.copy())
        mesh_obj.to_file(self.filepath)

        if mesh_tmp:
            bpy.data.meshes.remove(mesh)

        return {'FINISHED'}

    def invoke(self, context, event):
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}


def menu_export_gorgon(self, context):
    self.layout.operator(export_gorgon.bl_idname, text = 'Gorgon (.grg)')


def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_export_gorgon)


def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_export_gorgon)


if __name__ == '__main__':
    register()
