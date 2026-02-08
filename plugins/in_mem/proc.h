#ifndef IN_MEM_PROC_H
#define IN_MEM_PROC_H

#define PROC_PID_SIZE      1024
#define PROC_STAT_BUF_SIZE 1024

/*
 * This 'stat' format omits the first two fields, due to the nature
 * of sscanf(3) and whitespaces, programs with spaces in the name can
 * screw up when scanning the information.
 */
#define PROC_STAT_FORMAT "%c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld"

/* Our tast struct to read the /proc/PID/stat values */
struct proc_task {
    int  pid;                  /* %d  */
    char comm[256];            /* %s  */
    char state;			       /* %c  */
    int ppid;			       /* %d  */
    int pgrp;			       /* %d  */
    int session;		       /* %d  */
    int tty_nr;			       /* %d  */
    int tpgid;			       /* %d  */
    unsigned int flags;	       /* %u  */
    unsigned long minflt;	   /* %lu */
    unsigned long cminflt;	   /* %lu */
    unsigned long majflt;	   /* %lu */
    unsigned long cmajflt;	   /* %lu */
    unsigned long utime;	   /* %lu */
    unsigned long stime; 	   /* %lu */
    long cutime;		       /* %ld */
    long cstime;		       /* %ld */
    long priority;		       /* %ld */
    long nice;			       /* %ld */
    long num_threads;		   /* %ld */
    long itrealvalue;		   /* %ld */
    unsigned long long starttime; /* %llu */
    unsigned long vsize;	   /* %lu */
    long rss;			       /* %ld */

    /* Internal conversion */
    long    proc_rss;          /* bytes = (rss * PAGESIZE)      */
    char   *proc_rss_hr;       /* RSS in human readable format  */
};

struct proc_task *proc_stat(pid_t pid, int page_size);
void proc_free(struct proc_task *t);

#endif
