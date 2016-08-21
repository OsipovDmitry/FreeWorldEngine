#ifndef __IWINDOW__
#define __IWINDOW__


#include "Types.h"

namespace FreeWorldEngine {

class IWindow {
public:
	enum IWindowFlags {
		IWindow_Fullscreen = 0x00000001,
		IWindow_Show       = 0x00000002,
		IWindow_Resizable  = 0x00000004,
	};

	virtual ~IWindow() = 0 {}
	virtual void show() const = 0;
	virtual void hide() const = 0;
	virtual void move(int32 x, int32 y) const = 0;
	virtual void resize(int32 width, int32 height) const = 0;
	virtual void minimize() const = 0;
	virtual void restore() const = 0;
	virtual void maximize() const = 0;
	virtual void render() const = 0;
	virtual void update(uint32 time, uint32 dt) const = 0;

	virtual void setFuncShown(void (*)()) = 0;
	virtual void setFuncHidden(void (*)()) = 0;
	virtual void setFuncMoved(void (*)(int32, int32)) = 0;
	virtual void setFuncResized(void (*)(int32, int32)) = 0;
	virtual void setFuncMinimized(void (*)()) = 0;
	virtual void setFuncRestored(void (*)()) = 0;
	virtual void setFuncMaximized(void (*)()) = 0;
	virtual void setFuncClose(void (*)()) = 0;
	virtual void setFuncRender(void (*)()) = 0;
	virtual void setFuncUpdate(void (*)(uint32, uint32)) = 0;

	virtual uint32 id() const = 0;
	virtual uint32 width() const = 0;
	virtual uint32 height() const = 0;
	virtual uint64 frameNumber() const = 0;
	virtual void setMousePos(const int32 x, const int32 y) const = 0;

};

} // namespace

#endif // __IWINDOW__