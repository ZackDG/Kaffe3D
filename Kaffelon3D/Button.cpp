#include "Button.h"

void Button::Render()
{
    IsHover();
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
/// <summary>
/// Creates a button as a quad.
/// </summary>
/// <param name="startPosition">0-100</param>
/// <param name="width">0-100</param>
/// <param name="height">0-100</param>
/// <param name="color">RGBA</param>
Button::Button(std::string text, glm::vec2 startPosition, float width, float height, glm::vec4 color, float screenWidth, float screenHeight, GLFWwindow* win, std::function<void()> function)
{
    ButtonString = text;
    ButtonFunction = function;
    pWindow = win;
    glm::vec2 startPositionCopy = startPosition;
    NormalizedPosition = startPosition;
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
    width = width * 2.0f / 100.0f;
    height = height * 2.0f / 100.0f;

    static const GLfloat g_vertex_buffer_data[] = {
   //bottom left,
   startPosition.x, startPosition.y, 0.0f,
   //bottom right
   startPosition.x + width, startPosition.y, 0.0f,
   //top right
   startPosition.x + width, startPosition.y + height, 0.0f,
   //bottom left
   startPosition.x, startPosition.y, 0.0f,
   //top right
   startPosition.x + width, startPosition.y + height, 0.0f,
   //top left
    startPosition.x, startPosition.y + height, 0.0f
    };

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &VertexBuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    ButtonShader = Shader("shaders/buttonVShader.vertexshader", "shaders/buttonFShader.fragmentshader");

    ColorID = glGetUniformLocation(ButtonShader.ID, "ButtonColor");

    glm::vec2 margin = glm::vec2(3,3);
    
    PixelDimension = glm::vec2((width / 2.0) * ScreenWidth, (height / 2.0f) * ScreenHeight);
    PixelPosition = startPosition;
    PixelPosition.y += PixelDimension.y / 2.0;

    ButtonText = new Text(glm::vec3(0, 1, 0), startPositionCopy+margin, ButtonString, 36, ScreenWidth, ScreenHeight);
}

bool Button::IsHover()
{
    double xpos;
    double ypos;
    glfwGetCursorPos(pWindow, &xpos, &ypos);
    if (xpos > PixelPosition.x && xpos < (double)PixelPosition.x + ((double)PixelDimension.x * 1.0f) &&
        ypos > PixelPosition.y && ypos < (double)PixelPosition.y + ((double)PixelDimension.y * 1.0f))
    {
        // Make Button Blue
        ButtonColor[0] = 1;
        ButtonColor[2] = 0;
        if (glfwGetMouseButton(pWindow, 0) && !Clicked) {
            ButtonFunction();
            Clicked = true;
        }
        if (!glfwGetMouseButton(pWindow, 0) && Clicked)
        {
            Clicked = false;
        }
    }

    else
    {
        // Make Button Green;
        ButtonColor[0] = .7;
        ButtonColor[2] = 0;
        if (!glfwGetMouseButton(pWindow, 0))
        {
            Clicked = false;
        }
    }
    return false;
}
