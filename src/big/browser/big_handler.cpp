#include "big_handler.h"

ClientHandler::ClientHandler(CefWindowHandle window_handle)
: window_handle_(window_handle)
, browser_id_(0)
, renderer_(NULL)
, width_(0)
, height_(0)
{
}

ClientHandler::~ClientHandler() {
	if(renderer_) {
		delete renderer_;
		renderer_ = NULL;
	}
}

CefRefPtr<CefBrowser> ClientHandler::GetBrowser() {
	return browser_;
}

bool ClientHandler::IsTransparent(int x, int y) {
	AutoLock lock_scope(this);

	if(renderer_) {
		return renderer_->IsTransparent(x, y);
	}
	return true;
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

void ClientHandler::SetSize(int width, int height) {
	width_ = width;
	height_ = height;
}

void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	if(!browser_.get()) {
		browser_ = browser;
		browser_id_ = browser->GetIdentifier();
	}
}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	if(browser_id_ == browser->GetIdentifier()) {
		// Free the browser pointer so that the browser can be destroyed
		browser_ = NULL;
	}
}

bool ClientHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
	rect.x = rect.y = 0;
	rect.width = width_;
	rect.height = height_;
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

void ClientHandler::OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor) {
	if (!::IsWindow(window_handle_))
		return;

	// Change the plugin window's cursor.
	SetClassLong(window_handle_, GCL_HCURSOR,
		static_cast<LONG>(reinterpret_cast<LONG_PTR>(cursor)));
	SetCursor(cursor);
}
