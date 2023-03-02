#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"

GLfloat point[] = {
      0.0f,  50.f, 0.0f,
      50.f, -50.f, 0.0f,
    - 50.f, -50.f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLfloat texCoord[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

glm::vec2 windowSize(640, 480);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    windowSize.x = width;
    windowSize.y = height;
    glViewport(0, 0, windowSize.x, windowSize.y);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(windowSize.x, windowSize.y, "BattleCity", NULL, NULL);
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL())
    {
        std::cout << "Can't load GLAD!" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(1, 0, 1, 1);

    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.LoadShaders("DefaultShader", "resources\\shaders\\vertex.txt", "resources\\shaders\\fragment.txt");
        if (!pDefaultShaderProgram)
        {
            std::cerr << "Can't create shader program!" << "DefaultShader" << std::endl;
            return -1;
        }

        auto pSpriteShaderProgram = resourceManager.LoadShaders("SpriteShader", "resources\\shaders\\vSprite.txt", "resources\\shaders\\fSprite.txt");
        if (!pSpriteShaderProgram)
        {
            std::cerr << "Can't create shader program!" << "SpriteShader" << std::endl;
            return -1;
        }

        auto tex = resourceManager.LoadTexture("DefaultTexture", "resources\\textures\\map_16x16.png");

        std::vector<std::string> subTexturesNames = { "block", "topBlock", "bottomBlock", "leftBlock", "rightBlock", "topLeftBlock", "topRightBlock", "bottomLeftBlock", "bottomRightBlock", "beton"};
        auto pTextureAtlas = resourceManager.LoadTextureAtlas("DefaultTextureAtlas", "resources\\textures\\map_16x16.png", std::move(subTexturesNames), 16, 16);

        auto pSprite = resourceManager.LoadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "beton");
        pSprite->SetPosition(glm::vec2(300, 100));

        // передача в память видеокарты информации (позиция, цвет) для созданных шейдеров
        GLuint points_vbo = 0;  // vertex buffer object
        glGenBuffers(1, &points_vbo);   // генерация буфера (функция вернёт значение в points_vbo)
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);  // подключение буфера (сделали буфер текущим)
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);   // передача текущему буферу координат (на видеокарте размещение произайдёт в статической пмяти, данные не будут часто менятся)

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW); // перемещает данные из оператвной памяти процессора в память видеокарты

        GLuint texCoord_vbo = 0;
        glGenBuffers(1, &texCoord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

        GLuint vao = 0;     // vertex array object
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);   // включение нулевой позиции в шейдере (содержит положение точки)
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);  // сделать буфер текущим
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDefaultShaderProgram->Use();
        pDefaultShaderProgram->SetInt("tex", 0);

        glm::mat4 modelMatrix_1 = glm::mat4(1.f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(50.f, 50.f, 0.f));

        glm::mat4 modelMatrix_2 = glm::mat4(1.f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 200.f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, windowSize.x, 0.f, windowSize.y, -100.f, 100.f);

        pDefaultShaderProgram->SetMatrix4("projectionMat", projectionMatrix);

        pSpriteShaderProgram->Use();
        pSpriteShaderProgram->SetInt("tex", 0);
        pSpriteShaderProgram->SetMatrix4("projectionMat", projectionMatrix);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);


            pDefaultShaderProgram->Use();
            glBindVertexArray(vao);     // биндим эррей объект который хотим отрисовать, на данный момент он всего 1
            tex->Bind();

            pDefaultShaderProgram->SetMatrix4("modelMat", modelMatrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pDefaultShaderProgram->SetMatrix4("modelMat", modelMatrix_2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pSprite->Render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}