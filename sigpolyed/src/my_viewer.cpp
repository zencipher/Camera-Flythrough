
# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>
#include <math.h> 

# include <sigogl/ws_run.h>

int cameraMove = 0;
int cameraIndex = 0;
int balloonCamIndex = 0;
int templeCamIndex = 0;
int noahCamIndex = 0;
int typeBSpline = 1;
bool animating = true;
int animateType = 0;

SnManipulator* house_manip = new SnManipulator;
SnManipulator* temple_manip = new SnManipulator;
SnManipulator* balloon_manip = new SnManipulator;

static void my_polyed_callback ( SnPolyEditor* pe, SnPolyEditor::Event e, int pid )
{
	MyViewer* v = (MyViewer*)pe->userdata();
	if ( e==SnPolyEditor::PostMovement || e==SnPolyEditor::PostEdition || e==SnPolyEditor::PostInsertion )
	{	v->update_scene ();
	}
}

MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	rootg()->add ( _polyed = new SnPolyEditor );
	rootg()->add(_polyedCurve = new SnPolyEditor);
	rootg()->add ( _curveA = new SnLines2 );
	rootg()->add(_curveB = new SnLines2);
	rootg()->add(_curveC = new SnLines2);
	rootg()->add(_curveD = new SnLines2);

	rootg()->add(firstPath = new SnLines);
	rootg()->add(secondPath = new SnLines);
	rootg()->add(thirdPath = new SnLines);
	rootg()->add(balloonPath = new SnLines);

	shrinePoint.push(GsPnt(0, 0, 450));
	shrinePoint.push(GsPnt(0, 50, 350));
	shrinePoint.push(GsPnt(0, 50, 230));
	shrinePoint.push(GsPnt(20, 50, 130));
	shrinePoint.push(GsPnt(60, 80, 100));
	shrinePoint.push(GsPnt(70, 80, 80));
	shrinePoint.push(GsPnt(80, 70, 10));
	shrinePoint.push(GsPnt(80, 60, -10));
	shrinePoint.push(GsPnt(40, 70, -10));
	shrinePoint.push(GsPnt(0, 70, 0));
	shrinePoint.push(GsPnt(10, 50, 0));
	shrinePoint.push(GsPnt(0, 0, 450));

	noahPoint.push(GsPnt(250, 0, 300));
	noahPoint.push(GsPnt(250, 20, 250));
	noahPoint.push(GsPnt(280, 0, 50));
	noahPoint.push(GsPnt(280, 0, 0));
	noahPoint.push(GsPnt(300, 10, -20));
	noahPoint.push(GsPnt(290, 10, -30));
	noahPoint.push(GsPnt(280, 10, -40));
	noahPoint.push(GsPnt(250, 20, -30));
	noahPoint.push(GsPnt(280, 20, -40));
	noahPoint.push(GsPnt(290, 100, -40));
	noahPoint.push(GsPnt(270, 110, -45));
	noahPoint.push(GsPnt(260, 110, -45));
	noahPoint.push(GsPnt(260, 110, -30));
	noahPoint.push(GsPnt(270, 120, -40));
	noahPoint.push(GsPnt(270, 120, -50));
	noahPoint.push(GsPnt(270, 130, -60));
	noahPoint.push(GsPnt(270, 130, -150));
	noahPoint.push(GsPnt(300, 30, -130));

	templePoint.push(GsPnt(600, 0, 100));
	templePoint.push(GsPnt(650, 10, 60));
	templePoint.push(GsPnt(650, 10, 30));
	templePoint.push(GsPnt(630, 10, 20));
	templePoint.push(GsPnt(600, 20, 20));
	templePoint.push(GsPnt(590, 20, -50));
	templePoint.push(GsPnt(530, 20, 0));

	balloonPoint.push(GsPnt(0, 300, 300));
	balloonPoint.push(GsPnt(0, 300, 300));
	balloonPoint.push(GsPnt(300, 300, 300));
	balloonPoint.push(GsPnt(600, 300, 300));
	balloonPoint.push(GsPnt(600, 300, 0));
	balloonPoint.push(GsPnt(300, 300, 0));
	balloonPoint.push(GsPnt(0, 300, 0));
	balloonPoint.push(GsPnt(0, 300, 300));
	balloonPoint.push(GsPnt(0, 300, 300));


	firstPath->begin_polyline();
	for (int i = 0; i < shrinePoint.size(); i++) {
		firstPath->push(shrinePoint[i]);
	}
	firstPath->end_polyline();

	secondPath->begin_polyline();
	for (int i = 0; i < noahPoint.size(); i++) {
		secondPath->push(noahPoint[i]);
	}
	secondPath->end_polyline();

	thirdPath->begin_polyline();
	for (int i = 0; i < templePoint.size(); i++) {
		thirdPath->push(templePoint[i]);
	}
	thirdPath->end_polyline();

	balloonPath->begin_polyline();
	for (int i = 0; i < balloonPoint.size(); i++) {
		balloonPath->push(balloonPoint[i]);
	}
	balloonPath->end_polyline();

	firstPath->color(GsColor(200, 50, 0));
	firstPath->line_width(2.0f);

	secondPath->color(GsColor(0, 50, 0));
	secondPath->line_width(2.0f);

	thirdPath->color(GsColor(200, 50, 0));
	thirdPath->line_width(2.0f);

	balloonPath->color(GsColor(200, 50, 0));
	balloonPath->line_width(2.0f);

	_curveA->color ( GsColor(20,200,25) );
	_curveA->line_width ( 2.0f );
	_curveB->color(GsColor(20, 200, 25));
	_curveB->line_width(2.0f);
	_curveC->color(GsColor(20, 200, 25));
	_curveC->line_width(2.0f);
	_curveD->color(GsColor(20, 200, 25));
	_curveD->line_width(2.0f);

	// you may add new curves here

	// set initial control polygon:
	_polyed->callback ( my_polyed_callback, this );
	_polyed->max_polygons (1);
	_polyed->solid_drawing (0);
	GsPolygon& P = _polyed->polygons()->push();
	P.setpoly ( "-2 -2  -1 1  1 0  2 -2" );
	P.open(true);

	GsMat rotx;
	rotx.rotx(-3.1415f / 2);

	SnGroup * shrine_group = new SnGroup;
	SnModel * shrine = new SnModel;

	GsMat t;
	t.translation(GsVec(0, 0, 0));
	t.mult(t, rotx);

	shrine->model()->load("../shrine/tinker.obj");
	shrine_manip->child(shrine_group);
	shrine_group->add(shrine);
	shrine->model()->smooth();
	shrine_manip->initial_mat(t);

	SnGroup * house_group = new SnGroup;
	SnModel * house = new SnModel;

	GsMat house_mat;
	house_mat.translation(GsVec(300, 0, 0));
	house_mat.mult(house_mat, rotx);

	house->model()->load("../noah/tinker.obj");
	house_manip->child(house_group);
	house_group->add(house);
	house->model()->smooth();
	house_manip->initial_mat(house_mat);

	SnGroup * temple_group = new SnGroup;
	SnModel * temple = new SnModel;

	GsMat temple_mat;
	temple_mat.translation(GsVec(600, 0, 0));
	temple_mat.mult(temple_mat, rotx);

	temple->model()->load("../temple/tinker.obj");
	temple_manip->child(temple_group);
	temple_group->add(temple);
	temple->model()->smooth();
	temple_manip->initial_mat(temple_mat);

	GsMat balloon_mat;
	balloon_mat.translation(GsVec(300, 300, 0));
	balloon_mat.mult(balloon_mat, rotx);

	SnGroup* balloon_group = new SnGroup;
	SnModel* balloon = new SnModel;

	balloon->model()->load("../balloon/tinker.obj");
	balloon_manip->child(balloon_group);
	balloon_group->add(balloon);
	balloon->model()->smooth();
	balloon_manip->initial_mat(balloon_mat);

	
	rootg()->add(shrine_manip);
	rootg()->add(house_manip);
	rootg()->add(temple_manip);
	rootg()->add(balloon_manip);
	// start:
	build_ui ();
	update_scene ();
	message() = "Click on polygon to edit, use Esc to switch edition mode, Del deletes selected points. Enjoy!";
}

