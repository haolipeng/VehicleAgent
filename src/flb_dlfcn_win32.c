#include <fluent-bit/flb_compat.h>

static CHAR dlerrorbuf[512];
static BOOL has_error_message = FALSE;

static void store_error(void)
{
    DWORD err = GetLastError();
    if (err == NO_ERROR) {
        return;
    }

    if (!FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM |
                        FORMAT_MESSAGE_IGNORE_INSERTS,
                        NULL,
                        err,
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                        dlerrorbuf,
                        _countof(dlerrorbuf), NULL))
        dlerrorbuf[0] = '\0';

    has_error_message = TRUE;
}

__declspec(noinline)
void *dlopen(const char *filename, int _flag)
{
    HMODULE handle;

    handle = LoadLibrary(filename);
    if (handle == NULL) {
        store_error();
        return NULL;
    }
    return (void *)handle;
}

char *dlerror(void)
{
    char *errorptr = dlerrorbuf;

    /* POSIX requests that the second consective dlerror() calling should
     * be return NULL.*/
    if (!has_error_message)
    {
        return NULL;
    }

    has_error_message = FALSE;

    return errorptr;
}

__declspec(noinline)
void *dlsym(void *handle, const char *name)
{
    FARPROC *symbol;
    symbol = NULL;

    symbol = GetProcAddress((HMODULE) handle, name);
    if (symbol == NULL) {
        store_error();
        return NULL;
    }

    return (void *)symbol;
}

int dlclose(void *handle)
{
    BOOL result;

    result = FreeLibrary((HMODULE) handle);
    if (!result)
        store_error();

    /* dlcose(3) returns 0 on success, and nonzero on error. */
    /* FreeLibrary returns nonzero on success, and 0 on error. */
    /* ref:
     * https://docs.microsoft.com/en-us/windows/desktop/api/libloaderapi/nf-libloaderapi-freelibrary */
    return !result;
}
