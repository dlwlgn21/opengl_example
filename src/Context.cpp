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

void Context::InitPrograms()
{
    mDefaultLightingProgram = Program::CreateOrNull("./Shader/Lighting.vs", "./Shader/Lighting.fs");
    if (mDefaultLightingProgram == nullptr)
    {
        SPDLOG_INFO("Failed to create DefaultLightingProgram");
    }
    mSimpleColorProgram = Program::CreateOrNull("./Shader/Simple.vs", "./Shader/Simple.fs");
    if (mSimpleColorProgram == nullptr)
    {
        SPDLOG_INFO("Failed to create SimpleColorProgram");
    }
    mTextureProgram = Program::CreateOrNull("./Shader/Texture.vs","./Shader/Texture.fs");
    if (mTextureProgram == nullptr)
    {
        SPDLOG_INFO("Failed to create SimpleTextureProgram");
    }

    mPostProgram = Program::CreateOrNull("./Shader/Texture.vs", "./Shader/Gamma.fs");
    if (mPostProgram == nullptr)
    {
        SPDLOG_INFO("Failed to create PostProgram");
    }
    mSkyboxProgram = Program::CreateOrNull("./Shader/Skybox.vs", "./Shader/Skybox.fs");
    if (mSkyboxProgram == nullptr)
    {
        SPDLOG_INFO("Failed to create SkyBoxProgram");
    }

    mEnvmapProgram = Program::CreateOrNull("./shader/Envmap.vs", "./shader/Envmap.fs");
    if (mEnvmapProgram == nullptr)
    {
        SPDLOG_INFO("Failed to create EnvmapProgram");
    }

}
void Context::InitMeshes()
{
    mBoxMesh = Mesh::MakeBoxOrNull();
    if (mBoxMesh == nullptr)
    {
        SPDLOG_INFO("Failed to create Box mesh");
    }
    mPlaneMesh = Mesh::MakePlaneOrNull();
    if (mPlaneMesh == nullptr)
    {
        SPDLOG_INFO("Failed to create Plane mesh");
    }
}
void Context::InitMaterials()
{
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


    mBagMaterial = Material::Create();
    mBagMaterial->Diffuse = Texture::CreateFromImg(Image::CreateSingleColorImageOrNull(4, 4, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)).get());
    mBagMaterial->Specular = Texture::CreateFromImg(Image::CreateSingleColorImageOrNull(4, 4, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)).get());
    
}
void Context::InitTextures()
{
    unique_ptr<Image> cubeRight = Image::LoadOrNull("./image/skybox/right.jpg", false);
    unique_ptr<Image> cubeLeft = Image::LoadOrNull("./image/skybox/left.jpg", false);
    unique_ptr<Image> cubeTop = Image::LoadOrNull("./image/skybox/top.jpg", false);
    unique_ptr<Image> cubeBottom = Image::LoadOrNull("./image/skybox/bottom.jpg", false);
    unique_ptr<Image> cubeFront = Image::LoadOrNull("./image/skybox/front.jpg", false);
    unique_ptr<Image> cubeBack = Image::LoadOrNull("./image/skybox/back.jpg", false);
    mCubeTexture = CubeTexture::CreateFromImages({
        cubeRight.get(),
        cubeLeft.get(),
        cubeTop.get(),
        cubeBottom.get(),
        cubeFront.get(),
        cubeBack.get(),
    });
    mWindowTexture = Texture::CreateFromImg(
        Image::LoadOrNull("./image/blending_transparent_window.png").get()
    );

}
void Context::InitModels()
{
    mBagModel = Model::LoadOrNull("./Model/backpack.obj");
}

