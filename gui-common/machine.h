#ifndef __MACHINE_H
#define __MACHINE_H

#include <libgsyn/module.h>
#include <libgsyn/syncore.h>
#include <stlstuff.h>

class Module;
struct Connection
{
	Module *from, *to;
	Connection(Module *m1, Module *m2);

	int HitTest(float x, float y);
	void Draw();
};

class Module
{
private:
	SBuffer *smodule;
	float x,y;
	char *name;
	int state;
public:
	list<Connection*> connections;  // Connections only go from this machine to its outputs

// Constructor: if name is NULL, query module for its name
	Module(SBuffer *sbuf, float initx = 0.5, float inity = 0.5, const char *name = NULL);
	~Module();

	void Move(float newx, float newy);
	void GetPosition(float &xx, float &yy);
	const char *GetName(void) { return name; }
	void DrawConnections();
	void DrawModule();

	void AddConnection(Module *output);
	void RemoveConnection(Module *output);

	void SetState(int st);  // Set graphical state
	void SetName(const char *newname);

	int HitTest(float hx, float hy);

	SBuffer *GetSModule() { return smodule; }
};

class MachineEditor
{
private:
	list<Module*> modules; // list of modules (duh)
	Module *master;
	Module *highlight, *connect_from, *active;
	Connection *active_connection;
	int dragging, dragging_connection;
	float dragx, dragy;
	float createx, createy;

	void regenerate_tracker_mlist(void);
public:
	MachineEditor();
	~MachineEditor();

	void AddModule(Module *mod);
	void RemoveModule(Module *mod);

	Module *CreateModule(ModuleInfo *mi);

	void SetMaster(Module *m) { master = m; }
	Module *GetMaster(void) { return master; }

	const char *GetModuleName(void); // assumes highlighted module

	void Redraw();
	void OnMouseButton(int btn, int down, int x, int y);
	void OnMouseMove(int x, int y);
	int OnMenuPopup(void);
	void OnRemoveModule(void);
	void OnRenameModule(const char *);
	void OnDisconnect(void);
	const char* OnAboutModule(void);

	// for saving machine state to disk
	list<Module*> &GetModuleList(void) { return modules; }
};

#endif

