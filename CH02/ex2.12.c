/* utmplib.c  - functions to buffer reads from utmp file 
 *
 *      functions are
 *              utmp_open( filename )   - open file
 *                      returns -1 on error
 *              utmp_next( )            - return pointer to next struct
 *                      returns NULL on eof
 *              utmp_close()            - close file
 *
 *      reads NRECS per read and then doles them out from the buffer
 */
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>
#include <string.h>

#define NRECS   16
#define NULLUT  ((struct utmp *)NULL)
#define UTSIZE  (sizeof(struct utmp))

static  char    utmpbuf[NRECS * UTSIZE];                /* storage      */
static  int     num_recs;                               /* num stored   */
static  int     cur_rec;                                /* next to go   */
static  int     fd_utmp = -1;                           /* read from    */

int utmp_open( char *filename )
{
    fd_utmp = open( filename, O_RDONLY );           /* open it      */
    cur_rec = num_recs = 0;                         /* no recs yet  */
    return fd_utmp;                                 /* report       */
}

struct utmp *utmp_next()
{
    struct utmp *recp;

    if ( fd_utmp == -1 )                            /* error ?      */
        return NULLUT;
    if ( cur_rec==num_recs && utmp_reload()==0 )    /* any more ?   */
        return NULLUT;
                                    /* get address of next record    */
    recp = ( struct utmp *) &utmpbuf[cur_rec * UTSIZE];
    cur_rec++;

    if(recp->ut_type == EMPTY)
        return utmp_next();
    else
        return recp;
}

int utmp_reload()
/*
 *      read next bunch of records into buffer
 */
{
    int     amt_read;

                                            /* read them in         */
    amt_read = read( fd_utmp , utmpbuf, NRECS * UTSIZE );   

                                            /* how many did we get? */
    num_recs = amt_read/UTSIZE;
                                            /* reset pointer        */
    cur_rec  = 0;
    return num_recs;
}

int utmp_seek(int record_offset, int base){
    record_offset = num_recs - (cur_rec + record_offset);
    if(lseek(fd_utmp, record_offset * UTSIZE, base) != -1)
        return 0;
    else
        return -1;
}

utmp_close()
{
    if ( fd_utmp != -1 )                    /* don't close if not   */
        close( fd_utmp );               /* open                 */
}

/*
 * logout_tty(char *line)
 * marks a utmp record as logged out
 * dose not blank username or remote host
 * return -1 on error, 0 on success
*/

int logout_tty(char *line)
{
    int fd;
    struct utmp rec;
    int len = sizeof(struct utmp);
    int retval = -1;

    if((fd = open(UTMP_FILE, O_RDWR)) == -1)
        return -1;
    while(read(fd, &rec, len) == len)
        if(strncmp(rec.ut_line, line, sizeof(rec.ut_line)) == 0)
            rec.ut_type = DEAD_PROCESS;
            if(time(&rec.ut_time) != -1)
                if(lseek(fd, -len, SEEK_CUR) != -1)
                    if(write(fd, &rec, len) == len)
                        retval = 0;
    if(close(fd) == -1)
        retval = -1;
    return retval;
}