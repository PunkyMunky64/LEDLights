#pragma once
#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

class Gfx {
	int x;
	int y;
	static Gfx* active_gfx;
	void(Gfx::* display_func)(void);
	void(*display_func_stray)(void);
	bool display_func_is_stray_flag;
protected:
	int winw;
	int winh;
public:
	Gfx(int, int);
	void set_active();
	static void run_active(int argc, char** argv);
	/* Handler for window-repaint event. Call back when the window first appears and
	whenever the window needs to be re-painted. */
	void set_display_func(void(Gfx::*display_func)());
	void set_display_func(void(*display_func)());
private:
	void display();
	static void display_active();
	/* Main function: GLUT runs as a console application starting at main()  */
};