#include "QuadGraphic.h"

void QuadGraphic::Render()
{
    glUseProgram(ButtonShader.ID);
    glUniform4fv(ColorID, 1, ButtonColor);
    glBindVertexArray(VertexArrayID);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
    ButtonText->Render();

}

void QuadGraphic::UpdateMesh(float width, float height, float screenWidth, float screenHeight)
{
    width = width * 2.0f / 100.0f;
    height = height * 2.0f / 100.0f;
    GLfloat g_vertex_buffer_data[] = {
        //bottom left,
        Position.x, Position.y, 0.0f,
        //bottom right
        Position.x + width, Position.y, 0.0f,
        //top right
        Position.x + width, Position.y + height, 0.0f,
        //bottom left
        Position.x, Position.y, 0.0f,
        //top right
        Position.x + width, Position.y + height, 0.0f,
        //top left
         Position.x, Position.y + height, 0.0f
    };
    glBindVertexArray(VertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
   // // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

QuadGraphic::QuadGraphic(std::string text, glm::vec2 startPosition, float width, float height, glm::vec4 color, float screenWidth, float screenHeight, GLFWwindow* win, std::function<void()> function)
{
    ButtonString = text;
    ButtonFunction = function;
    window = win;
    //Position = startPosition;
    RatioPosition = startPosition;
    ScreenHeight = screenHeight;
    ScreenWidth = screenWidth;
    ButtonColor[0] = color.x;
    ButtonColor[1] = color.y;
    ButtonColor[2] = color.z;

    VertexArrayID = 0;
    VertexBuffer = 0;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    startPosition = glm::vec2(((startPosition.x * 2.0f) - 100) / 100.0f, ((startPosition.y * 2.0f) - 100) / 100.0f);
    Position = startPosition;

    width = width * 2.0f / 100.0f;
    height = height * 2.0f / 100.0f;

    GLfloat g_vertex_buffer_data[] = {
        //bottom left,
        startPosition.x, startPosition.y, -1.0f,
        //bottom right
        startPosition.x + width, startPosition.y, -1.0f,
        //top right
        startPosition.x + width, startPosition.y + height, -1.0f,
        //bottom left
        startPosition.x, startPosition.y, -1.0f,
        //top right
        startPosition.x + width, startPosition.y + height, -1.0f,

        //top left
         startPosition.x, startPosition.y + height, -1.0f
    };

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &VertexBuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    ButtonShader = Shader("shaders/buttonVShader.vertexshader", "shaders/buttonFShader.fragmentshader");

    ColorID = glGetUniformLocation(ButtonShader.ID, "ButtonColor");

    RatioPosition = glm::vec2(RatioPosition.x / 100.0f, RatioPosition.y / 100.0f);
    float marginX = (((width / 100.0f) * 5.0f) * ScreenWidth);
    float marginY = (((height / 100.0f) * 20.0f) * ScreenHeight);

    PixelPosition = glm::vec2(ScreenWidth * RatioPosition.x, ScreenHeight * RatioPosition.y);
    PixelDimension = glm::vec2((width / 2.0) * ScreenWidth, (height / 2.0f) * ScreenHeight);


    ButtonText = new Text(glm::vec3(0, 1, 0), glm::vec2(PixelPosition.x + marginX, PixelPosition.y + marginY), ButtonString, 36, ScreenWidth, ScreenHeight);
}
