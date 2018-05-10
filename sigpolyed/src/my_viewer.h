# pragma once

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>

# include <sigogl/ui_check_button.h>
# include <sigogl/ui_slider.h>
# include <sigogl/ws_viewer.h>
# include <sig/sn_manipulator.h>


// Viewer for this example:
class MyViewer : public WsViewer
{  protected :
	enum MenuEv { EvExit, EvViewCurveA, EvViewCurveB, EvViewCurveC, EvDeltaT };
	UiCheckButton *_viewA;
	UiCheckButton *_viewB;
	UiCheckButton *_viewC;
	UiSlider* _slider;
	SnPolyEditor* _polyed;
	SnPolyEditor* _polyedCurve;
	SnLines2 *_curveA;
	SnLines2 *_curveB;
	SnLines2 *_curveC;
	SnLines2 *_curveD;

	SnLines* firstPath;
	SnLines* secondPath;
	SnLines* thirdPath;
	SnLines* balloonPath;

	GsArray<GsPnt> shrinePoint;
	GsArray<GsPnt> noahPoint;
	GsArray<GsPnt> templePoint;
	GsArray<GsPnt> balloonPoint;
	GsArray<GsPnt> shrinePointArray;
	GsArray<GsPnt> noahPointArray;
	GsArray<GsPnt> templePointArray;
	GsArray<GsPnt> balloonPointArray;

   public :
	MyViewer ( int x, int y, int w, int h, const char* l );
	void build_ui ();
	void update_scene ();
	void moveCamera(int cameraMove);
	void animate(int animateType);
	virtual int uievent ( int e ) override;
	virtual int handle_keyboard(const GsEvent &e) override;

	SnManipulator * shrine_manip = new SnManipulator;
};

