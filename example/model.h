#pragma once

// capnp includes to load the model
#include <capnp/message.h>
#include <capnp/serialize.h>
#include "gorgon.capnp.h"

#include <string>
#include <vector>

#include <GL/gl.h>

class Model
{

private:
    struct VertexData
    {
        float pos[3];
        float normal[3];
    };

public:
    Model(int fd)
    {
        ::capnp::StreamFdMessageReader message(fd);

        Mesh::Reader mesh = message.getRoot<Mesh>();

        name = mesh.getName().cStr();

        for (const Face::Reader &face: mesh.getFaces())
        {
            std::vector<VertexData> vertex_data;

            for (const Vertex::Reader &vertex: face.getVertices())
            {
                Vector::Reader position = vertex.getPosition();
                Vector::Reader normal = vertex.getNormal();

                VertexData data;
                data.pos[0] = position.getX();
                data.pos[1] = position.getY();
                data.pos[2] = position.getZ();

                data.normal[0] = normal.getX();
                data.normal[1] = normal.getY();
                data.normal[2] = normal.getZ();

                vertex_data.push_back(data);
            }

            faces.push_back(vertex_data);
        }
    }

    void draw()
    {
        for (const auto &face: faces)
        {
            if (face.size() == 4)
            {
                glBegin(GL_QUADS);
            }
            else
            {
                glBegin(GL_TRIANGLES);
            }

            for (const auto &vertex: face)
            {
                glNormal3fv(vertex.normal);
                glVertex3fv(vertex.pos);
            }

            glEnd();
        }
    }

private:
    std::string name;
    std::vector< std::vector<VertexData> > faces;
};
