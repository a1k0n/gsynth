// gsynfile.cpp - save and load gsyn files.
// all file data is in network byte order.

#include <string.h>
#include <stdio.h>
#include <gsynth.h>
#include <libgsyn/modload.h>
#include <gui-common/machine.h>
#include "gsynfile.h"

// for ntohl/htonl
#ifdef WIN32
#include <winsock.h>
#define strcasecmp stricmp
#else
#include <netinet/in.h> 
#endif

static ModuleInfo *locate_module(const char *base)
{
	ModuleInfo *mi = modload_next_module(1);
	while(mi) {
		if(!strcasecmp(mi->basename, base))
			return mi;
		mi = modload_next_module(0);
	}
	return NULL;
}

static Module *locate_module_id(int id)
{
	list<Module*> modlist = gsynth_editor->GetModuleList();
	list<Module*>::iterator i;
	for(i=modlist.begin();i!=modlist.end();i++)
		if((*i)->GetSModule()->get_id() == id) return (*i);
	return NULL;
}

static gsynth_Pattern *locate_pattern(int patID, SBuffer *sbuf)
{
	list<gsynth_Pattern*>::iterator i;
	for(i=sbuf->patterns.begin();i!=sbuf->patterns.end();i++)
		if((*i)->get_id() == patID) return (*i);
	return NULL;
}

int gsynth_new(void)
{
	// delete all modules but master, move master to center.
	list<Module*> modlist = gsynth_editor->GetModuleList();
	list<Module*>::iterator i,j;
	for(i=modlist.begin();i!=modlist.end();i=j) {
		j = i; j++;
		if((*i)->GetSModule() == gsynth_core->GetMasterBuf()) {
			(*i)->Move(0.5, 0.5);
			continue;
		}
		gsynth_editor->RemoveModule(*i);
	}
	// FIXME: doesn't remove the patterns associated with master
	gsynth_editor->Redraw();
	return 1;
}

// To open:
// read in a module instance, use gui-common machine editor to instantiate the
// modules, then set their positions.  use the GetSBuffer function to set the
// IDs.  Create tracks with the gui-common track editor object and ignore the
// sequencer entries for the time being.

static void gsynth_read_pattern(FILE *fp, SBuffer *buf)
{
	int length, tracks, id, i;
	char name[32];
	gsynth_Pattern *pat;

	fread(&id, 4, 1, fp); id = ntohl(id);
	fread(name, 32, 1, fp);
	fread(&length, 4, 1, fp); length = ntohl(length);
	fread(&tracks, 4, 1, fp); tracks = ntohl(tracks);
	// multiple tracks are unsupported
	pat = buf->new_pattern(length, name);
	pat->set_id(id);

	long *dptr, datsize, data;
	dptr = pat->GetData(0,0);
	// yeah, i know it's ugly.
	datsize = pat->GetData(0,1) - dptr;
	datsize *= tracks*length;
	for(i=0;i<datsize;i++)
	{
		fread(&data, 4, 1, fp);
		dptr[i] = ntohl(data);
	}
}

static int gsynth_read_module(FILE *fp)
{
	ModuleInfo *mi;
	Module *mod = NULL;
	char basename[32];
	char modulename[32];

	int module_id;
	fread(&module_id, 4, 1, fp); module_id = ntohl(module_id);
	fread(basename, 32, 1, fp);
	fread(modulename, 32, 1, fp);

	if(module_id) {
		mi = locate_module(basename);
		if(mi) mod = gsynth_editor->CreateModule(mi);
		if(mod) mod->GetSModule()->set_id(module_id);
	}
	else mod = gsynth_editor->GetMaster();
	if(!mod) {
		fprintf(stderr, "Can't load module \"%s\".  Aborting load.\n", basename);
		return 0;
	}
	int x, y;
	fread(&x, 4, 1, fp); x = ntohl(x);
	fread(&y, 4, 1, fp); y = ntohl(y);
	mod->Move((float)x/GSYNTH_FLOAT_SCALE, (float)y/GSYNTH_FLOAT_SCALE);
	mod->SetName(modulename);
	
	int num_patterns;
	fread(&num_patterns, 4, 1, fp); num_patterns = ntohl(num_patterns);
	for(;num_patterns;num_patterns--)
		gsynth_read_pattern(fp, mod->GetSModule());
	
	int num_entries;
	fread(&num_entries, 4, 1, fp); num_entries = ntohl(num_entries);
	for(;num_entries;num_entries--) {
		int row, patID;
		gsynth_Pattern *p;
		fread(&row, 4, 1, fp); row = ntohl(row);
		fread(&patID, 4, 1, fp); patID = ntohl(patID);
		p = locate_pattern(patID, mod->GetSModule());
		mod->GetSModule()->sequence.SetEntry(p, row);
	}

	return 1;
}

int gsynth_open(const char *fname)
{
	FILE *fp = fopen(fname, "rb");
	if(!fp) return 0;
	gsynth_new();

	// check GSYN tag
	if(fgetc(fp) != 'G') return 0;
	if(fgetc(fp) != 'S') return 0;
	if(fgetc(fp) != 'Y') return 0;
	if(fgetc(fp) != 'N') return 0;

	// get song name
	char songname[32];
	fread(&songname, 32, 1, fp);
	gsynth_core->SetSongName(songname);

	// load the modules
	int num_modules,i;
	fread(&num_modules, 4, 1, fp); num_modules = ntohl(num_modules);
	for(i=0;i<num_modules;i++)
		if(!gsynth_read_module(fp)) { fclose(fp); return 0; }
	
	// connect them
	int num_connections, src, dst;
	Module *srcmod, *dstmod;
	fread(&num_connections, 4, 1, fp); num_connections = ntohl(num_connections);
	for(i=0;i<num_connections;i++) {
		fread(&src, 4, 1, fp); src = ntohl(src);
		fread(&dst, 4, 1, fp); dst = ntohl(dst);
		srcmod = locate_module_id(src);
		dstmod = locate_module_id(dst);
		srcmod->AddConnection(dstmod);
	}
	
	fclose(fp);

	gsynth_editor->Redraw();

	return 1;
}


