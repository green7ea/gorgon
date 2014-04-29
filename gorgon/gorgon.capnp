@0xed5e19d1afa74555;

struct Vector {
    x @0: Float32;
    y @1: Float32;
    z @2: Float32;
}

struct Vertex {
    position @0: Vector;
    normal @1: Vector;
    color @2: Vector;
}

struct Face {
    vertices @0: List(Vertex);
}

struct Mesh {
    name @0: Text;
    faces @1: List(Face);
}
