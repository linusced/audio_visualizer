#include "../../../header/opengl_gui/gui_element.hpp"

void opengl_gui::Element::containerMeshBorderRadius(const int &borderRadius, const int &borderWidth, const glm::vec4 &bgColor, const glm::vec4 &borderColor, const Layout &layout)
{
    unsigned int cornerResolution = (borderRadius - 15.0f) / (100 - 15.0f) * (10 - 3) + 3,
                 numVertices = 12 + cornerResolution * 4,
                 numIndices = 12 * 1.5 + (cornerResolution * 3 + 3) * 4;

    containerMesh.vertices.resize(numVertices);
    containerMesh.indices.resize(numIndices);

    if (borderWidth > 0)
    {
        numVertices = 16 + (cornerResolution * 2) * 4;
        numIndices = 16 * 1.5 + (cornerResolution * 6 + 6) * 4;

        borderMesh.vertices.resize(numVertices);
        borderMesh.indices.resize(numIndices);
    }

    unsigned int vertexIndex = 0, indicesIndex = 0;

    containerMesh.vertices[vertexIndex].position = {layout.x + borderRadius, layout.y};
    containerMesh.vertices[vertexIndex + 1].position = {layout.x + layout.width - borderRadius, layout.y};
    containerMesh.vertices[vertexIndex + 2].position = {layout.x + layout.width - borderRadius, layout.y + layout.height};
    containerMesh.vertices[vertexIndex + 3].position = {layout.x + borderRadius, layout.y + layout.height};

    containerMesh.indices[indicesIndex++] = vertexIndex;
    containerMesh.indices[indicesIndex++] = vertexIndex + 1;
    containerMesh.indices[indicesIndex++] = vertexIndex + 2;
    containerMesh.indices[indicesIndex++] = vertexIndex;
    containerMesh.indices[indicesIndex++] = vertexIndex + 2;
    containerMesh.indices[indicesIndex++] = vertexIndex + 3;

    vertexIndex += 4;

    containerMesh.vertices[vertexIndex].position = {layout.x + layout.width - borderRadius, layout.y + borderRadius};
    containerMesh.vertices[vertexIndex + 1].position = {layout.x + layout.width, layout.y + borderRadius};
    containerMesh.vertices[vertexIndex + 2].position = {layout.x + layout.width, layout.y + layout.height - borderRadius};
    containerMesh.vertices[vertexIndex + 3].position = {layout.x + layout.width - borderRadius, layout.y + layout.height - borderRadius};

    containerMesh.indices[indicesIndex++] = vertexIndex;
    containerMesh.indices[indicesIndex++] = vertexIndex + 1;
    containerMesh.indices[indicesIndex++] = vertexIndex + 2;
    containerMesh.indices[indicesIndex++] = vertexIndex;
    containerMesh.indices[indicesIndex++] = vertexIndex + 2;
    containerMesh.indices[indicesIndex++] = vertexIndex + 3;

    vertexIndex += 4;

    containerMesh.vertices[vertexIndex].position = {layout.x, layout.y + borderRadius};
    containerMesh.vertices[vertexIndex + 1].position = {layout.x + borderRadius, layout.y + borderRadius};
    containerMesh.vertices[vertexIndex + 2].position = {layout.x + borderRadius, layout.y + layout.height - borderRadius};
    containerMesh.vertices[vertexIndex + 3].position = {layout.x, layout.y + layout.height - borderRadius};

    containerMesh.indices[indicesIndex++] = vertexIndex;
    containerMesh.indices[indicesIndex++] = vertexIndex + 1;
    containerMesh.indices[indicesIndex++] = vertexIndex + 2;
    containerMesh.indices[indicesIndex++] = vertexIndex;
    containerMesh.indices[indicesIndex++] = vertexIndex + 2;
    containerMesh.indices[indicesIndex++] = vertexIndex + 3;

    vertexIndex += 4;

    unsigned int cornerStartIndex, cornerCenterIndex, cornerEndIndex;
    float angle;

    for (unsigned int corner = 1; corner <= 4; corner++)
    {
        if (corner == 1)
        {
            cornerStartIndex = 1;
            cornerCenterIndex = 4;
            cornerEndIndex = 5;
        }
        else if (corner == 2)
        {
            cornerStartIndex = 6;
            cornerCenterIndex = 7;
            cornerEndIndex = 2;
        }
        else if (corner == 3)
        {
            cornerStartIndex = 3;
            cornerCenterIndex = 10;
            cornerEndIndex = 11;
        }
        else
        {
            cornerStartIndex = 8;
            cornerCenterIndex = 9;
            cornerEndIndex = 0;
        }

        for (unsigned int i = 1; i < cornerResolution + 1; i++)
        {
            angle = i / (float)(cornerResolution + 1) * (M_PI / 2.0f);

            if (corner == 1)
                containerMesh.vertices[vertexIndex].position = {(layout.x + layout.width - borderRadius) + borderRadius * sin(angle), (layout.y + borderRadius) - borderRadius * cos(angle)};
            else if (corner == 2)
                containerMesh.vertices[vertexIndex].position = {(layout.x + layout.width - borderRadius) + borderRadius * sin((M_PI / 2.0f) - angle), (layout.y + layout.height - borderRadius) + borderRadius * cos((M_PI / 2.0f) - angle)};
            else if (corner == 3)
                containerMesh.vertices[vertexIndex].position = {(layout.x + borderRadius) - borderRadius * sin(angle), (layout.y + layout.height - borderRadius) + borderRadius * cos(angle)};
            else
                containerMesh.vertices[vertexIndex].position = {(layout.x + borderRadius) - borderRadius * sin((M_PI / 2.0f) - angle), (layout.y + borderRadius) - borderRadius * cos((M_PI / 2.0f) - angle)};

            if (i == cornerResolution)
            {
                containerMesh.indices[indicesIndex++] = vertexIndex - 1;
                containerMesh.indices[indicesIndex++] = vertexIndex;
                containerMesh.indices[indicesIndex++] = cornerCenterIndex;

                containerMesh.indices[indicesIndex++] = vertexIndex;
                containerMesh.indices[indicesIndex++] = cornerEndIndex;
                containerMesh.indices[indicesIndex++] = cornerCenterIndex;
            }
            else if (i == 1)
            {
                containerMesh.indices[indicesIndex++] = cornerStartIndex;
                containerMesh.indices[indicesIndex++] = vertexIndex;
                containerMesh.indices[indicesIndex++] = cornerCenterIndex;
            }
            else
            {
                containerMesh.indices[indicesIndex++] = vertexIndex - 1;
                containerMesh.indices[indicesIndex++] = vertexIndex;
                containerMesh.indices[indicesIndex++] = cornerCenterIndex;
            }

            vertexIndex++;
        }
    }

    for (unsigned int i = 0; i < containerMesh.vertices.size(); i++)
        containerMesh.vertices[i].color = bgColor;

    if (borderWidth > 0)
    {
        vertexIndex = indicesIndex = 0;

        borderMesh.vertices[vertexIndex].position = {layout.x + borderRadius, layout.y - borderWidth};
        borderMesh.vertices[vertexIndex + 1].position = {layout.x + layout.width - borderRadius, layout.y - borderWidth};
        borderMesh.vertices[vertexIndex + 2].position = {layout.x + layout.width - borderRadius, layout.y};
        borderMesh.vertices[vertexIndex + 3].position = {layout.x + borderRadius, layout.y};

        borderMesh.indices[indicesIndex++] = vertexIndex;
        borderMesh.indices[indicesIndex++] = vertexIndex + 1;
        borderMesh.indices[indicesIndex++] = vertexIndex + 2;
        borderMesh.indices[indicesIndex++] = vertexIndex;
        borderMesh.indices[indicesIndex++] = vertexIndex + 2;
        borderMesh.indices[indicesIndex++] = vertexIndex + 3;

        vertexIndex += 4;

        borderMesh.vertices[vertexIndex].position = {layout.x + borderRadius, layout.y + layout.height};
        borderMesh.vertices[vertexIndex + 1].position = {layout.x + layout.width - borderRadius, layout.y + layout.height};
        borderMesh.vertices[vertexIndex + 2].position = {layout.x + layout.width - borderRadius, layout.y + layout.height + borderWidth};
        borderMesh.vertices[vertexIndex + 3].position = {layout.x + borderRadius, layout.y + layout.height + borderWidth};

        borderMesh.indices[indicesIndex++] = vertexIndex;
        borderMesh.indices[indicesIndex++] = vertexIndex + 1;
        borderMesh.indices[indicesIndex++] = vertexIndex + 2;
        borderMesh.indices[indicesIndex++] = vertexIndex;
        borderMesh.indices[indicesIndex++] = vertexIndex + 2;
        borderMesh.indices[indicesIndex++] = vertexIndex + 3;

        vertexIndex += 4;

        borderMesh.vertices[vertexIndex].position = {layout.x - borderWidth, layout.y + borderRadius};
        borderMesh.vertices[vertexIndex + 1].position = {layout.x, layout.y + borderRadius};
        borderMesh.vertices[vertexIndex + 2].position = {layout.x, layout.y + layout.height - borderRadius};
        borderMesh.vertices[vertexIndex + 3].position = {layout.x - borderWidth, layout.y + layout.height - borderRadius};

        borderMesh.indices[indicesIndex++] = vertexIndex;
        borderMesh.indices[indicesIndex++] = vertexIndex + 1;
        borderMesh.indices[indicesIndex++] = vertexIndex + 2;
        borderMesh.indices[indicesIndex++] = vertexIndex;
        borderMesh.indices[indicesIndex++] = vertexIndex + 2;
        borderMesh.indices[indicesIndex++] = vertexIndex + 3;

        vertexIndex += 4;

        borderMesh.vertices[vertexIndex].position = {layout.x + layout.width, layout.y + borderRadius};
        borderMesh.vertices[vertexIndex + 1].position = {layout.x + layout.width + borderWidth, layout.y + borderRadius};
        borderMesh.vertices[vertexIndex + 2].position = {layout.x + layout.width + borderWidth, layout.y + layout.height - borderRadius};
        borderMesh.vertices[vertexIndex + 3].position = {layout.x + layout.width, layout.y + layout.height - borderRadius};

        borderMesh.indices[indicesIndex++] = vertexIndex;
        borderMesh.indices[indicesIndex++] = vertexIndex + 1;
        borderMesh.indices[indicesIndex++] = vertexIndex + 2;
        borderMesh.indices[indicesIndex++] = vertexIndex;
        borderMesh.indices[indicesIndex++] = vertexIndex + 2;
        borderMesh.indices[indicesIndex++] = vertexIndex + 3;

        vertexIndex += 4;

        unsigned int borderSquareIndex1, borderSquareIndex2,
            borderEndSquareIndex1, borderEndSquareIndex2;

        for (unsigned int corner = 1; corner <= 4; corner++)
        {
            if (corner == 1)
            {
                borderSquareIndex1 = 1;
                borderSquareIndex2 = 2;
                borderEndSquareIndex1 = 12;
                borderEndSquareIndex2 = 13;
            }
            else if (corner == 2)
            {
                borderSquareIndex1 = 14;
                borderSquareIndex2 = 15;
                borderEndSquareIndex1 = 5;
                borderEndSquareIndex2 = 6;
            }
            else if (corner == 3)
            {
                borderSquareIndex1 = 7;
                borderSquareIndex2 = 4;
                borderEndSquareIndex1 = 10;
                borderEndSquareIndex2 = 11;
            }
            else
            {
                borderSquareIndex1 = 8;
                borderSquareIndex2 = 9;
                borderEndSquareIndex1 = 3;
                borderEndSquareIndex2 = 0;
            }

            for (unsigned int i = 1; i < cornerResolution + 1; i++)
            {
                angle = i / (float)(cornerResolution + 1) * (M_PI / 2.0f);

                if (corner == 1)
                {
                    borderMesh.vertices[vertexIndex].position = {(layout.x + layout.width - borderRadius) + borderRadius * sin(angle), (layout.y + borderRadius) - borderRadius * cos(angle)};
                    borderMesh.vertices[vertexIndex + 1].position = {(layout.x + layout.width - borderRadius) + (borderRadius + borderWidth) * sin(angle), (layout.y + borderRadius) - (borderRadius + borderWidth) * cos(angle)};
                }
                else if (corner == 2)
                {
                    borderMesh.vertices[vertexIndex].position = {(layout.x + layout.width - borderRadius) + borderRadius * sin((M_PI / 2.0f) - angle), (layout.y + layout.height - borderRadius) + borderRadius * cos((M_PI / 2.0f) - angle)};
                    borderMesh.vertices[vertexIndex + 1].position = {(layout.x + layout.width - borderRadius) + (borderRadius + borderWidth) * sin((M_PI / 2.0f) - angle), (layout.y + layout.height - borderRadius) + (borderRadius + borderWidth) * cos((M_PI / 2.0f) - angle)};
                }
                else if (corner == 3)
                {
                    borderMesh.vertices[vertexIndex].position = {(layout.x + borderRadius) - borderRadius * sin(angle), (layout.y + layout.height - borderRadius) + borderRadius * cos(angle)};
                    borderMesh.vertices[vertexIndex + 1].position = {(layout.x + borderRadius) - (borderRadius + borderWidth) * sin(angle), (layout.y + layout.height - borderRadius) + (borderRadius + borderWidth) * cos(angle)};
                }
                else
                {
                    borderMesh.vertices[vertexIndex].position = {(layout.x + borderRadius) - borderRadius * sin((M_PI / 2.0f) - angle), (layout.y + borderRadius) - borderRadius * cos((M_PI / 2.0f) - angle)};
                    borderMesh.vertices[vertexIndex + 1].position = {(layout.x + borderRadius) - (borderRadius + borderWidth) * sin((M_PI / 2.0f) - angle), (layout.y + borderRadius) - (borderRadius + borderWidth) * cos((M_PI / 2.0f) - angle)};
                }

                borderMesh.indices[indicesIndex++] = borderSquareIndex1;
                borderMesh.indices[indicesIndex++] = vertexIndex;
                borderMesh.indices[indicesIndex++] = borderSquareIndex2;

                borderMesh.indices[indicesIndex++] = borderSquareIndex1;
                borderMesh.indices[indicesIndex++] = vertexIndex;
                borderMesh.indices[indicesIndex++] = vertexIndex + 1;

                if (i == cornerResolution)
                {
                    borderMesh.indices[indicesIndex++] = vertexIndex;
                    borderMesh.indices[indicesIndex++] = borderEndSquareIndex1;
                    borderMesh.indices[indicesIndex++] = vertexIndex + 1;

                    borderMesh.indices[indicesIndex++] = vertexIndex + 1;
                    borderMesh.indices[indicesIndex++] = borderEndSquareIndex1;
                    borderMesh.indices[indicesIndex++] = borderEndSquareIndex2;
                }

                borderSquareIndex1 = vertexIndex + 1;
                borderSquareIndex2 = vertexIndex;
                vertexIndex += 2;
            }
        }

        for (unsigned int i = 0; i < vertexIndex; i++)
            borderMesh.vertices[i].color = borderColor;
    }
}