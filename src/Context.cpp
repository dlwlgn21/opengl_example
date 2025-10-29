#include "Context.h"
#include "Image.h"
#include <imgui.h>
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
void Context::ProcessInput(GLFWwindow* pWindow)
{
    if (!mIsCamControl)
    {
        return;
    }
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
    if (!mIsCamControl)
    {
        return;
    }
    glm::vec2 pos = glm::vec2(static_cast<float>(x), static_cast<float>(y));
    glm::vec2 deltaPos = pos - mPrevMousePos;

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
    mPrevMousePos = pos;
}

void Context::OnMouseButton(int btn, int action, double x, double y)
{
    if (btn == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            mPrevMousePos = glm::vec2(static_cast<float>(x), static_cast<float>(y));
            mIsCamControl = true;
        }
        else if (action == GLFW_RELEASE)
        {
            mIsCamControl = false;
        }
    }
    
}

void Context::Reshape(int width, int height)
{
    mWidth = width;
    mHeight = height;
    glViewport(0, 0, mWidth, mHeight);
    mFramebuffer = Framebuffer::CreateOrNull(
        Texture::CreateEmptyTexture(width, height, GL_RGBA)
    );
}
bool Context::TryInit()
{
    mBoxMesh = Mesh::MakeBoxOrNull();
    if (mBoxMesh == nullptr)
    {
        return false;
    }
    mPlaneMesh = Mesh::MakePlaneOrNull();
    if (mPlaneMesh == nullptr)
    {
        return false;
    }
    // mModel = Model::LoadOrNull("./Model/backpack.obj");
    // if (mModel == nullptr)
    // {
    //     return false;
    // }
    unique_ptr<Shader> vs = Shader::CreateFromFileOrNull("./Shader/Lighting.vs", GL_VERTEX_SHADER);
    unique_ptr<Shader> fs = Shader::CreateFromFileOrNull("./Shader/Lighting.fs", GL_FRAGMENT_SHADER);
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
    SPDLOG_INFO("Lighthing Program Id : {}", mProgram->GetId());

    unique_ptr<Shader> sVs = Shader::CreateFromFileOrNull("./Shader/Simple.vs", GL_VERTEX_SHADER);
    unique_ptr<Shader> sFs = Shader::CreateFromFileOrNull("./Shader/Simple.fs", GL_FRAGMENT_SHADER);
    if (sVs == nullptr || sFs == nullptr)
    {
        return false;
    }
    SPDLOG_INFO("Simple VertexShader Id {}", sVs->GetId());
    SPDLOG_INFO("Simple FragmentShader Id {}", sFs->GetId());
    mSimpleProgram = Program::CreateOrNull({sVs.get(), sFs.get()});
    if (mSimpleProgram == nullptr)
    {
        return false;
    }
    SPDLOG_INFO("Simple Program Id : {}", mSimpleProgram->GetId());

    unique_ptr<Shader> textureVs = Shader::CreateFromFileOrNull("./Shader/Texture.vs", GL_VERTEX_SHADER);
    unique_ptr<Shader> textureFs = Shader::CreateFromFileOrNull("./Shader/Texture.fs", GL_FRAGMENT_SHADER);
    if (textureVs == nullptr || textureFs == nullptr)
    {
        return false;
    }
    SPDLOG_INFO("Texture VertexShader Id {}", textureVs->GetId());
    SPDLOG_INFO("Texture FragmentShader Id {}", textureFs->GetId());
    mTextureProgram = Program::CreateOrNull({textureVs.get(), textureFs.get()});
    if (mTextureProgram == nullptr)
    {
        return false;
    }
    SPDLOG_INFO("Texture Program Id : {}", mTextureProgram->GetId());
    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);

    mBagMaterial = Material::Create();
    mBagMaterial->Diffuse = Texture::CreateFromImg(Image::CreateSingleColorImageOrNull(4, 4, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)).get());
    mBagMaterial->Specular = Texture::CreateFromImg(Image::CreateSingleColorImageOrNull(4, 4, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)).get());
    
    mWindowTexture = Texture::CreateFromImg(
        Image::LoadOrNull("./image/blending_transparent_window.png").get()
    );

    unique_ptr<Texture> darkGrayTexture = Texture::CreateFromImg(
        Image::CreateSingleColorImageOrNull(4, 4, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)).get()
    );

    unique_ptr<Texture> grayTexture = Texture::CreateFromImg(
        Image::CreateSingleColorImageOrNull(4, 4, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)).get()
    );
            
    mPlaneMaterial = Material::Create();
    mPlaneMaterial->Diffuse = Texture::CreateFromImg(
        Image::LoadOrNull("./image/marble.jpg").get()
    );
    mPlaneMaterial->Specular = move(grayTexture);
    mPlaneMaterial->Shininess = 128.0f;

    mBox1Material = Material::Create();
    mBox1Material->Diffuse = Texture::CreateFromImg(
        Image::LoadOrNull("./image/container.jpg").get()
    );
    mBox1Material->Specular = move(darkGrayTexture);
    mBox1Material->Shininess = 16.0f;

    mBox2Material = Material::Create();
    mBox2Material->Diffuse = Texture::CreateFromImg(
        Image::LoadOrNull("./image/container2.png").get()
    );
    mBox2Material->Specular = Texture::CreateFromImg(
        Image::LoadOrNull("./image/container2_specular.png").get()
    );
    mBox2Material->Shininess = 64.0f;
    SPDLOG_INFO("PlaneMaterial Diffuse Texture Id {}", mPlaneMaterial->Diffuse->GetId());
    return true;
}