void MyViewer::build_ui ()
{
	UiPanel *p;
	p = uim()->add_panel ( "", UiPanel::HorizLeft );

	p->add ( _viewA=new UiCheckButton ( "CurveA", EvViewCurveA, false ) );
	p->add(_viewB = new UiCheckButton("CurveB", EvViewCurveB, false));
	p->add(_viewC = new UiCheckButton("CurveC", EvViewCurveC, false));

	p->add ( _slider=new UiSlider ( " dt:", EvDeltaT, 0,0,150 ) );
	_slider->separate();
	_slider->range ( 0.01f, 0.25f );

	p->add ( new UiButton ( "Exit", EvExit ) ); p->top()->separate();
}



static float numN(int i, int k, float t) { // k always = 3
	//Code taken from bSpline Lecture slides
	float ti = float(i);

	if (k == 1) {
		return ti <= t && t < ti + 1? 1.0f : 0;
	}
	else {
		return ((t - ti) / (k - 1)) * numN(i, k - 1, t) + 
			((ti + k - t) / (k - 1)) *	numN(i + 1, k - 1, t);
	}
}

static GsPnt eval_bSpline(float t, const GsArray<GsPnt>& P) {
	GsPnt curvePoint;
	
	for (int i = 0; i < P.size(); i++) {
		curvePoint += (P[i] * numN(i, 3, t));
	}

	return curvePoint;
}




