#include "machine.h"
#include "gui-common.h"
#include <stdio.h>
#include <gsynth.h>
#include <libgsyn/modload.h>

Connection::Connection(Module *m1, Module *m2) { from=m1; to=m2; }

void Connection::Draw()
{
	float x1, y1, x2, y2;
	from->GetPosition(x1, y1);
	to->GetPosition(x2, y2);
	draw_machine_connection(x1, y1, x2, y2);
}

// this is gonna be kinda ugly...
#define CONNECTION_RADIUS 20.0/640

int Connection::HitTest(float x, float y)
{
	float x1, y1, x2, y2;
	from->GetPosition(x1, y1);
	to->GetPosition(x2, y2);
	x1 += x2; x1 /= 2;
	y1 += y2; y1 /= 2;
	if(x > x1 + CONNECTION_RADIUS || x < x1 - CONNECTION_RADIUS) return 0;
	if(y > y1 + CONNECTION_RADIUS || y < y1 - CONNECTION_RADIUS) return 0;
	return 1;
}

Module::Module(SBuffer *sbuf, float initx, float inity, const char *n)
{
	if(!n) n = sbuf->get_name();
	else sbuf->set_name(n);
	name = strdup(n);
	smodule = sbuf;
	x = initx;
	y = inity;
	state = MACHINE_MASTER;
}

void Module::SetName(const char *n)
{
	if(!n) return;
	if(name) free(name);
	name = strdup(n);
	smodule->set_name(n);
}

Module::~Module()
{
	list<Connection*>::iterator i;
	for(i = connections.begin(); i != connections.end(); i++)
		delete (*i);
	free(name);
}

void Module::GetPosition(float &xx, float &yy)
{
	xx = x;
	yy = y;
}

int Module::HitTest(float hx, float hy)
{
	return machine_hit_test(name, x, y, hx, hy);
}

void Module::DrawConnections()
{
	list<Connection*>::iterator i;
	for(i = connections.begin(); i != connections.end(); i++)
		(*i)->Draw();
}

void Module::DrawModule()
{
	draw_machine_box(name, x, y, state);
}

void Module::SetState(int st)
{
	state = st;
	DrawModule();
}

void Module::Move(float newx, float newy)
{
	x = newx;
	y = newy;
	// whole window needs repaint at this point... can't do it here
}

void Module::AddConnection(Module *output)
{
	if( output->GetSModule()->add_dependency(GetSModule()) )
		connections.push_back(new Connection(this, output));
}

void Module::RemoveConnection(Module *mod)
{
	list<Connection*>::iterator i,j;
	for(i=connections.begin();i!=connections.end();i=j)
	{
		j = i; j++;
		if((*i)->to == mod) {
			delete (*i);
			connections.erase(i);
			mod->GetSModule()->remove_dependency(GetSModule());
		}
	}
}

MachineEditor::MachineEditor()
{
	ModuleInfo *mi;

	highlight = active = connect_from = NULL;
	active_connection = NULL;
	dragging = 0;
	dragging_connection = 0;

	mi = modload_next_module(1);
	while(mi) {
		add_machine_name(mi->name, mi);
		mi = modload_next_module(0);
	}
}

MachineEditor::~MachineEditor()
{
}

void MachineEditor::regenerate_tracker_mlist(void)
{
	list<Module*>::iterator i;

	tracker_mlist_clear();
	for(i=modules.begin();i!=modules.end();i++)
		tracker_mlist_add((*i)->GetName(), (*i)->GetSModule());
}

void MachineEditor::AddModule(Module *mod)
{
	modules.push_back(mod);
	tracker_mlist_add(mod->GetName(), (void*)mod->GetSModule());
}

void MachineEditor::RemoveModule(Module *mod)
{
	list<Module*>::iterator i,mod_iter = modules.end();
	for(i=modules.begin();i!=modules.end();i++) {
		if((*i) == mod) mod_iter = i;
		else (*i)->RemoveConnection(mod);
	}

	if(mod_iter == modules.end()) return;
	modules.erase(mod_iter);

	gsynth_core->RemoveModule(mod->GetSModule());
	delete mod;

	regenerate_tracker_mlist();
}

