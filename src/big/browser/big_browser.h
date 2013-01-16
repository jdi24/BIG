#ifndef BIG_BROWSER_H_
#define BIG_BROWSER_H_

#include "big_handler.h"

void BigInit();

void BigShutdown();

class BigRenderer;

class BigBrowser {
public:
	BigBrowser(int width, int height, const CefString& url, CefWindowHandle window_handle);
	~BigBrowser();

	void Paint();
	void SetRenderer(BigRenderer* renderer);

	void OpenDevTools();

	// Message related

	bool IsTransparent(int x, int y);

	// Send a capture lost event to the browser.
	void SendCaptureLostEvent();

	// Send a focus event to the browser.
	void SendFocusEvent(bool setFocus);

	// Send a mouse click event to the browser. The |x| and |y| coordinates are
	// relative to the upper-left corner of the view.
	void SendMouseClickEvent(int x, int y, CefBrowserHost::MouseButtonType type, bool mouseUp, int clickCount);

	// Send a mouse move event to the browser. The |x| and |y| coordinates are
	// relative to the upper-left corner of the view.
	void SendMouseMoveEvent(int x, int y, bool mouseLeave);

	// Send a mouse wheel event to the browser. The |x| and |y| coordinates are
	// relative to the upper-left corner of the view. The |deltaX| and |deltaY|
	// values represent the movement delta in the X and Y directions respectively.
	void SendMouseWheelEvent(int x, int y, int deltaX, int deltaY);
private:
	CefRefPtr<ClientHandler> client_handler_;
};

#endif