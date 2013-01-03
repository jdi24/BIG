#include "big_renderer_gles20.h"

//GLT_SHADER_TEXTURE_REPLACE
// Just put the texture on the polygons
static const char *szTextureReplaceVP =	
"attribute vec4 vVertex;"
"attribute vec2 vTexCoord0;"
"varying vec2 vTex;"
"void main(void) "
"{ vTex = vTexCoord0;" 
" gl_Position = vVertex; "
"}";

static const char *szTextureReplaceFP = 
"precision mediump float;"
"varying vec2 vTex;"
"uniform sampler2D textureUnit0;"
"void main(void) "
"{ gl_FragColor = texture2D(textureUnit0, vTex); "
"}";

//////////////////////////////////////////////////////////////////////////
// Load the shader from the source text
static void gltLoadShaderSrc(const char *szShaderSrc, GLuint shader)
{
	GLchar *fsStringPtr[1];

	fsStringPtr[0] = (GLchar *)szShaderSrc;
	glShaderSource(shader, 1, (const GLchar **)fsStringPtr, NULL);
}

/////////////////////////////////////////////////////////////////
// Load a pair of shaders, compile, and link together. Specify the complete
// source code text for each shader. Note, there is no support for
// just loading say a vertex program... you have to do both.
static GLuint gltLoadShaderPairSrcWithAttributes(const char *szVertexSrc, const char *szFragmentSrc, ...)
{
	// Temporary Shader objects
	GLuint hVertexShader;
	GLuint hFragmentShader; 
	GLuint hReturn = 0;   
	GLint testVal;

	// Create shader objects
	hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load them. 
	gltLoadShaderSrc(szVertexSrc, hVertexShader);
	gltLoadShaderSrc(szFragmentSrc, hFragmentShader);

	// Compile them
	glCompileShader(hVertexShader);
	glCompileShader(hFragmentShader);

	// Check for errors
	glGetShaderiv(hVertexShader, GL_COMPILE_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		char infoLog[1024];
		glGetShaderInfoLog(hVertexShader, 1024, NULL, infoLog);
		fprintf(stderr, "The vertex shader failed to compile with the following error:\n%s\n", infoLog);
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return (GLuint)NULL;
	}

	glGetShaderiv(hFragmentShader, GL_COMPILE_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		char infoLog[1024];
		glGetShaderInfoLog(hFragmentShader, 1024, NULL, infoLog);
		fprintf(stderr, "The fragment shader failed to compile with the following error:\n%s\n", infoLog);
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return (GLuint)NULL;
	}

	// Link them - assuming it works...
	hReturn = glCreateProgram();
	glAttachShader(hReturn, hVertexShader);
	glAttachShader(hReturn, hFragmentShader);

	// List of attributes
	va_list attributeList;
	va_start(attributeList, szFragmentSrc);

	char *szNextArg;
	int iArgCount = va_arg(attributeList, int);	// Number of attributes
	for(int i = 0; i < iArgCount; i++)
	{
		int index = va_arg(attributeList, int);
		szNextArg = va_arg(attributeList, char*);
		glBindAttribLocation(hReturn, index, szNextArg);
	}
	va_end(attributeList);


	glLinkProgram(hReturn);

	// These are no longer needed
	glDeleteShader(hVertexShader);
	glDeleteShader(hFragmentShader);  

	// Make sure link worked too
	glGetProgramiv(hReturn, GL_LINK_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		glDeleteProgram(hReturn);
		return (GLuint)NULL;
	}

	return hReturn;  
}   

BigRendererGLES20::GLBatch::GLBatch()
:num_texture_units_(0), num_verts_(0)
, vertex_array_(0)
, normal_array_(0)
, verts_building_(0), texture_coord_array_(NULL)
{

}

BigRendererGLES20::GLBatch::~GLBatch() {
	// Vertex buffer objects
	if(vertex_array_ != 0)
		glDeleteBuffers(1, &vertex_array_);

	if(normal_array_ != 0)
		glDeleteBuffers(1, &normal_array_);

	for(unsigned int i = 0; i < num_texture_units_; i++)
		glDeleteBuffers(1, &texture_coord_array_[i]);

	delete [] texture_coord_array_;
}

// Start the primitive batch.
void BigRendererGLES20::GLBatch::Begin(GLenum primitive, GLuint nVerts, GLuint nTextureUnits) {
	primitive_type_ = primitive;
	num_verts_ = nVerts;

	if(nTextureUnits > 4)   // Limit to four texture units
		nTextureUnits = 4;

	num_texture_units_ = nTextureUnits;

	if(num_texture_units_ != 0) {
		texture_coord_array_ = new GLuint[num_texture_units_];

		// An array of pointers to texture coordinate arrays
		for(unsigned int i = 0; i < num_texture_units_; i++) {
			texture_coord_array_[i] = 0;
		}
	}
}