void MyViewer::update_scene ()
{
	// Initializations:
	_curveA->init();
	_curveB->init();
	_curveC->init();
	_curveD->init();
	
	// Access the control polygon:
	GsPolygon& P = _polyed->polygon(0);
	float deltat = _slider->value();

	_curveD->begin_polyline();
	for (float t = 2; t<balloonPoint.size(); t += deltat) {
		_curveD->push(eval_bSpline(t, balloonPoint));
		balloonPointArray.push(eval_bSpline(t, balloonPoint));
	}

	_curveD->end_polyline();
	
	
	if (typeBSpline == 0) {
		_curveA->begin_polyline();
		for (float t = 2; t<shrinePoint.size(); t += deltat) {
			_curveA->push(eval_bSpline(t, shrinePoint));
			shrinePointArray.push(eval_bSpline(t, shrinePoint));
		}
		
		_curveA->end_polyline();
		
	}

	if (typeBSpline == 2) {
		_curveB->begin_polyline();
		for (float t = 2; t<noahPoint.size(); t += deltat) {
			_curveB->push(eval_bSpline(t, noahPoint));
			noahPointArray.push(eval_bSpline(t, noahPoint));
		}

		_curveB->end_polyline();
	}

	if (typeBSpline == 3) {
		_curveC->begin_polyline();
		for (float t = 2; t<templePoint.size(); t += deltat) {
			_curveC->push(eval_bSpline(t, templePoint));
			templePointArray.push(eval_bSpline(t, templePoint));
		}

		_curveC->end_polyline();
	}

}
void MyViewer::animate(int animateType) {
	GsMat balloonMat = balloon_manip->mat();
	if (animateType == 1) {
		GsMat moveForward;
		GsMat rotx;
		rotx.rotx(-3.1415f / 2);

		if (balloonCamIndex > balloonPointArray.size()) {
			balloonCamIndex = 0;
		}

		else {
			moveForward.translation(balloonPointArray[balloonCamIndex]);
			moveForward.mult(moveForward, rotx);


			balloon_manip->initial_mat(moveForward);

			render(); // notify it needs redraw
			ws_check(); // redraw now

		}
		
	}
}

void MyViewer::moveCamera(int cameraMove) {

	if (cameraMove == 1) {
		if (cameraIndex > shrinePointArray.size()) {
			cameraIndex = 0;
		}
		else {
			camera().center = shrinePointArray[cameraIndex];
			//gsout << shrinePointArray[cameraIndex];
			camera().eye = shrinePointArray[cameraIndex - 1];
		}

	}

	if (cameraMove == 2) {
		if (noahCamIndex > noahPointArray.size()) {
			noahCamIndex = 0;
		}
		else {
			camera().center =noahPointArray[noahCamIndex];
			//gsout << shrinePointArray[cameraIndex];
			camera().eye = noahPointArray[noahCamIndex - 1];
		}

	}

	if (cameraMove == 3) {
		if (templeCamIndex > templePointArray.size()) {
			templeCamIndex = 0;
		}
		else {
			camera().center = templePointArray[templeCamIndex];
			//gsout << shrinePointArray[cameraIndex];
			camera().eye = templePointArray[templeCamIndex - 1];
		}

	}

	render();
	ws_check();
}

int MyViewer::uievent ( int e )
{
	switch ( e )
	{
	case EvViewCurveA: {
		if (typeBSpline == 0) {
			typeBSpline = 1;
			update_scene();
			return 1;
		}

		if (typeBSpline == 1) {
			typeBSpline = 0;
			update_scene();
			return 1;
		}
	}
	case EvViewCurveB: {
		if (typeBSpline == 0) {
			typeBSpline = 2;
			update_scene();
			return 1;
		}

		if (typeBSpline == 2) {
			typeBSpline = 0;
			update_scene();
			return 1;
		}
	}

	case EvViewCurveC: {
		if (typeBSpline == 0) {
			typeBSpline = 3;
			update_scene();
			return 1;
		}

		if (typeBSpline == 3) {
			typeBSpline = 0;
			update_scene();
			return 1;
		}
	}

		case EvDeltaT:		update_scene(); return 1;
		case EvExit: gs_exit();

		case ' ': {
			return 1;
		}
	}
	return WsViewer::uievent(e);
}

int MyViewer::handle_keyboard(const GsEvent &e) {

	switch (e.key) {
	case GsEvent::KeyEsc: gs_exit(); return 1;
	case 'q': {
		if (cameraMove == 0) {
			moveCamera(1);
			cameraIndex+= 2;
			return 1;
		}


		//update_scene();
		return 1;
	}

	case 'w': {
		if (cameraMove == 0) {
			moveCamera(2);
			noahCamIndex+= 2;
			return 1;
		}


		//update_scene();
		return 1;
	}

	case 'e': {
		if (cameraMove == 0) {
			moveCamera(3);
			templeCamIndex += 2;
			return 1;
		}


		//update_scene();
		return 1;
	}

	case 'd': {
		while (animating) {
			animate(1);
			balloonCamIndex++;
		}

		return 1;
	}

	default: gsout << "Key pressed: " << e.key << gsnl;
	}
	return 0;

}
