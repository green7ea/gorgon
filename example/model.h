#pragma once

// capnp includes to load the model
#include <capnp/message.h>
#include <capnp/serialize.h>
#include "gorgon.capnp.h"

#include <string>
#include <vector>

#include <GL/gl.h>

#include "texture.h"

class Model
{

private:

    struct VertexData
    {
        float pos[3];
        float normal[3];
        float color[3];
        float uv[2];
    };

public:
    Model(int fd)
    {
        ::capnp::StreamFdMessageReader message(fd);

        Mesh::Reader mesh = message.getRoot<Mesh>();

        name = mesh.getName().cStr();

        const char *texture_name = mesh.getTextures()[0].cStr();
        if (!texture.load(texture_name))
        {
            fprintf(stderr, "Couldn't load '%s'\n",
                    texture_name);
            exit(1);
        }

        for (const Face::Reader &face: mesh.getFaces())
        {
            std::vector<VertexData> vertex_data;

            for (const Vertex::Reader &vertex: face.getVertices())
            {
                Vector::Reader position = vertex.getPosition();
                Vector::Reader normal = vertex.getNormal();
                Vector::Reader color = vertex.getColor();
                TexCoord::Reader uv = vertex.getTexcoord();

                VertexData data;
                data.pos[0] = position.getX();
                data.pos[1] = position.getY();
                data.pos[2] = position.getZ();

                data.normal[0] = normal.getX();
                data.normal[1] = normal.getY();
                data.normal[2] = normal.getZ();

                data.color[0] = color.getX();
                data.color[1] = color.getY();
                data.color[2] = color.getZ();

                data.uv[0] = uv.getU();
                data.uv[1] = uv.getV();

                vertex_data.push_back(data);
            }

            faces.push_back(vertex_data);
        }
    }

    void draw()
    {
        static float time = 0.f;
        glRotatef(time, 1.f, 3.f, 5.f);
        time += 0.5f;

        glBindTexture(GL_TEXTURE_2D, texture.texture);

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
                glTexCoord2fv(vertex.uv);
                glNormal3fv(vertex.normal);
                glVertex3fv(vertex.pos);
            }

            glEnd();
        }
    }

private:
    std::string name;
    std::vector< std::vector<VertexData> > faces;
    Texture texture;
};