enum GLT_SHADER_ATTRIBUTE { GLT_ATTRIBUTE_VERTEX = 0, GLT_ATTRIBUTE_COLOR, GLT_ATTRIBUTE_NORMAL, 
GLT_ATTRIBUTE_TEXTURE0, GLT_ATTRIBUTE_TEXTURE1, GLT_ATTRIBUTE_TEXTURE2, GLT_ATTRIBUTE_TEXTURE3, 
GLT_ATTRIBUTE_LAST};

void BigRendererGLES20::GLBatch::Draw(void) {
	if(vertex_array_ !=0) {
		glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_array_);
		glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if(normal_array_ != 0) {
		glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
		glBindBuffer(GL_ARRAY_BUFFER, normal_array_);
		glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// How many texture units
	for(unsigned int i = 0; i < num_texture_units_; i++) {
		if(texture_coord_array_[i] != 0) {
			glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0 + i);
			glBindBuffer(GL_ARRAY_BUFFER, texture_coord_array_[i]);
			glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0 + i, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}
	}

	glDrawArrays(primitive_type_, 0, num_verts_);

	glDisableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
	glDisableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
	glDisableVertexAttribArray(GLT_ATTRIBUTE_COLOR);

	for(unsigned int i = 0; i < num_texture_units_; i++) {
		if(texture_coord_array_[i] != 0) {
			glDisableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0 + i);
		}
	}
}

// Bind everything up in a little package
void BigRendererGLES20::GLBatch::End(void) {
	if(vertex_array_ !=0) {
		glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_array_);
		glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if(normal_array_ != 0) {
		glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
		glBindBuffer(GL_ARRAY_BUFFER, normal_array_);
		glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// How many texture units
	for(unsigned int i = 0; i < num_texture_units_; i++) {
		if(texture_coord_array_[i] != 0) {
			glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0 + i);
			glBindBuffer(GL_ARRAY_BUFFER, texture_coord_array_[i]);
			glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0 + i, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}
	}
}

// Unlike normal OpenGL immediate mode, you must specify a texture coord
// per vertex or you will get junk...
void BigRendererGLES20::GLBatch::MultiTexCoord2f(GLuint texture, GLclampf s, GLclampf t) {
	// First see if the vertex array buffer has been created...
	if(texture_coord_array_[texture] == 0) {	// Nope, we need to create it
		glGenBuffers(1, &texture_coord_array_[texture]);
		glBindBuffer(GL_ARRAY_BUFFER, texture_coord_array_[texture]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * num_verts_, NULL, GL_DYNAMIC_DRAW);
	}

	// Ignore if we go past the end, keeps things from blowing up
	if(verts_building_ >= num_verts_)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, texture_coord_array_[texture]);
	GLfloat texCoord[2] = {s, t};
	int sizePerTexCoord = sizeof(GLfloat) * 2;
	glBufferSubData(GL_ARRAY_BUFFER, sizePerTexCoord * verts_building_, sizePerTexCoord, texCoord);
}

// Unlike normal OpenGL immediate mode, you must specify a normal per vertex
// or you will get junk...
void BigRendererGLES20::GLBatch::Normal3f(GLfloat x, GLfloat y, GLfloat z) {
	// First see if the vertex array buffer has been created...
	if(normal_array_ == 0) {	// Nope, we need to create it
		glGenBuffers(1, &normal_array_);
		glBindBuffer(GL_ARRAY_BUFFER, normal_array_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * num_verts_, NULL, GL_DYNAMIC_DRAW);
	}

	// Ignore if we go past the end, keeps things from blowing up
	if(verts_building_ >= num_verts_)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, normal_array_);
	GLfloat normal[3] = {x, y, z};
	int sizePerNormal = sizeof(GLfloat) * 3;
	glBufferSubData(GL_ARRAY_BUFFER, sizePerNormal * verts_building_, sizePerNormal, normal);
}

