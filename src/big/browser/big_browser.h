#ifndef BIG_BROWSER_H_
#define BIG_BROWSER_H_

#include "big_handler.h"

void BigInit();

void BigShutdown();

class BigRenderer;

class BigBrowser {
public:
	BigBrowser(int width, int height, const CefString& url);
	~BigBrowser();

	void Paint();
	void SetRenderer(BigRenderer* renderer);

	// Message related

	// Send a mouse move event to the browser. The |x| and |y| coordinates are
	// relative to the upper-left corner of the view.
	void SendMouseMoveEvent(int x, int y, bool mouseLeave);
private:
	CefRefPtr<ClientHandler> client_handler_;
};

#endif