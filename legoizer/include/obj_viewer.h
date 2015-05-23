#ifndef _OBJ_VIEWER_H__
#define _OBJ_VIEWER_H__


#define GLFW_DLL
#include "tiny_obj_loader.h"
#include "obj_data.h"

#include <GLFW/glfw3.h>

class ObjViewer {
public:
    void SetData(std::shared_ptr<ObjData> data);
    void Run();
    static ObjViewer& GetInstance();

private:
    static void s_HandleKeyboard(GLFWwindow* wd, int key, int scancode, int action, int mods);
    static void s_HandleCharacter(GLFWwindow* wd, unsigned int key);
    static void s_HandleError(int errcode, const char* desc);
    static void s_HandleQuit(GLFWwindow* wd);
    static void s_HandleFrameBufferReshape(GLFWwindow* wd, int w, int h);
    static void s_HandleWindowReshape(GLFWwindow* wd, int w, int h);

    void HandleKeyboard(GLFWwindow* wd, int key, int scancode, int action, int mods);
    void HandleCharacter(GLFWwindow* wd, unsigned int key);
    void HandleError(int errcode, const char* desc);
    void HandleQuit(GLFWwindow* wd);
    void HandleFrameBufferReshape(GLFWwindow* wd, int w, int h);
    void HandleWindowReshape(GLFWwindow* wd, int w, int h);

    void Render();

    int m_frameWidth;
    int m_frameHeight;
    GLFWwindow* m_window;

    volatile bool m_dataChanged;
    std::shared_ptr<ObjData> m_data;

    ObjViewer();
    ObjViewer(ObjViewer const&) = delete;
    void operator=(ObjViewer const&) = delete;
};

#endif
