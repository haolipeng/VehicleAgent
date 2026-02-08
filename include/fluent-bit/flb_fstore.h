#ifndef FLB_FSTORE_H
#define FLB_FSTORE_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>
#include <chunkio/chunkio.h>
#include <chunkio/cio_stats.h>

#define FLB_FSTORE_LOCK        222
#define FLB_FSTORE_UNLOCK      333

#define FLB_FSTORE_MEM         CIO_STORE_MEM
#define FLB_FSTORE_FS          CIO_STORE_FS

struct flb_fstore_file {
    flb_sds_t name;                 /* file name */

    void *meta_buf;                 /* copy of metadata content */
    size_t meta_size;               /* metadata size */
    void *data;                     /* opaque data type for user/caller context */
    struct cio_chunk *chunk;        /* chunk context */
    struct cio_stream *stream;      /* parent stream that owns this file */
    struct mk_list _head;           /* link to parent flb_fstore->files */
};

struct flb_fstore_stream {
    char *name;                     /* reference to stream->name */
    flb_sds_t path;                 /* stream filesystem path */
    struct cio_stream *stream;      /* Chunk I/O stream context */
    struct mk_list files;
    struct mk_list _head;
};

struct flb_fstore {
    int store_type;
    char *root_path;
    struct cio_ctx *cio;            /* Chunk I/O context */
    struct mk_list streams;
};

struct flb_fstore *flb_fstore_create(char *path, int store_type);

int flb_fstore_destroy(struct flb_fstore *fs);

struct flb_fstore_stream *flb_fstore_stream_create(struct flb_fstore *fs,
                                                   char *stream_name);
void flb_fstore_stream_destroy(struct flb_fstore_stream *stream, int delete);

int flb_fstore_file_meta_set(struct flb_fstore *fs,
                             struct flb_fstore_file *fsf,
                             void *meta, size_t size);

int flb_fstore_file_meta_get(struct flb_fstore *fs,
                             struct flb_fstore_file *fsf);

struct flb_fstore_file *flb_fstore_file_create(struct flb_fstore *fs,
                                               struct flb_fstore_stream *fs_stream,
                                               char *name, size_t size);

int flb_fstore_file_content_copy(struct flb_fstore *fs,
                                 struct flb_fstore_file *fsf,
                                 void **out_buf, size_t *out_size);

int flb_fstore_file_append(struct flb_fstore_file *fsf, void *data, size_t size);
struct flb_fstore_file *flb_fstore_file_get(struct flb_fstore *fs,
                                            struct flb_fstore_stream *fs_stream,
                                            char *name, size_t size);
int flb_fstore_file_inactive(struct flb_fstore *fs,
                             struct flb_fstore_file *fsf);
int flb_fstore_file_delete(struct flb_fstore *fs,
                           struct flb_fstore_file *fsf);
void flb_fstore_dump(struct flb_fstore *fs);

#endif
