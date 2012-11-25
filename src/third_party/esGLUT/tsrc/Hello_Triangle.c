//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// Hello_Triangle.c
//
//    This is a simple example that draws a single triangle with
//    a minimal vertex/fragment shader.  The purpose of this
//    example is to demonstrate the basic concepts of
//    OpenGL ES 2.0 rendering.
#include <esGLUT/esGLUT.h>
#include <stdio.h>
#include <stdlib.h>

// Handle to a program object
static GLuint _programObject = 0;
static GLuint _vertexPosObject = 0;

///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader ( type );

    if ( shader == 0 )
        return 0;

    // Load the shader source
    glShaderSource ( shader, 1, &shaderSrc, NULL );

    // Compile the shader
    glCompileShader ( shader );

    // Check the compile status
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

    if ( !compiled )
    {
        GLint infoLen = 0;

        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

        if ( infoLen > 1 )
        {
            char* infoLog = (char*)malloc (sizeof(char) * infoLen );

            glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
            glutLogMessage("Error compiling shader:\n%s\n", infoLog);

            free ( infoLog );
        }

        glDeleteShader ( shader );
        return 0;
    }

    return shader;

}

int test_init()
{
    char vShaderStr[] =
        "attribute vec4 vPosition;    \n"
        "void main()                  \n"
        "{                            \n"
        "   gl_Position = vPosition;  \n"
        "}                            \n";

    char fShaderStr[] =
        "precision mediump float;\n"\
        "void main()                                  \n"
        "{                                            \n"
        "  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
        "}                                            \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;

    // Load the vertex/fragment shaders
    vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
    fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

    // Create the program object
    programObject = glCreateProgram ( );

    if ( programObject == 0 )
        return 0;

    glAttachShader ( programObject, vertexShader );
    glAttachShader ( programObject, fragmentShader );

    // Bind vPosition to attribute 0
    glBindAttribLocation ( programObject, 0, "vPosition" );

    // Link the program
    glLinkProgram ( programObject );

    // Check the link status
    glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

    if ( !linked )
    {
        GLint infoLen = 0;

        glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

        if ( infoLen > 1 )
        {
            char* infoLog = (char*)malloc (sizeof(char) * infoLen );

            glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
            glutLogMessage("Error linking program:\n%s\n", infoLog);

            free ( infoLog );
        }

        glDeleteProgram ( programObject );
        return 0;
    }

    // Store the program object
    _programObject = programObject;

    // No clientside arrays, so do this in a webgl-friendly manner
    {
        GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f,
                                 -0.5f, -0.5f, 0.0f,
                                 0.5f, -0.5f, 0.0f };
        glGenBuffers(1, &_vertexPosObject);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexPosObject);
        glBufferData(GL_ARRAY_BUFFER, 9*4, vVertices, GL_STATIC_DRAW);
    }

    return 1;
}

static void test_draw(void) {
    glClearColor(0.0, 1.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the program object
    glUseProgram ( _programObject );

    // Load the vertex data
    glBindBuffer(GL_ARRAY_BUFFER, _vertexPosObject);
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray ( 0 );

    glDrawArrays ( GL_TRIANGLES, 0, 3 );

    glutSwapBuffers();
}

static void test_idle(void) {
}

static void test_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitWindowSize(512,512);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    if (!glutCreateWindow("testgl1")) {
        glutLogMessage("E/ Can't create window !");
        return -1;
    }

    glutIdleFunc(test_idle);
    glutReshapeFunc(test_reshape);
    glutDisplayFunc(test_draw);

    if (!test_init()) {
        glutLogMessage("E/ Can't initialize test !");
        return -1;
    }

    glutMainLoop();

    return 0;
}
