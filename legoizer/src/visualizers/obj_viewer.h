#ifndef _OBJ_VIEWER_H__
#define _OBJ_VIEWER_H__

#include <GL/glew.h>

#define GLFW_DLL

#include "obj_data.h"
#include "lego_data.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class ObjViewer {
public:
    void SetData(std::shared_ptr<plx::LegoData> data);
    void Run();
    static ObjViewer& GetInstance();

private:
    static void s_HandleKeyboard(GLFWwindow* wd, int key, int scancode, int action, int mods);
    static void s_HandleCharacter(GLFWwindow* wd, unsigned int key);
    static void s_HandleScroll(GLFWwindow* wd, double xoffset, double yoffset);
    static void s_HandleError(int errcode, const char* desc);
    static void s_HandleQuit(GLFWwindow* wd);
    static void s_HandleFrameBufferReshape(GLFWwindow* wd, int w, int h);
    static void s_HandleWindowReshape(GLFWwindow* wd, int w, int h);

    void HandleKeyboard(GLFWwindow* wd, int key, int scancode, int action, int mods);
    void HandleCharacter(GLFWwindow* wd, unsigned int key);
    void HandleScroll(GLFWwindow* wd, double xoffset, double yoffset);
    void HandleError(int errcode, const char* desc);
    void HandleQuit(GLFWwindow* wd);
    void HandleFrameBufferReshape(GLFWwindow* wd, int w, int h);
    void HandleWindowReshape(GLFWwindow* wd, int w, int h);

    void HandleMotion();
    void Render();

    float m_scale;
    int m_frameWidth;
    int m_frameHeight;
    GLFWwindow* m_window;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_right;
    glm::vec3 m_up;

    float m_horizontalAngle;
    float m_verticalAngle;
    float m_speed;
    float m_mouseSpeed;


    volatile bool m_dataChanged;
    std::shared_ptr<plx::LegoData> m_data;

    ObjViewer();
    ObjViewer(ObjViewer const&) = delete;
    void operator=(ObjViewer const&) = delete;
};

#endif
