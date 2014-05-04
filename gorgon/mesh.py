import capnp
import gorgon.gorgon_capnp
import bpy

class Vertex:
    pos = None
    normal = None
    color = None
    texcoord = None

    def __init__(self, pos, normal, color, texcoord):
        self.pos = pos
        self.normal = normal
        self.color = color
        self.texcoord = texcoord

    def to_capnp(self, capnp_vertex):
        pos = capnp_vertex.position
        pos.x = self.pos[0]
        pos.y = self.pos[1]
        pos.z = self.pos[2]

        normal = capnp_vertex.normal
        normal.x = self.normal[0]
        normal.y = self.normal[1]
        normal.z = self.normal[2]

        color = capnp_vertex.color
        color.x = self.color[0]
        color.y = self.color[1]
        color.z = self.color[2]

        texcoord = capnp_vertex.texcoord
        texcoord.u = self.texcoord[0]
        texcoord.v = self.texcoord[1]


class Face:
    vertices = None

    def __init__(self, mesh, face):
        self.vertices = []
        for loop_index in face.loop_indices:
            vertex_index = mesh.loops[loop_index].vertex_index
            pos = mesh.vertices[vertex_index].co
            normal = mesh.vertices[vertex_index].normal
            color = [1, 2, 3]

            texcoord = [0, 0]
            if mesh.uv_layers.active:
                uv_layer = mesh.uv_layers.active.data
                texcoord = uv_layer[loop_index].uv

            self.vertices.append(Vertex(pos, normal, color, texcoord))

    def to_capnp(self, capnp_face):
        capnp_vertices = capnp_face.init('vertices', len(self.vertices))

        for i in range(len(self.vertices)):
            self.vertices[i].to_capnp(capnp_vertices[i])


class Mesh:
    name = ""
    textures = None
    faces = None

    def __init__(self, mesh, matrix):
        self.name = mesh.name
        self.faces = []
        self.textures = []

        for face in mesh.polygons:
            self.faces.append(Face(mesh, face))

        for image in bpy.data.images:
            file = '.' + image.filepath[1:]
            self.textures.append(file)

    def to_capnp(self, capnp_mesh):
        capnp_mesh.name = self.name

        capnp_faces = capnp_mesh.init('faces', len(self.faces))
        for i in range(len(self.faces)):
            self.faces[i].to_capnp(capnp_faces[i])

        capnp_textures = capnp_mesh.init('textures', len(self.textures))
        for i in range(len(self.textures)):
            capnp_textures[i] = self.textures[i]

    def to_file(self, filepath):
        try:
            file = open(filepath, 'wb')
            mesh = gorgon.gorgon_capnp.Mesh.new_message()
            self.to_capnp(mesh)
            mesh.write(file)
        finally:
            file.close()
