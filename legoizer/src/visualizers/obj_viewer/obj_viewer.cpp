#include "obj_viewer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <math.h>

ObjViewer&
ObjViewer::GetInstance()
{
    static ObjViewer instance;
    return instance;
}

void
ObjViewer::SetData(
    std::shared_ptr<ObjData> data
)
{
    m_data = data;
    m_dataChanged = true;
}

void
ObjViewer::Run()
{
    glfwSetErrorCallback(ObjViewer::GetInstance().s_HandleError);
    if (!glfwInit()) {
        exit(1);
    }

    /* create the window and its associated OpenGL context */
    m_window = glfwCreateWindow(m_frameWidth, m_frameHeight, "GLFW Window", NULL, NULL);
    if (!m_window) {
        glfwTerminate();
        exit(1);
    }

    /* make the window's context the current context */
    glfwMakeContextCurrent(m_window);

    /* With Retina display on Mac OS X, GLFW's FramebufferSize is different from WindowSize */
    glfwGetFramebufferSize(m_window, &m_frameWidth, &m_frameHeight);


    /* shape view port */
    GetInstance().s_HandleFrameBufferReshape(m_window, m_frameWidth, m_frameHeight);

    /* --- register callbacks with GLFW --- */
    /* register function to handle window resizes */
    /* With Retina display on Mac OS X GLFW's FramebufferSize is different from WindowSize */
    glfwSetFramebufferSizeCallback(m_window, GetInstance().s_HandleFrameBufferReshape);
    glfwSetWindowSizeCallback(m_window, GetInstance().s_HandleWindowReshape);

    /* register function to handle window close */
    glfwSetWindowCloseCallback(m_window, GetInstance().s_HandleQuit);

    /* register function to handle keyboard input */
    glfwSetKeyCallback(m_window, ObjViewer::GetInstance().s_HandleKeyboard);      // general keyboard input
    glfwSetCharCallback(m_window, ObjViewer::GetInstance().s_HandleCharacter);  // ser specific character handling
    glfwSetScrollCallback(m_window, ObjViewer::GetInstance().s_HandleScroll);

    do {
        Render();
        glfwWaitEvents();
    } while (!glfwWindowShouldClose(m_window));
}

void
ObjViewer::s_HandleKeyboard(GLFWwindow* wd, int key, int scancode, int action, int mods)
{
    GetInstance().HandleKeyboard(wd, key, scancode, action, mods);
}

void
ObjViewer::s_HandleCharacter(GLFWwindow* wd, unsigned int key)
{
    GetInstance().HandleCharacter(wd, key);
}

void
ObjViewer::s_HandleScroll(GLFWwindow* wd, double xoffset, double yoffset)
{
    GetInstance().HandleScroll(wd, xoffset, yoffset);
}

void
ObjViewer::s_HandleError(int errcode, const char* desc)
{
    GetInstance().HandleError(errcode, desc);
}

void
ObjViewer::s_HandleQuit(GLFWwindow* wd)
{
    GetInstance().HandleQuit(wd);
}

/* Called when framebuffer is resized, e.g., when window is resized
 * OR when the same size buffer is moved across Retina and non-Retina displays
 * when running Mac OS X.
 * NOT called automatically when window is first created.
 * Called by GLFW BEFORE reshape().
*/
void
ObjViewer::s_HandleFrameBufferReshape(GLFWwindow* wd, int w, int h)
{
    GetInstance().HandleFrameBufferReshape(wd, w, h);
}

/* Called when window is resized.
 * NOT called automatically when window is first created.
 * Called by GLFW AFTER FrameBufferReshape().
 */
void
ObjViewer::s_HandleWindowReshape(GLFWwindow* wd, int w, int h)
{
    GetInstance().HandleWindowReshape(wd, w, h);
}

void
ObjViewer::HandleKeyboard(GLFWwindow* wd, int key, int scancode, int action, int mods)
{
    if (action == GLFW_RELEASE) { // function is called first on GLFW_PRESS.
        return;
    }

    switch (key) {
        case GLFW_KEY_ESCAPE:
            HandleQuit(wd);
            break;
        default:
            break;
    }
}

void
ObjViewer::HandleCharacter(GLFWwindow* wd, unsigned int key)
{
    switch (key) {
        case 'q':
            HandleQuit(wd);
            break;
        default:
           break;
    }
}

void
ObjViewer::HandleScroll(GLFWwindow* wd, double xoffset, double yoffset)
{
    std::cout << yoffset << std::endl;

    float offset = m_scale + (double)yoffset * 0.01f;
    if (offset > 50.0f)
    {
        offset = 50.0f;
    }
    if (offset < 0.001f)
    {
        offset = 0.001f;
    }
    m_scale = offset;
}

void
ObjViewer::HandleError(int errcode, const char* desc)
{
    fprintf(stderr, "%d: %s\n", errcode, desc);
}

void
ObjViewer::HandleQuit(GLFWwindow* wd)
{
    glfwDestroyWindow(wd);
    glfwTerminate();
    exit(0);
}

void
ObjViewer::HandleFrameBufferReshape(GLFWwindow* wd, int w, int h)
{
    /* save new framebuffer dimensions */
    m_frameWidth = w;
    m_frameHeight = h;

    /* do an orthographic parallel projection with the view volume
    set to first quadrant, fixed to the initial window dimension */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (float)m_frameWidth, 0.0, (float)m_frameHeight, -1.f, 1.f);

    /* Tell OpenGL to use the whole window for drawing.
    Note that we don't resize the view volume, so
    the viewport will show the whole view volume
    shrunk/stretched to fit the current view port. */
    glViewport(0, 0, (GLsizei) m_frameWidth, (GLsizei) m_frameHeight);
}

void
ObjViewer::HandleWindowReshape(GLFWwindow* wd, int w, int h)
{
    /* save new screen dimensions */
    m_frameWidth = w;
    m_frameHeight = h;
}

void
ObjViewer::Render()
{
    float ratio = m_frameWidth / (float) m_frameHeight;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

    glScalef(m_scale, m_scale, m_scale);

    auto shapes = m_data->m_shapes;

    for (size_t i = 0; i < shapes->size(); ++i)
    {
        tinyobj::mesh_t mesh = (*shapes)[i].mesh;
        for (size_t f = 0; f < mesh.faces.size(); ++f)
        {
            glBegin(GL_TRIANGLES);

            glColor3f(1.f, 1.f, 1.f);
            glVertex3f(
                mesh.vertices[mesh.faces[f].v1].x,
                mesh.vertices[mesh.faces[f].v1].y,
                mesh.vertices[mesh.faces[f].v1].z
            );

            glColor3f(1.f, 1.f, 1.f);
            glVertex3f(
                mesh.vertices[mesh.faces[f].v2].x,
                mesh.vertices[mesh.faces[f].v2].y,
                mesh.vertices[mesh.faces[f].v2].z
            );

            glColor3f(1.f, 1.f, 1.f);
            glVertex3f(
                mesh.vertices[mesh.faces[f].v3].x,
                mesh.vertices[mesh.faces[f].v3].y,
                mesh.vertices[mesh.faces[f].v3].z
            );

            glEnd();
        }

    }

    glfwSwapBuffers(m_window);
}

ObjViewer::ObjViewer() :
    m_scale(1.0f),
    m_frameWidth(600),
    m_frameHeight(480),
    m_dataChanged(false)
{

}