void MachineEditor::Redraw()
{
	list<Module*>::iterator i;

	clear_machine_window();

	if(dragging_connection)
		draw_machine_connection(dragx, dragy, createx, createy);

	for(i=modules.begin();i!=modules.end();i++)
		(*i)->DrawConnections();
	for(i=modules.begin();i!=modules.end();i++)
		(*i)->DrawModule();

	flush_machine_window();
}

void MachineEditor::OnMouseButton(int btn, int state, int x, int y)
{
//	printf("button %d state %d (%d %d)\n", btn, state, x, y);
	if(btn == 1 && (state&GUI_STATE_BUTTON_DOWN) && highlight) // button 1 pressed
	{
		if(state&GUI_STATE_SHIFT) {
			dragging_connection = 1;
			connect_from = highlight;
		}
		else
			dragging = 1;
		machine_position_abs_to_rel(x, y, &dragx, &dragy);
		createx = dragx; createy = dragy;
	}
	if(btn == 1 && !(state&GUI_STATE_BUTTON_DOWN)) {
		dragging = 0;
		if(dragging_connection) {
			dragging_connection = 0;
			if(highlight && highlight != connect_from)
				connect_from->AddConnection(highlight);
			Redraw();
		}
	}
}

void MachineEditor::OnMouseMove(int x, int y)
{
	list<Module*>::iterator i;
	list<Connection*>::iterator c;
	float fx, fy;
	machine_position_abs_to_rel(x, y, &fx, &fy);

	createx = fx;
	createy = fy;

	active_connection = NULL;
	if(!dragging) {
		for(i=modules.begin();i!=modules.end();i++) 
		{
			if((*i)->HitTest(fx, fy)) {
				if(highlight && ((*i) == highlight)) break;
				if(highlight)
					highlight->SetState(MACHINE_MASTER);
				highlight = (*i);
				highlight->SetState(MACHINE_PRELIGHT);
				flush_machine_window();
			} else if((*i) == highlight) {
				highlight->SetState(MACHINE_MASTER);
				highlight = NULL;
				flush_machine_window();
			}
			for(c = (*i)->connections.begin(); c != (*i)->connections.end(); c++) {
				if(!(*c)->HitTest(fx, fy)) continue;
				active_connection = (*c);
				break;
			}
		}
		if(dragging_connection) Redraw();
	} else {
		float ox, oy;
		if(fx < 0) fx = 0; if(fx > 1) fx = 1;
		if(fy < 0) fy = 0; if(fy > 1) fy = 1;
		highlight->GetPosition(ox, oy);
		highlight->Move(ox+fx-dragx, oy+fy-dragy);
		dragx = fx; dragy = fy;
		Redraw();
	}
}

Module *MachineEditor::CreateModule(ModuleInfo *mi)
{
	Module *newmod;
	SBuffer *sbuf;
	char name[256];
	static int module_no = 1;

	sbuf = gsynth_core->InstantiateModule(mi);
	sprintf(name, "(%d) %s", module_no++, sbuf->get_name());
	newmod = new Module(sbuf, createx, createy, name);

	AddModule(newmod);
	Redraw();

	return newmod;
}

const char *MachineEditor::GetModuleName(void)
{
	if(!active) return NULL;
	return active->GetName();
}

int MachineEditor::OnMenuPopup(void)
{
	active = highlight;
	if(highlight) return 1;
	if(active_connection) return 2;
	return 0;
}

void MachineEditor::OnRemoveModule(void)
{
	Module *mod = highlight;
	highlight = NULL;

	if(!mod) return;

	// can't delete the master
	if(mod->GetSModule() == gsynth_core->GetMasterBuf()) return;
	RemoveModule(mod);
	Redraw();
}

void MachineEditor::OnRenameModule(const char *name)
{
	if(!active) return;
	active->SetName(name);
	regenerate_tracker_mlist();
	Redraw();
}

const char *MachineEditor::OnAboutModule(void)
{
	ModuleInfo *mi;
	if(!active) return NULL;
	mi = active->GetSModule()->get_moduleinfo();
	if(!mi) return NULL;
	return mi->about;
}

void MachineEditor::OnDisconnect(void)
{
	if(!active_connection) return;
	active_connection->from->RemoveConnection(active_connection->to);
	Redraw();
}

