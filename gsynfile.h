#ifndef __GSYN_FILE_H
#define __GSYN_FILE_H

#define GSYNTH_FLOAT_SCALE (1<<24)

extern int gsynth_new(void);
extern int gsynth_open(const char *fname);
extern int gsynth_save(const char *fname);

#endif

