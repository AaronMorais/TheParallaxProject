#include "obj_viewer.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>

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
    /* no transformation */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* clear color buffer to white */
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(1.0);

    /* color buffer must be cleared each time */
    glClear(GL_COLOR_BUFFER_BIT);

    if (m_dataChanged == true)
    {

        vector<glm::vec4> vertices;
        std::shared_ptr<ObjData> data = m_data;

        auto shapes = data->m_shapes;

        glBegin(GL_POLYGON);

        for (unsigned int i = 0; i < shapes->size(); ++i) {

            assert(((*shapes)[i].mesh.positions.size() % 3) == 0);

            for (unsigned int f = 0; f < (*shapes)[i].mesh.positions.size() / 3; ++f)
            {
                glVertex3f(
                    (*shapes)[i].mesh.positions[3*f+0],
                    (*shapes)[i].mesh.positions[3*f+1],
                    (*shapes)[i].mesh.positions[3*f+2]
                );
            }

        }

        glEnd();

        // m_dataChanged = false;
    }

    /* GLFW is ALWAYS double buffered; will call glFlush() */
    glfwSwapBuffers(m_window);
}

ObjViewer::ObjViewer() :
    m_frameWidth(600),
    m_frameHeight(480),
    m_dataChanged(false)
{

}