// To save:
// Grab list of module instances from the gui-common machine editor object for
// each module, grab the sbuffer, the sbuffer's moduleinfo, the position,
// patterns, and sequence.  then output connectivity graph which is also
// attainable from the gui-common machine builder object.

static void gsynth_write_pattern(FILE *fp, gsynth_Pattern *p)
{
	// write the ID
	int id = htonl(p->get_id());
	fwrite(&id, 4, 1, fp);

	// get the name as a 32-byte buffer
	char namebuf[32];
	memset(namebuf, 0, sizeof(namebuf));
	strncpy(namebuf, p->GetName(), 32);
	fwrite(namebuf, 32, 1, fp);

	// write length and tracks
	int length, tracks;
	length = htonl(p->GetLength());
	tracks = htonl(p->GetTracks());
	fwrite(&length, 4, 1, fp);
	fwrite(&tracks, 4, 1, fp);
	length = ntohl(length);
	tracks = ntohl(tracks);

	// convert (htonl) and write the pattern data
	int i;
	long *dptr, datsize, data;
	dptr = p->GetData(0,0);
	// yeah, i know it's ugly.
	datsize = p->GetData(0,1) - dptr;
	datsize *= tracks*length;
	for(i=0;i<datsize;i++)
	{
		data = htonl(dptr[i]);
		fwrite(&data, 4, 1, fp);
	}
}

static void gsynth_write_module(FILE *fp, Module *m)
{
	int id, ix, iy, num_patterns;
	float fx, fy;
	char namebuf[32];
	list<gsynth_Pattern*>::iterator pi;

	// write ID
	id = htonl(m->GetSModule()->get_id());
	fwrite(&id, 4, 1, fp);

	// write base module name
	memset(namebuf, 0, sizeof(namebuf));
	strncpy(namebuf, m->GetSModule()->get_moduleinfo()->basename, 32);
	fwrite(namebuf, 32, 1, fp);

	// write the module name
	memset(namebuf, 0, sizeof(namebuf));
	strncpy(namebuf, m->GetSModule()->get_name(), 32);
	fwrite(namebuf, 32, 1, fp);

	// convert and write position
	m->GetPosition(fx, fy);
	ix = htonl((int) (fx*GSYNTH_FLOAT_SCALE));
	iy = htonl((int) (fy*GSYNTH_FLOAT_SCALE));
	fwrite(&ix, 4, 1, fp);
	fwrite(&iy, 4, 1, fp);

	// write number of patterns
	num_patterns = htonl(m->GetSModule()->patterns.size());
	fwrite(&num_patterns, 4, 1, fp);

	// iterate through list, write patterns
	pi = m->GetSModule()->patterns.begin();
	while(pi != m->GetSModule()->patterns.end()) {
		gsynth_write_pattern(fp, *pi);
		pi++;
	}

	// write sequencer order
	gsynth_seq_entry *seqent;
	
	// first, get number of entries
	int num_entries = 0;
	seqent = m->GetSModule()->sequence.GetHeadEntry();
	while(seqent) {
		num_entries++;
		seqent = seqent->next;
	}
	num_entries = htonl(num_entries);
	fwrite(&num_entries, 4, 1, fp);

	// now write out each one
	seqent = m->GetSModule()->sequence.GetHeadEntry();
	while(seqent) {
		int row = htonl(seqent->row);
		int id = htonl(seqent->pat->get_id());
		fwrite(&row, 4, 1, fp);
		fwrite(&id, 4, 1, fp);
		seqent = seqent->next;
	}
}

int gsynth_save(const char *fname)
{
	if(!gsynth_editor) return 0;
	list<Module*> modlist = gsynth_editor->GetModuleList();
	list<Module*>::iterator i;
	list<Connection*>::iterator ci;
	int size;
	FILE *fp = fopen(fname, "wb");
	if(!fp) return 0;

	// write magic identifier
	fwrite("GSYN", 4, 1, fp);

	// write song name
	char songname[32];
	memset(songname, 0, sizeof(songname));
	strncpy(songname, gsynth_core->GetSongName(), 32);
	fwrite(songname, 32, 1, fp);

	// write number of modules
	size = htonl(modlist.size());
	fwrite(&size, 1, 4, fp);
	for(i=modlist.begin();i!=modlist.end();i++)
		gsynth_write_module(fp, *i);
	
	// find total number of connections
	size = 0;
	for(i=modlist.begin();i!=modlist.end();i++)
		size += (*i)->connections.size();
	size = htonl(size);
	fwrite(&size, 4, 1, fp);

	// write each one by ID
	for(i=modlist.begin();i!=modlist.end();i++) {
		for(ci=(*i)->connections.begin();ci!=(*i)->connections.end();ci++) {
			int srcID = htonl( (*ci)->from->GetSModule()->get_id() );
			int dstID = htonl( (*ci)->to->GetSModule()->get_id() );
			fwrite(&srcID, 4, 1, fp);
			fwrite(&dstID, 4, 1, fp);
		}
	}

	fclose(fp);

	return 1;
}

