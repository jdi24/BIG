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

	client_handler_ = new ClientHandler();
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

void BigBrowser::SendMouseMoveEvent(int x, int y, bool mouseLeave) {
	if(client_handler_.get() && client_handler_->GetBrowser().get()) {
		CefRefPtr<CefBrowserHost> host = client_handler_->GetBrowser()->GetHost();
		host->SendMouseMoveEvent(x, y, mouseLeave);
	}
}
