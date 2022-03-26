#ifndef OPENGL_GUI_MESH
#define OPENGL_GUI_MESH

#include "shader.hpp"
#include "texture.hpp"

namespace opengl_gui
{
    struct Vertex2D
    {
        glm::vec2 position = glm::vec2(0), textureCoords = glm::vec2(0);
        glm::vec4 color = glm::vec4(0);
    };

    class VAO : public GL
    {
    public:
        VAO();
        ~VAO();

        void linkAttrib(unsigned int layout, unsigned int numComponents, GLenum type, GLsizeiptr stride, void *offset);
        void bind();
        void unbind();
    };

    class VBO : public GL
    {
    public:
        VBO(size_t verticesSize, size_t vertexSizeof, const void *verticesData);
        ~VBO();

        void bind();
        void unbind();
    };

    class EBO : public GL
    {
    public:
        EBO(std::vector<unsigned int> &indices);
        ~EBO();

        void bind();
        void unbind();
    };

    class Mesh2D
    {
    public:
        Mesh2D();
        Mesh2D(std::vector<Vertex2D> &vertices, std::vector<unsigned int> &indices);
        Mesh2D(std::vector<Vertex2D> &vertices, std::vector<unsigned int> &indices, std::vector<Texture *> textures);
        virtual ~Mesh2D();

        virtual void applyVertexChange();

        virtual void draw(Shader *shader);

        std::vector<Vertex2D> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture *> textures;

    protected:
        VAO *vao = nullptr;
        VBO *vbo = nullptr;
        EBO *ebo = nullptr;
    };
}

#endif