#include "object2D.h"

#include <vector>
#include <math.h>


#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2D::CreateDiamond(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float diagonalLength,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    float halfDiagonal = diagonalLength / 2.0f;

    std::vector<VertexFormat> vertices =
    {
         VertexFormat(corner + glm::vec3(halfDiagonal, 0, 0), color),
        VertexFormat(corner + glm::vec3(diagonalLength, halfDiagonal, 0), color),
        VertexFormat(corner + glm::vec3(halfDiagonal, diagonalLength, 0), color),
        VertexFormat(corner + glm::vec3(0, halfDiagonal, 0), color)
    };

    Mesh* diamond = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        diamond->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    diamond->InitFromData(vertices, indices);
    return diamond;
}

Mesh* object2D::CreateStar(const std::string& name,
    glm::vec3 center,
    float outerRadius,
    float innerRadius,
    int points,
    glm::vec3 color, bool fill)
{
    Mesh* star = new Mesh(name);
    star->SetDrawMode(GL_TRIANGLE_FAN);
    std::vector<unsigned int> indices;
    std::vector<VertexFormat> vertices;

    // Initial angle
    float startAngle = M_PI / 2.0f;


    vertices.push_back(VertexFormat(center, color));
    indices.push_back(0);

    for (int i = 0; i <= 4 * points; i++) {
        float angle = startAngle +  i * M_PI / points;
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;

        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);

        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
        indices.push_back(i / 2 + 1);
    }

    star->InitFromData(vertices, indices);
    return star;
}

Mesh* object2D::CreateHexagons(const std::string& name,
    glm::vec3 center,
    float length,
    glm::vec3 largeHexagonColor,
    bool fill)
{
    Mesh* hexagon = new Mesh(name);
    hexagon->SetDrawMode(GL_TRIANGLE_FAN);

    // Initialize vectors to store vertex data (positions and colors) and indices
    std::vector<unsigned int> indices;
    std::vector<VertexFormat> vertices;

    for (int i = 0; i < 6; ++i) {
        float angle_rad = glm::radians(60.0f * i);
        float x = center.x + length * cos(angle_rad);
        float y = center.y + length * sin(angle_rad);
        vertices.push_back(VertexFormat{ glm::vec3(x, y, 0), largeHexagonColor });
        indices.push_back(i);
    }

    // Create hexagon using rotated triangles
    indices.push_back(0);
    indices.push_back(6);


    hexagon->InitFromData(vertices, indices);

    return hexagon;

}