void Context::Render()
{
    if (ImGui::Begin("ui window")) 
    {
        if (ImGui::ColorEdit4("clear color", glm::value_ptr(mClearColor))) 
        {
            glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
        }
        ImGui::Separator();
        ImGui::DragFloat3("camera pos", glm::value_ptr(mCamPos), 0.01f);
        ImGui::DragFloat("camera yaw", &mCamYaw, 0.5f);
        ImGui::DragFloat("camera pitch", &mCamPitch, 0.5f, -89.0f, 89.0f);
        ImGui::Separator();
        if (ImGui::Button("reset camera"))
        {
            mCamYaw = 0.0f;
            mCamPitch = 0.0f;
            mCamPos = glm::vec3(0.0f, 0.0f, 3.0f);
        }

        if (ImGui::CollapsingHeader("light", ImGuiTreeNodeFlags_DefaultOpen)) 
        {
            //ImGui::DragFloat3("l.Direction", glm::value_ptr(mLight.Direction), 0.01f);
            ImGui::DragFloat3("l.Pos", glm::value_ptr(mLight.Pos), 0.01f);
            ImGui::DragFloat3("l.Dir", glm::value_ptr(mLight.Dir), 0.01f);
            ImGui::DragFloat2("l.Cutoff", glm::value_ptr(mLight.Cutoff), 0.5f, 0.0f, 90.0f);
            ImGui::DragFloat("l.Dist", &mLight.Dist, 0.5f, 0.0f, 3000.0f);
            ImGui::ColorEdit3("l.ambient", glm::value_ptr(mLight.Ambient));
            ImGui::ColorEdit3("l.diffuse", glm::value_ptr(mLight.Diffuse));
            ImGui::ColorEdit3("l.specular", glm::value_ptr(mLight.Specular));
            ImGui::Checkbox("Is FlashLightMode", &mIsFlashLight);

        }
        
        // if (ImGui::CollapsingHeader("material", ImGuiTreeNodeFlags_DefaultOpen)) 
        // {
        //     ImGui::DragFloat("m.shininess", &mBagMaterial->Shininess, 1.0f, 1.0f, 256.0f);
        // }
        ImGui::Checkbox("animation", &mIsAnimation);
    }
    ImGui::End();

    mFramebuffer->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glm::highp_mat4 projection = glm::perspective(glm::radians(45.0f),
        static_cast<float>(mWidth) / mHeight, 0.1f, 20.0f);

    mCamFront = glm::rotate(glm::mat4(1.0f), glm::radians(mCamYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(mCamPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::vec4(0.0f, 0.0f, -1.0f, 0.0f); // 벡터기 때문에 마지막에 0 집어넣음, 평행이동이 안됨


    glm::highp_mat4 view = glm::lookAt(
        mCamPos, 
        mCamPos + mCamFront, 
        mCamUp
    );


    // mProgram->Use();
    // mProgram->SetUniform("viewWorldPos", mCamPos);
    // mProgram->SetUniform("light.Pos", mLight.Pos);
    // mProgram->SetUniform("light.Dir", mLight.Dir);
	// mProgram->SetUniform("light.Cutoff", glm::vec2(
    //     cosf(glm::radians(mLight.Cutoff[0])),
    //     cosf(glm::radians(mLight.Cutoff[0] + mLight.Cutoff[1]))));
    // mProgram->SetUniform("light.Att", GetAttCoeff(mLight.Dist));
    // mProgram->SetUniform("light.Ambient", mLight.Ambient);
    // mProgram->SetUniform("light.Diffuse", mLight.Diffuse);
    // mProgram->SetUniform("light.Specular", mLight.Specular);
    // mProgram->SetUniform("material.Diffuse", 0);
    // mProgram->SetUniform("material.Specular", 1);
    // mProgram->SetUniform("material.Shininess", mMaterial.Shininess);

    // glActiveTexture(GL_TEXTURE0);
    // mMaterial.Diffuse->Bind();

    // glActiveTexture(GL_TEXTURE1);
    // mMaterial.Specular->Bind();
    
    // for (size_t i = 0; i < cubePositions.size(); i++)
    // {
    //     glm::vec3& pos = cubePositions[i];
    //     glm::highp_mat4 model = glm::translate(glm::mat4(1.0f), pos);
    //     float angle = glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i);
    //     model = glm::rotate(model,
    //         mIsAnimation ? angle : 0.0f,
    //         glm::vec3(1.0f, 0.5f, 0.0f));
    //     glm::highp_mat4 transform = projection * view * model;
    //     mProgram->SetUniform("transform", transform);
    //     mProgram->SetUniform("modelTransform", model);
    //     mBoxMesh->Draw();
    // }
    glm::vec3 lightPos = mIsFlashLight ? mCamPos : mLight.Pos;
    glm::vec3 lightDir = mIsFlashLight ? mCamFront : mLight.Dir;
    if (!mIsFlashLight)
    {
        glm::highp_mat4 lightModelTransform = 
            glm::translate(glm::mat4(1.0f), mLight.Pos) *
            glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        mSimpleProgram->Use();
        mSimpleProgram->SetUniform("color", glm::vec4(mLight.Ambient + mLight.Diffuse, 1.0f));
        mSimpleProgram->SetUniform("transform", projection * view * lightModelTransform);
        mBoxMesh->Draw(mSimpleProgram.get());
    }
    mProgram->Use();
    mProgram->SetUniform("viewWorldPos", mCamPos);
    mProgram->SetUniform("light.Pos", lightPos);
    mProgram->SetUniform("light.Dir", lightDir);
    mProgram->SetUniform("light.Cutoff", glm::vec2(cosf(glm::radians(mLight.Cutoff[0])), cosf(glm::radians(mLight.Cutoff[0] + mLight.Cutoff[1]))));
    mProgram->SetUniform("light.Att", GetAttCoeff(mLight.Dist));
    mProgram->SetUniform("light.Ambient", mLight.Ambient);
    mProgram->SetUniform("light.Diffuse", mLight.Diffuse);
    mProgram->SetUniform("light.Specular", mLight.Specular);

    // mProgram->SetUniform("material.Diffuse", 0);
    // mProgram->SetUniform("material.Specular", 1);
    // mProgram->SetUniform("material.Shininess", mBagMaterial->Shininess);

    // glActiveTexture(GL_TEXTURE0);
    // mMaterial.Diffuse->Bind();
    // glActiveTexture(GL_TEXTURE1);
    // mMaterial.Specular->Bind();

    // auto modelTransform = glm::mat4(1.0f);
    // auto transform = projection * view * modelTransform;
    // mProgram->SetUniform("transform", transform);
    // mProgram->SetUniform("modelTransform", modelTransform);
    //mModel->Draw(mProgram.get());


    mProgram->Use();
    auto modelTransform =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
    auto transform = projection * view * modelTransform;
    mProgram->SetUniform("transform", transform);
    mProgram->SetUniform("modelTransform", modelTransform);
    mPlaneMaterial->SetToProgram(mProgram.get());
    mBoxMesh->Draw(mProgram.get());

    modelTransform =
        glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.75f, -4.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
    transform = projection * view * modelTransform;
    mProgram->SetUniform("transform", transform);
    mProgram->SetUniform("modelTransform", modelTransform);
    mBox1Material->SetToProgram(mProgram.get());
    mBoxMesh->Draw(mProgram.get());

    modelTransform =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.75f, 2.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
    transform = projection * view * modelTransform;
    mProgram->SetUniform("transform", transform);
    mProgram->SetUniform("modelTransform", modelTransform);
    mBox2Material->SetToProgram(mProgram.get());
    mBoxMesh->Draw(mProgram.get());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mTextureProgram->Use();
    mWindowTexture->Bind();
    mTextureProgram->SetUniform("texSampler", 0);

    modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 4.0f));
    transform = projection * view * modelTransform;
    mTextureProgram->SetUniform("transform", transform);
    mPlaneMesh->Draw(mTextureProgram.get());

    modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, 0.5f, 5.0f));
    transform = projection * view * modelTransform;
    mTextureProgram->SetUniform("transform", transform);
    mPlaneMesh->Draw(mTextureProgram.get());

    modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.4f, 0.5f, 6.0f));
    transform = projection * view * modelTransform;
    mTextureProgram->SetUniform("transform", transform);
    mPlaneMesh->Draw(mTextureProgram.get());



    Framebuffer::BindToDefault();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    mTextureProgram->Use();
    mTextureProgram->SetUniform("transform",
        glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f)));
    mFramebuffer->GetColorAttachment()->Bind();
    mTextureProgram->SetUniform("texSampler", 0);
    mPlaneMesh->Draw(mTextureProgram.get());    
}
