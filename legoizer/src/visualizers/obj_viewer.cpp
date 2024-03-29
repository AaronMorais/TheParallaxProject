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
    std::shared_ptr<plx::LegoData> data
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

    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    // glEnable(GL_BACK);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

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
    glfwSetKeyCallback(m_window, GetInstance().s_HandleKeyboard);      // general keyboard input
    glfwSetCharCallback(m_window, GetInstance().s_HandleCharacter);  // ser specific character handling
    glfwSetScrollCallback(m_window, GetInstance().s_HandleScroll);


    do {
        HandleMotion();
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

    float offset;
    switch (key) {
        case GLFW_KEY_ESCAPE:
            HandleQuit(wd);
            break;

        case GLFW_KEY_EQUAL:
            m_scale = m_scale + 0.02f;
        case GLFW_KEY_MINUS:
            offset = m_scale - 0.01f;
            if (offset > 50.0f)
            {
                offset = 50.0f;
            }
            if (offset < 0.001f)
            {
                offset = 0.001f;
            }
            m_scale = offset;
            break;

        case GLFW_KEY_UP:
        case GLFW_KEY_DOWN:
        case GLFW_KEY_LEFT:
        case GLFW_KEY_RIGHT:
        case GLFW_KEY_0:
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
ObjViewer::HandleMotion()
{
    // static double lastTime = glfwGetTime();
    // double currentTime = glfwGetTime();
    // float deltaTime = float(currentTime - lastTime);

    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    glfwSetCursorPos(m_window, m_frameWidth/2, m_frameHeight/2);

    m_horizontalAngle += m_speed;
    m_direction = glm::normalize(glm::vec3(
        std::cos(m_verticalAngle) * std::sin(m_horizontalAngle),
        std::sin(m_verticalAngle),
        std::cos(m_verticalAngle) * std::cos(m_horizontalAngle)
    ));

    m_right = glm::normalize(glm::vec3(
        std::sin(m_horizontalAngle - (3.14f / 2.0f)),
        0.f,
        std::cos(m_horizontalAngle - (3.14f / 2.0f))
    ));

    m_up = glm::cross(m_right, m_direction);

    if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        // m_position += m_direction * deltaTime * m_speed;
         m_verticalAngle += m_speed;// * float(m_frameHeight/2 - ypos);
    }
    if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        // m_position -= m_direction * deltaTime * m_speed;
         m_verticalAngle -= m_speed;// * float(m_frameHeight/2 - ypos);
    }
    if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
     //   m_position += m_right * deltaTime * m_speed;
        m_horizontalAngle -= m_speed;// * float(m_frameWidth/2 - xpos);

    }
    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
       // m_position -= m_right * deltaTime * m_speed;
        m_horizontalAngle += m_speed;// * float(m_frameWidth/2 - xpos);
    }

    // m_projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, m_up);

    // lastTime = currentTime;
}

void
ObjViewer::HandleScroll(GLFWwindow* wd, double xoffset, double yoffset)
{

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
    glOrtho(0.0, (float)m_frameWidth, 0.0, (float)m_frameHeight, -1.f, 100.f);

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

    glClearColor(0.20f, 0.59f, 0.85f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 100.f, -100.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        m_position.x, m_position.y, m_position.z,
        0.f, 0.f, 0.f,
        0.f, 1.f, 0.f);
    glScalef(m_scale, m_scale, m_scale);


    const std::vector<glm::vec3>& faces = m_data->faces();
    const std::vector<glm::vec3>& vertices = m_data->vertices();

    glPushMatrix();
    glRotatef(m_horizontalAngle, 0, 1, 0);
    glRotatef(m_verticalAngle, 1, 0, 0);

    for (size_t f = 0; f < faces.size(); ++f)
    {
        glBegin(GL_TRIANGLES);

        glColor3f((float)f/faces.size(), (float)f/faces.size(), (float)f/faces.size());
        glVertex3f(
            vertices[faces[f].x].x,
            vertices[faces[f].x].y,
            vertices[faces[f].x].z
        );

        glColor3f((float)f/faces.size(), (float)f/faces.size(), (float)f/faces.size());
        glVertex3f(
            vertices[faces[f].y].x,
            vertices[faces[f].y].y,
            vertices[faces[f].y].z
        );

        glColor3f((float)f/faces.size(), (float)f/faces.size(), (float)f/faces.size());
        glVertex3f(
            vertices[faces[f].z].x,
            vertices[faces[f].z].y,
            vertices[faces[f].z].z
        );

        glEnd();
    }

    glPopMatrix();

    glfwSwapBuffers(m_window);
}

ObjViewer::ObjViewer() :
    m_scale(1.0f),
    m_frameWidth(1920),
    m_frameHeight(1080),
    m_dataChanged(false)
{
    m_position = glm::vec3(0, 0, 5);

    m_horizontalAngle = 3.14f;
    m_verticalAngle = 0.0f;
    m_speed = 0.2f;
    m_mouseSpeed = 0.005f;
}
