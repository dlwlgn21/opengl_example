#include "Context.h"
#include "Image.h"

using namespace std;

unique_ptr<Context> Context::CreateOrNull()
{
    unique_ptr<Context> context = unique_ptr<Context>(new Context());
    if (!context->TryInit())
    {
        return nullptr;
    }
    return move(context);
}
void Context::ProcessInput(GLFWwindow* pWindow) {
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        mCamPos += cameraSpeed * mCamFront;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        mCamPos -= cameraSpeed * mCamFront;
    }

    glm::vec3 camRight = glm::normalize(glm::cross(mCamUp, -mCamFront));
    if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        mCamPos += cameraSpeed * camRight;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        mCamPos -= cameraSpeed * camRight;    
    }

    glm::vec3 camUp = glm::normalize(glm::cross(-mCamFront, camRight));
    if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS)
    {
        mCamPos += cameraSpeed * camUp;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS)
    {
        mCamPos -= cameraSpeed * camUp;
    }
}


void Context::OnMouseMove(double x, double y)
{
    static glm::vec2 prevPos = glm::vec2(static_cast<float>(x), static_cast<float>(y));
    glm::vec2 pos = glm::vec2(static_cast<float>(x), static_cast<float>(y));
    glm::vec2 deltaPos = pos - prevPos;

    const float CAM_ROT_SPEED = 0.2f;
    mCamYaw -= deltaPos.x * CAM_ROT_SPEED;
    mCamPitch -= deltaPos.y * CAM_ROT_SPEED;

    if (mCamYaw < 0.0f)
    {
        mCamYaw += 360.0f;
    }
    if (mCamYaw > 360.0f)
    {
        mCamYaw -= 360.0f;
    }
    mCamPitch = glm::clamp<float>(mCamPitch, -89.0f, 89.0f);
    // if (mCamPitch > 89.0f)
    // {
    //     mCamPitch = 89.0f;
    // }
    // if (mCamPitch > -89.0f)
    // {
    //     mCamPitch = -89.0f;
    // }
    prevPos = pos;
}
void Context::Reshape(int width, int height)
{
    mWidth = width;
    mHeight = height;
    glViewport(0, 0, mWidth, mHeight);
}
bool Context::TryInit()
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    };

    uint32_t indices[] = {
        0,  2,  1,  2,  0,  3,
        4,  5,  6,  6,  7,  4,
        8,  9, 10, 10, 11,  8,
        12, 14, 13, 14, 12, 15,
        16, 17, 18, 18, 19, 16,
        20, 22, 21, 22, 20, 23,
    };

    mVertexLayout = VertexLayout::Create();

    mVertexBuffer = Buffer::CreateWithDataOrNull(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 24 * 5);
    mVertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    mVertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);

    mIndexBuffer = Buffer::CreateWithDataOrNull(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 36);

    unique_ptr<Shader> vs = Shader::CreateFromFileOrNull("./Shader/Texture.vs", GL_VERTEX_SHADER);
    unique_ptr<Shader> fs = Shader::CreateFromFileOrNull("./Shader/Texture.fs", GL_FRAGMENT_SHADER);
    if (vs == nullptr || fs == nullptr)
    {
        return false;
    }
    SPDLOG_INFO("VertexShader Id {}", vs->GetId());
    SPDLOG_INFO("FragmentShader Id {}", fs->GetId());
    mProgram = Program::CreateOrNull({fs.get(), vs.get()});
    if (mProgram == nullptr)
    {
        return false;
    }
    SPDLOG_INFO("Program Id : {}", mProgram->GetId());
    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);

    unique_ptr<Image> img2 = Image::LoadOrNull("./Image/awesomeface.png");
    if (img2 == nullptr)
    {
        return false;
    }
    
    unique_ptr<Image> img = Image::LoadOrNull("./Image/container.jpg");
    if (img == nullptr)
    {
        return false;
    }
    mTexture = Texture::CreateFromImg(img.get());
    mTexture2 = Texture::CreateFromImg(img2.get());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture->GetId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTexture2->GetId());

    mProgram->Use();
    mProgram->SetUniform("texSampler1", 0);
    mProgram->SetUniform("texSampler1", 1);

    glEnable(GL_DEPTH_TEST);
    return true;
}

void Context::Render()
{
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glm::highp_mat4 worldMat = glm::rotate(
    //         glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f)),
    //         glm::radians(static_cast<float>(glfwGetTime()) * 120.0f), 
    //         glm::vec3(1.0f, 0.0f, 0.0f)
    // );
    // glm::highp_mat4 viewMat = glm::translate(
    //     glm::mat4(1.0f),
    //     glm::vec3(0.0f, 0.0f, -3.0f)
    // );
    // glm::highp_mat4 projectionMat = glm::perspective(
    //     glm::radians(45.0f),
    //     static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT,
    //     0.01f,
    //     10.0f
    // );
    // glm::highp_mat4 transform = projectionMat * viewMat * worldMat;
    // mProgram->SetUniform("transform", transform);



    // mProgram->Use();
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto projection = glm::perspective(glm::radians(45.0f),
        static_cast<float>(mWidth) / mHeight, 0.01f, 100.0f);


    mCamFront = glm::rotate(glm::mat4(1.0f),
                    glm::radians(mCamYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f),
                    glm::radians(mCamPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::vec4(0.0f, 0.0f, -1.0f, 0.0f); // 벡터기 때문에 마지막에 0 집어넣음, 평행이동이 안됨

    glm::highp_mat4 view = glm::lookAt(
        mCamPos, 
        mCamPos + mCamFront, 
        mCamUp
    );

    for (size_t i = 0; i < cubePositions.size(); i++){
        auto& pos = cubePositions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model,
            glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i),
            glm::vec3(1.0f, 0.5f, 0.0f));
        auto transform = projection * view * model;
        mProgram->SetUniform("transform", transform);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}