// Add a single vertex to the end of the array
void BigRendererGLES20::GLBatch::Vertex3f(GLfloat x, GLfloat y, GLfloat z) {
	// First see if the vertex array buffer has been created...
	if(vertex_array_ == 0) {	// Nope, we need to create it
		glGenBuffers(1, &vertex_array_);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_array_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * num_verts_, NULL, GL_DYNAMIC_DRAW);
	}

	// Ignore if we go past the end, keeps things from blowing up
	if(verts_building_ >= num_verts_)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, vertex_array_);
	GLfloat vertex[3] = {x, y, z};
	int sizePerVertex = sizeof(GLfloat) * 3;
	glBufferSubData(GL_ARRAY_BUFFER, sizePerVertex * verts_building_, sizePerVertex, vertex);

	verts_building_++;
}

BigRendererGLES20::BigRendererGLES20() 
: buffer_(NULL)
, width_(0)
, height_(0)
, shader_(0)
, texture_id_(0)
, buffer_ready_(false) 
, buffer_size_changed_(false)
{
	Init();
}

BigRendererGLES20::~BigRendererGLES20() {
	glDeleteProgram(shader_);
	FreeBuffer();
}

bool BigRendererGLES20::IsTransparent(int x, int y) {
	if(buffer_ready_) {
		int color = ((int*)buffer_)[y * width_ + x];
		return (color & 0xFF000000) == 0;
	}
	return true;
}

void BigRendererGLES20::OnPaint(const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height) {
	buffer_ready_ = false;

	int buffer_size = width * height * 4;
	if(width != width_ || height != height_) {
		buffer_size_changed_ = true;
		width_ = width;
		height_ = height;
		FreeBuffer();
		buffer_ = new char[buffer_size];
	}

	memcpy_s(buffer_, buffer_size, buffer, buffer_size);
	dirty_rects_ = dirtyRects;

	buffer_ready_ = true;
}

void BigRendererGLES20::Paint() {
	if(!buffer_ready_)
		return;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(shader_);
	GLint iTextureUnit = glGetUniformLocation(shader_, "textureUnit0");
	glUniform1i(iTextureUnit, 0);
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	if(buffer_size_changed_) {
		// Update the whole texture.
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0
			, GL_RGBA, GL_UNSIGNED_BYTE, buffer_);
		buffer_size_changed_ = false;
	} else {
		CefRenderHandler::RectList::const_iterator i = dirty_rects_.begin();
		for(; i != dirty_rects_.end(); ++i) {
			const CefRect& rect = *i;
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0
				, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, buffer_);
		}
	}

	quad_batch_.Draw();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void BigRendererGLES20::FreeBuffer() {
	if(buffer_) {
		delete buffer_;
		buffer_ = NULL;
	}
}

void BigRendererGLES20::Init() {
	glGenTextures(1, &texture_id_);
	glBindTexture(GL_TEXTURE_2D, texture_id_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLubyte pBits[16] = {128, 0, 0, 128
		, 128, 0, 0, 128
		, 128, 0, 0, 128
		, 128, 0, 0, 128};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, pBits);

	quad_batch_.Begin(GL_TRIANGLES, 6, 1);

	quad_batch_.Normal3f(0.0f, 0.0f, 1.0f);
	quad_batch_.MultiTexCoord2f(0, 0.0f, 1.0f);
	quad_batch_.Vertex3f(-1.0f, -1.0f, 0.0f);

	quad_batch_.Normal3f(0.0f, 0.0f, 1.0f);
	quad_batch_.MultiTexCoord2f(0, 1.0f, 1.0f);
	quad_batch_.Vertex3f(1.0f, -1.0f, 0.0f);

	quad_batch_.Normal3f(0.0f, 0.0f, 1.0f);
	quad_batch_.MultiTexCoord2f(0, 1.0f, 0.0f);
	quad_batch_.Vertex3f(1.0f, 1.0f, 0.0f);

	quad_batch_.Normal3f(0.0f, 0.0f, 1.0f);
	quad_batch_.MultiTexCoord2f(0, 0.0f, 0.0f);
	quad_batch_.Vertex3f(-1.0f, 1.0f, 0.0f);

	quad_batch_.Normal3f(0.0f, 0.0f, 1.0f);
	quad_batch_.MultiTexCoord2f(0, 0.0f, 1.0f);
	quad_batch_.Vertex3f(-1.0f, -1.0f, 0.0f);

	quad_batch_.Normal3f(0.0f, 0.0f, 1.0f);
	quad_batch_.MultiTexCoord2f(0, 1.0f, 0.0f);
	quad_batch_.Vertex3f(1.0f, 1.0f, 0.0f);

	quad_batch_.End();

	shader_ = gltLoadShaderPairSrcWithAttributes(szTextureReplaceVP, szTextureReplaceFP, 2
		, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_TEXTURE0, "vTexCoord0");
}
