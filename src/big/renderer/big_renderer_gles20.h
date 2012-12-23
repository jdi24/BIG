#ifndef BIG_RENDERER_GLES20_H_
#define BIG_RENDERER_GLES20_H_

#include "browser/big_browser.h"

#include "GLES2/gl2.h"

class BigRendererGLES20 : public BigRenderer {
public:
	BigRendererGLES20();
	virtual ~BigRendererGLES20();
	virtual void OnPaint(const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height);
	virtual void Paint();

private:
	void FreeBuffer();
	void Init();

private:
	class GLBatch {
	public:
		GLBatch();
		~GLBatch();

		// Start populating the array
		void Begin(GLenum primitive, GLuint nVerts, GLuint nTextureUnits = 0);

		// Tell the batch you are done
		void End(void);

		void Draw(void);

		void MultiTexCoord2f(GLuint texture, GLclampf s, GLclampf t);
		void Normal3f(GLfloat x, GLfloat y, GLfloat z);
		void Vertex3f(GLfloat x, GLfloat y, GLfloat z);
	private:
		GLenum		primitive_type_;		// What am I drawing....

		GLuint		vertex_array_;
		GLuint      normal_array_;
		GLuint		*texture_coord_array_;

		GLuint verts_building_;			// Building up vertexes counter (immediate mode emulator)
        GLuint num_verts_;				// Number of verticies in this batch
        GLuint num_texture_units_;		// Number of texture coordinate sets
	};
private:
	CefRenderHandler::RectList dirty_rects_;
	void* buffer_;
	int width_;
	int height_;

	GLuint shader_;
	GLuint texture_id_;
	GLBatch quad_batch_;

	bool buffer_ready_;
	bool buffer_size_changed_;
};

#endif