bool Context::TryInit()
{
    InitPrograms();
    InitMeshes();
    InitMaterials();
    InitTextures();
    InitModels();
    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);
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
        ImGui::DragFloat("gamma", &mGamma, 0.01f, 0.0f, 2.0f);

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
        static_cast<float>(mWidth) / mHeight, 0.1f, 100.0f);

    mCamFront = glm::rotate(glm::mat4(1.0f), glm::radians(mCamYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(mCamPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::vec4(0.0f, 0.0f, -1.0f, 0.0f); // 벡터기 때문에 마지막에 0 집어넣음, 평행이동이 안됨


    glm::highp_mat4 view = glm::lookAt(
        mCamPos, 
        mCamPos + mCamFront, 
        mCamUp
    );

    glm::highp_mat4 skyboxModelTransform =
        glm::translate(glm::mat4(1.0), mCamPos) *
        glm::scale(glm::mat4(1.0), glm::vec3(50.0f));
    mSkyboxProgram->Use();
    mCubeTexture->Bind();
    mSkyboxProgram->SetUniform("skybox", 0);
    mSkyboxProgram->SetUniform("transform", projection * view * skyboxModelTransform);
    mBoxMesh->Draw(mSkyboxProgram.get());




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

        mSimpleColorProgram->Use();
        mSimpleColorProgram->SetUniform("color", glm::vec4(mLight.Ambient + mLight.Diffuse, 1.0f));
        mSimpleColorProgram->SetUniform("transform", projection * view * lightModelTransform);
        mBoxMesh->Draw(mSimpleColorProgram.get());
    }
    mDefaultLightingProgram->Use();
    mDefaultLightingProgram->SetUniform("viewWorldPos", mCamPos);
    mDefaultLightingProgram->SetUniform("light.Pos", lightPos);
    mDefaultLightingProgram->SetUniform("light.Dir", lightDir);
    mDefaultLightingProgram->SetUniform("light.Cutoff", glm::vec2(cosf(glm::radians(mLight.Cutoff[0])), cosf(glm::radians(mLight.Cutoff[0] + mLight.Cutoff[1]))));
    mDefaultLightingProgram->SetUniform("light.Att", GetAttCoeff(mLight.Dist));
    mDefaultLightingProgram->SetUniform("light.Ambient", mLight.Ambient);
    mDefaultLightingProgram->SetUniform("light.Diffuse", mLight.Diffuse);
    mDefaultLightingProgram->SetUniform("light.Specular", mLight.Specular);

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


    mDefaultLightingProgram->Use();
    auto modelTransform =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
    auto transform = projection * view * modelTransform;
    mDefaultLightingProgram->SetUniform("transform", transform);
    mDefaultLightingProgram->SetUniform("modelTransform", modelTransform);
    mPlaneMaterial->SetToProgram(mDefaultLightingProgram.get());
    mBoxMesh->Draw(mDefaultLightingProgram.get());

    modelTransform =
        glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.75f, -4.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
    transform = projection * view * modelTransform;
    mDefaultLightingProgram->SetUniform("transform", transform);
    mDefaultLightingProgram->SetUniform("modelTransform", modelTransform);
    mBox1Material->SetToProgram(mDefaultLightingProgram.get());
    mBoxMesh->Draw(mDefaultLightingProgram.get());

    modelTransform =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.75f, 2.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
    transform = projection * view * modelTransform;
    mDefaultLightingProgram->SetUniform("transform", transform);
    mDefaultLightingProgram->SetUniform("modelTransform", modelTransform);
    mBox2Material->SetToProgram(mDefaultLightingProgram.get());
    mBoxMesh->Draw(mDefaultLightingProgram.get());

    modelTransform =
        glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.75f, -2.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
    mEnvmapProgram->Use();
    mEnvmapProgram->SetUniform("model", modelTransform);
    mEnvmapProgram->SetUniform("view", view);
    mEnvmapProgram->SetUniform("projection", projection);
    mEnvmapProgram->SetUniform("cameraPos", mCamPos);
    mCubeTexture->Bind();
    mEnvmapProgram->SetUniform("skybox", 0);
    mBoxMesh->Draw(mEnvmapProgram.get());




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
    mPostProgram->Use();
    mPostProgram->SetUniform("transform",
        glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f)));
    mPostProgram->SetUniform("gamma", mGamma);
    mFramebuffer->GetColorAttachment()->Bind();
    mPlaneMesh->Draw(mPostProgram.get());

    // mTextureProgram->Use();
    // mTextureProgram->SetUniform("transform",
    //     glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f)));
    // mFramebuffer->GetColorAttachment()->Bind();
    // mTextureProgram->SetUniform("texSampler", 0);
    // mPlaneMesh->Draw(mTextureProgram.get());
}
