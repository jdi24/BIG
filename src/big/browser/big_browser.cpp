#include "big_browser.h"

#include <ShellAPI.h>

#include "include/cef_app.h"

void BigInit() {
	// Initialize for cef.
	CefMainArgs args;
	CefSettings settings;
	// Run cef's message loop on separate thread.
	settings.multi_threaded_message_loop = true;
	// Enable dev tools
	settings.remote_debugging_port = 8088;
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

BigBrowser::BigBrowser(int width, int height, const CefString& url, CefWindowHandle window_handle) {
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

	client_handler_ = new ClientHandler(window_handle);
	client_handler_->SetSize(width, height);
	CefBrowserHost::CreateBrowser(windowInfo, client_handler_.get()
		, url
		, browserSettings);
}

BigBrowser::~BigBrowser() {

}

void BigBrowser::Paint() {
	if(client_handler_.get()) {
		client_handler_->PaintGLES();
	}
}

void BigBrowser::SetRenderer(BigRenderer* renderer) {
	if(client_handler_.get()) {
		client_handler_->SetRenderer(renderer);
	}
}

void BigBrowser::OpenDevTools() {
	if(client_handler_.get() && client_handler_->GetBrowser().get()) {
		CefRefPtr<CefBrowserHost> host = client_handler_->GetBrowser()->GetHost();
		CefString dev_tools_url_http = host->GetDevToolsURL(true);
		// Open dev tools with system's default browser
		ShellExecute(NULL, L"open", dev_tools_url_http.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
}

bool BigBrowser::IsTransparent(int x, int y) {
	if(client_handler_.get()) {
		return client_handler_->IsTransparent(x, y);
	}
	return true;
}

void BigBrowser::SendCaptureLostEvent() {
	if(client_handler_.get() && client_handler_->GetBrowser().get()) {
		CefRefPtr<CefBrowserHost> host = client_handler_->GetBrowser()->GetHost();
		host->SendCaptureLostEvent();
	}
}

void BigBrowser::SendFocusEvent(bool setFocus) {
	if(client_handler_.get() && client_handler_->GetBrowser().get()) {
		CefRefPtr<CefBrowserHost> host = client_handler_->GetBrowser()->GetHost();
		host->SendFocusEvent(setFocus);
	}
}

void BigBrowser::SendMouseClickEvent(int x, int y, CefBrowserHost::MouseButtonType type, bool mouseUp, int clickCount) {
	if(client_handler_.get() && client_handler_->GetBrowser().get()) {
		CefRefPtr<CefBrowserHost> host = client_handler_->GetBrowser()->GetHost();
		host->SendMouseClickEvent(x, y, (CefBrowserHost::MouseButtonType)type, mouseUp, clickCount);
	}
}

void BigBrowser::SendMouseMoveEvent(int x, int y, bool mouseLeave) {
	if(client_handler_.get() && client_handler_->GetBrowser().get()) {
		CefRefPtr<CefBrowserHost> host = client_handler_->GetBrowser()->GetHost();
		host->SendMouseMoveEvent(x, y, mouseLeave);
	}
}

VOID BigBrowser::SendMouseWheelEvent(int x, int y, int deltaX, int deltaY) {
	if(client_handler_.get() && client_handler_->GetBrowser().get()) {
		CefRefPtr<CefBrowserHost> host = client_handler_->GetBrowser()->GetHost();
		host->SendMouseWheelEvent(x, y, deltaX, deltaY);
	}
}
