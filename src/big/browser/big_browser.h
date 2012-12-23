#ifndef BIG_BROWSER_H_
#define BIG_BROWSER_H_

#include "include/cef_client.h"

void BigInit();

void BigShutdown();

class BigRenderer {
public:
	virtual ~BigRenderer() {};
	virtual void OnPaint(const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height) = 0;
	virtual void Paint() = 0;
};

class ClientHandler : public CefClient
	, CefRenderHandler
{
public:
	virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE { return this; }

	// CefRenderHandler methods
	virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;
	virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type
		, const RectList& dirtyRects, const void* buffer, int width, int height) OVERRIDE;

	void PaintGLES();
	void SetRenderer(BigRenderer* renderer);

	ClientHandler();
	~ClientHandler();

protected:
	IMPLEMENT_REFCOUNTING(ClientHandler);
	IMPLEMENT_LOCKING(ClientHandler);

private:
	BigRenderer* renderer_;
};

class BigBrowser {
public:
	BigBrowser(int width, int height, const CefString& url);
	~BigBrowser();

	void Paint();
	void SetRenderer(BigRenderer* renderer);
private:
	CefRefPtr<ClientHandler> clientHandler;
};

#endif