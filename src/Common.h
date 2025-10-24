#ifndef __COMMON_H__
#define __COMMON_H__

// #include <memory>
// #include <string>
// #include <optional>
// #include <glad/glad.h>
// #include <glfw/glfw3.h>
// #include <spdlog/spdlog.h>

// 1) Windows 헤더를 먼저
#if defined(_WIN32)
  #ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
  #endif
  #ifndef NOMINMAX
  #define NOMINMAX
  #endif
  #include <windows.h>   // => APIENTRY가 여기서 먼저 정의됨
#endif

// 2) GLFW가 OpenGL 헤더를 끼워넣지 않도록
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

// 3) 그다음 GL/GLFW/기타
#include <memory>
#include <string>
#include <optional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CLASS_PTR(klassName) \
class klassName; \
using klassName ## UPtr = std::unique_ptr<klassName>; \
using klassName ## ShPtr = std::shared_ptr<klassName>; \
using klassName ## WPtr = std::weak_ptr<klassName>;

std::optional<std::string> LoadTextFileOrEmpty(const std::string& filename);
glm::vec3 GetAttCoeff(float distance);
#endif // __COMMON_H__