#include "big_browser.h"

#include "include/cef_app.h"

void BigInit() {
	// Initialize for cef.
	CefMainArgs args;
	CefSettings settings;
	// Run cef's message loop on separate thread.
	settings.multi_threaded_message_loop = true;
	// Use cefclient.exe to run render process and other sub processes.
	CefString(&settings.browser_subprocess_path) = "cefclient.exe";
	// Load locales/zh-CN.pak
	CefString(&settings.locale) = "zh-CN";
	CefInitialize(args, settings, NULL);
}

void BigShutdown() {
	// Must shut down cef before exit.
	CefShutdown();
}

BigBrowser::BigBrowser(int width, int height, const CefString& url) {
	// Browser initialization.
	CefWindowInfo windowInfo;
	// SetTransparentPainting(TRUE) makes the page's background has alpha channel.
	windowInfo.SetTransparentPainting(TRUE);
	// Off screen rendering will pass the bitmap of browser to the handler,
	// so we can use it as texture.
	// The parent HWND is set to NULL for convenient.
	windowInfo.SetAsOffScreen(NULL);
	windowInfo.x = 0;
	windowInfo.y = 0;
	windowInfo.width = width;
	windowInfo.height = height;

	CefBrowserSettings browserSettings;

	clientHandler = new ClientHandler();
	CefBrowserHost::CreateBrowser(windowInfo, clientHandler.get()
		, url
		, browserSettings);
}

BigBrowser::~BigBrowser() {

}

void BigBrowser::Paint() {
	if(clientHandler.get()) {
		clientHandler->PaintGLES();
	}
}

void BigBrowser::SetRenderer(BigRenderer* renderer) {
	if(clientHandler.get()) {
		clientHandler->SetRenderer(renderer);
	}
}

bool ClientHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
	rect.x = rect.y = 0;
	rect.width = 800;
	rect.height = 600;
	return true;
}

void ClientHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type
							, const RectList& dirtyRects, const void* buffer
							, int width, int height) {
	AutoLock lock_scope(this);

	if(renderer_) {
		renderer_->OnPaint(dirtyRects, buffer, width, height);
	}
}

void ClientHandler::PaintGLES() {
	AutoLock lock_scope(this);

	if(renderer_) {
		renderer_->Paint();
	}
}

void ClientHandler::SetRenderer(BigRenderer* renderer) {
	renderer_ = renderer;
}

ClientHandler::ClientHandler()
: renderer_(NULL)
{
}

ClientHandler::~ClientHandler() {
	if(renderer_) {
		delete renderer_;
		renderer_ = NULL;
	}
}
