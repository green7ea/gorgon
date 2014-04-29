import capnp
import gorgon.gorgon_capnp


class Vertex:
    pos = []
    normal = []

    def __init__(self, pos, normal):
        self.pos = pos
        self.normal = normal

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
        color.x = 1
        color.y = 2
        color.z = 3


class Face:
    vertices = None

    def __init__(self, mesh, face):
        self.vertices = []
        for vertex_index in face.vertices:
            pos = mesh.vertices[vertex_index].co
            normal = mesh.vertices[vertex_index].normal
            self.vertices.append(Vertex(pos, normal))

    def to_capnp(self, capnp_face):
        capnp_vertices = capnp_face.init('vertices', len(self.vertices))

        for i in range(len(self.vertices)):
            self.vertices[i].to_capnp(capnp_vertices[i])


class Mesh:
    name = ""
    faces = None

    def __init__(self, mesh, matrix):
        self.name = mesh.name
        self.faces = []

        for face in mesh.tessfaces:
            self.faces.append(Face(mesh, face))

    def to_capnp(self, capnp_mesh):
        capnp_mesh.name = self.name
        capnp_faces = capnp_mesh.init('faces', len(self.faces))

        for i in range(len(self.faces)):
            self.faces[i].to_capnp(capnp_faces[i])

    def to_file(self, filepath):
        try:
            file = open(filepath, 'wb')
            mesh = gorgon.gorgon_capnp.Mesh.new_message()
            self.to_capnp(mesh)
            mesh.write(file)
        finally:
            file.close()
