#ifndef BIG_HANDLER_H_
#define BIG_HANDLER_H_

#include "include/cef_client.h"

class BigRenderer {
public:
	virtual ~BigRenderer() {};
	virtual void OnPaint(const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height) = 0;
	virtual void Paint() = 0;
};

class ClientHandler : public CefClient
	, CefLifeSpanHandler
	, CefRenderHandler
{
public:
	ClientHandler();
	~ClientHandler();

	CefRefPtr<CefBrowser> GetBrowser();
	void PaintGLES();
	void SetRenderer(BigRenderer* renderer);
	void SetSize(int width, int height);

	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE { return this; }

	// CefLifeSpanHandler methods
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// CefRenderHandler methods
	virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;
	virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type
		, const RectList& dirtyRects, const void* buffer, int width, int height) OVERRIDE;

protected:
	IMPLEMENT_REFCOUNTING(ClientHandler);
	IMPLEMENT_LOCKING(ClientHandler);

private:
	CefRefPtr<CefBrowser> browser_;
	int browser_id_;

	BigRenderer* renderer_;

	int width_;
	int height_;
};

#endif