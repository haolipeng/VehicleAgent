#define RTLD_LAZY 0

void *dlopen(const char *filename, int _flag);
char *dlerror(void);
void *dlsym(void *handle, const char *symbol);
int dlclose(void *handle);
