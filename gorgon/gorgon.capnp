@0xed5e19d1afa74555;

struct Vector {
  x @0: Float32;
  y @1: Float32;
  z @2: Float32;
}

struct TexCoord {
  u @0: Float32;
  v @1: Float32;
}

struct Vertex {
  position @0: Vector;
  normal @1: Vector;
  color @2: Vector;
  texcoord @3: TexCoord;
}

struct Face {
  vertices @0: List(Vertex);
}

struct Mesh {
  name @0: Text;
  textures @1: List(Text);
  faces @2: List(Face);
}
