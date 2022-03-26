#include "../../../header/opengl_gui/mesh.hpp"

opengl_gui::Mesh2D::Mesh2D() {}
opengl_gui::Mesh2D::Mesh2D(std::vector<Vertex2D> &vertices, std::vector<unsigned int> &indices)
{
    this->vertices = vertices;
    this->indices = indices;

    applyVertexChange();
}
opengl_gui::Mesh2D::Mesh2D(std::vector<Vertex2D> &vertices, std::vector<unsigned int> &indices, std::vector<Texture *> textures)
{
    this->vertices = vertices;
    this->indices = indices;

    for (int i = 0; i < textures.size(); i++)
        this->textures.push_back(textures[i]);

    applyVertexChange();
}
opengl_gui::Mesh2D::~Mesh2D()
{
    delete vao;
    delete vbo;
    delete ebo;
}

void opengl_gui::Mesh2D::applyVertexChange()
{
    std::vector<Vertex2D> vertOut = vertices;
    for (Vertex2D &v : vertOut)
    {
        v.position = {v.position.x * 2 - 1, v.position.y * -2 + 1};
        v.textureCoords.y *= -1;
    }

    if (vao)
    {
        vao->bind();
        delete vbo;
        vbo = new VBO(vertOut.size(), sizeof(Vertex2D), vertOut.data());
        delete ebo;
        ebo = new EBO(indices);
    }
    else
    {
        vao = new VAO();
        vao->bind();
        vbo = new VBO(vertOut.size(), sizeof(Vertex2D), vertOut.data());
        ebo = new EBO(indices);
    }
    vbo->bind();

    vao->linkAttrib(0, 2, GL_FLOAT, sizeof(Vertex2D), (void *)0);
    vao->linkAttrib(1, 2, GL_FLOAT, sizeof(Vertex2D), (void *)(2 * sizeof(float)));
    vao->linkAttrib(2, 4, GL_FLOAT, sizeof(Vertex2D), (void *)(4 * sizeof(float)));

    vao->unbind();
    vbo->unbind();
    ebo->unbind();
}

void opengl_gui::Mesh2D::draw(Shader *shader)
{
    vao->bind();

    glUniform1i(glGetUniformLocation(shader->getID(), "useTexture"), textures.size() != 0);
    for (int i = 0; i < textures.size(); i++)
    {
        textures[i]->activate(shader, "texture" + std::to_string(i));
        textures[i]->bind();
    }

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    vao->unbind();
}