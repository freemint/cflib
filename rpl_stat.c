/*
 * Work around platform bugs in stat.
 */
#include "intern.h"
#if defined(__PUREC__) && !defined(_GNUC_SOURCE) && !defined(_MINTLIB_SOURCE) && defined(_FEATURES_H)
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>


#include <sys/ioctl.h>
#include <mint/mintbind.h>

ino_t __inode = 32;
static int _x_Bit_set_in_stat = 0;

/*
 * undocumented function in PCSTDLIB.LIB;
 * translates TOS error codes to unix style error
 * codes used by the library.
 */
#if !defined(__PUREC__) || defined(__USE_GEMLIB)
#define _XltErr(r) ((int)(-(r)))
#else
extern int _XltErr(unsigned int err);
#endif

/* ------------------------------------------------------------------------- */

/* date for files (like root directories) that don't have one */
#define OLDDATE __unixtime(0,0)

/* Convert a GEMDOS time to seconds since the epoch.
   The calculated value is always in UTC. */
static time_t __unixtime(unsigned timestamp, unsigned datestamp)
{
	struct tm tmbuf;
	register struct tm *tm = &tmbuf;

	tm->tm_sec = (timestamp & 31) << 1;
	tm->tm_min = (timestamp >> 5) & 63;
	tm->tm_hour = (timestamp >> 11) & 31;

	tm->tm_mday = datestamp & 31;
	tm->tm_mon = ((datestamp >> 5) & 15) - 1;
	tm->tm_year = 80 + ((datestamp >> 9) & 127);

	return mktime(tm);
}

/* ------------------------------------------------------------------------- */

/* 
 * stat system call wrapper
 * 
 * first try Fstat64, then fallback to Fxattr and convert
 * to struct stat
 */
static long __sys_stat(const char *path, struct stat *st, int lflag, int exact)
{
	long r;

	/* first try the native syscall */
	r = Fstat64(lflag, path, st);
	if (r == -ETOS_NOSYS || r == -ETOS_INVAL)
	{
		/* fall back to Fxattr */
		struct xattr xattr;

		r = Fxattr(lflag, path, &xattr);
		if (r == 0)
		{
			memset(st, 0, sizeof(*st));

			__quad_make(st->st_dev, 0, xattr.st_dev);
			st->st_ino = (__ino_t) xattr.st_ino;
			st->st_mode = (__mode_t) xattr.st_mode;
			st->st_nlink = (__nlink_t) xattr.st_nlink;
			st->st_uid = (__uid_t) xattr.st_uid;
			st->st_gid = (__gid_t) xattr.st_gid;
			__quad_make(st->st_rdev, 0, xattr.st_rdev);

			if (exact)
			{
				union
				{
					unsigned short s[2];
					unsigned long l;
				} data;

				data.l = xattr.st_mtime;
				st->st_mtime = __unixtime(data.s[0], data.s[1]);
				data.l = xattr.st_atime;
				st->st_atime = __unixtime(data.s[0], data.s[1]);
				data.l = xattr.st_ctime;
				st->st_ctime = __unixtime(data.s[0], data.s[1]);
			}

			st->st_size = (__off_t) xattr.st_size;
			st->st_blocks = (__off_t) (((__off_t) xattr.st_blocks * (__off_t) xattr.st_blksize) >> 9);
			st->st_blksize = xattr.st_blksize;
			/* st->st_flags = 0; */
			/* st->st_gen = 0; */
		}
	}

	return r;
}

/* ------------------------------------------------------------------------- */

static int __do_stat(const char *_path, struct stat *st, int lflag)
{
	char pathbuf[PATH_MAX];
	char *path;
	long r;

	if (!_path)
	{
		__set_errno(EFAULT);
		return -1;
	}

	if (*_path == '\0')
	{
		__set_errno(ENOENT);
		return -1;
	}

	strcpy(pathbuf, _path);
	path = pathbuf;
	r = __sys_stat(path, st, lflag, 1);
	if (r != -ETOS_NOSYS)
	{
		if (r)
		{
			if (r == -ETOS_NOTDIR)
			{
				r = -ETOS_NOENT;
			}
			__set_errno(_XltErr((int)r));
			return -1;
		}
		return 0;
	}

	{
		_DTA *olddta;
		char *ext, drv;
		char *prevdir = NULL;
		int fd;
		short magic;
		_DTA d;
		int isdot = 0;
		int isdir = 0;

		memset(st, 0, sizeof(*st));

		/* A file name: check for root directory of a drive */
		if (path[0] == '\\' && path[1] == 0)
		{
			drv = Dgetdrv() + 'A';
			isdir = 1;
			goto rootdir;
		}

		if (((drv = path[0]) != 0) && path[1] == ':' && (path[2] == 0 || (path[2] == '\\' && path[3] == 0)))
		{
		  rootdir:
			isdir = 1;
			st->st_mode = S_IFDIR | 0755;
			st->st_flags = 0;
			__quad_make(st->st_dev, 0, isupper(drv) ? drv - 'A' : drv - 'a');
			st->st_ino = 2;
			st->st_mtime = st->st_ctime = st->st_atime = OLDDATE;
			goto fill_dir;
		}

		/* forbid wildcards in path names */
		if (strchr(path, '*') || strchr(path, '?'))
		{
			__set_errno(ENOENT);
			return -1;
		}

		/*
		 * OK, here we're going to have to do an Fsfirst to get the date
		 * 
		 * NOTE: Fsfirst(".",-1) or Fsfirst("..",-1) both fail under TOS,
		 * so we kludge around this by using the fact that Fsfirst(".\*.*"
		 * or "..\*.*" will return the correct file first (except, of course,
		 * in root directories :-( ).
		 * NOTE2: Some versions of TOS don't like Fsfirst("RCS\\", -1) either,
		 * so we do the same thing if the path ends in '\\'.
		 */

		/* Find the end of the string, and previous directory for kludging */
		for (ext = path; ext[0] && ext[1]; ext++)
		{
			if (ext[1] && ext[1] != '.')
			{
				if (ext[0] == '\\')
				{
					prevdir = ext;
				}
			}
		}

		/* Add appropriate kludge if necessary. */

		/* Handle C:\XXXX\. */
		if (*ext == '.' && (ext == path || ext[-1] == '\\'))
		{
			isdot = 1;
			strcat(path, "\\*.*");
		}
		/* Now, Handle C:\XXXX\.. */
		else if (*ext == '.' && (ext == path || ext[-1] == '.'))
		{
			/*
			 * XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
			 * FIXME.
			 * Need to handle recursively, such as....
			 * C:\XXXX\YYYY\ZZZZ\..\..\..
			 *
			 * Also, need to handle non-rooted drives such as...
			 * ..\..\.., where the CWD needs to be retrieved.
			 * XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
			 */

			isdot = 1;
			if (prevdir)
			{
				/* 
				 * In the case of C:\XXXX\YYYY\.., we now have....
				 * C:\XXXX\*.*
				 */
				strcpy(prevdir, "\\*.*\0");
			} else
			{
				/*
				 * In the case of C:\.., we now have....
				 * C:\*.*
				 */
				strcpy(&ext[-2], "\\*.*\0");
			}
		}
		/* Finally, Handle C:\XXXX\ */
		else if (*ext == '\\')
		{
			isdot = 1;
			strcat(path, "*.*");
		}
		olddta = Fgetdta();
		Fsetdta(&d);
		r = Fsfirst(path, 0xff);
		Fsetdta(olddta);
		if (r < 0)
		{
			/* 
			 * This is incorrect. When Fsfirst fails for things such as
			 * C:\\FOO\\ and appends *.*, to become C:\\FOO\\*.*, and
			 * we get ENOENT, why did we say it was a directory and return
			 * success ???
			 *
			 * Commenting out. See bug....
			 * http://sparemint.atariforge.net/bugtracker/view.php?id=191
			 *
			 * if (isdot && r == -ENOENT) goto rootdir;
			 */
			__set_errno(ENOENT);
			return -1;
		}

		if (isdot && ((d.dta_name[0] != '.') || (d.dta_name[1])))
		{
			goto rootdir;
		}

		st->st_mtime = st->st_ctime = st->st_atime = __unixtime(d.dta_time, d.dta_date);
		if (((drv = *path) != 0) && path[1] == ':')
			__quad_make(st->st_dev, 0, toupper(drv) - 'A');
		else
			__quad_make(st->st_dev, 0, Dgetdrv());

		isdir = (d.dta_attribute & FA_DIR) != 0;

		st->st_ino = __inode++;
		st->st_flags = 0;
		st->st_mode = 0644 | (isdir ? S_IFDIR | 0111 : S_IFREG);

		if (st->st_flags & FA_RDONLY)
			st->st_mode &= ~0222;		/* no write permission */
		if (st->st_flags & FA_HIDDEN)
			st->st_mode &= ~0444;		/* no read permission */

		/* check for a file with an executable extension */
		ext = strrchr(path, '.');
		if (ext)
		{
			if (!strcmp(ext, ".app") ||
				!strcmp(ext, ".gtp") ||
				!strcmp(ext, ".ttp") ||
				!strcmp(ext, ".prg") ||
				!strcmp(ext, ".tos"))
			{
				st->st_mode |= 0111;
			}
		}
		if ((st->st_mode & S_IFMT) == S_IFREG)
		{
			if (_x_Bit_set_in_stat)
			{
				if ((fd = (int) Fopen(path, 0)) < 0)
				{
					__set_errno(_XltErr(fd));
					return -1;
				}
				magic = 0;
				(void) Fread(fd, 2, (char *) &magic);
				(void) Fclose(fd);
				if (magic == 0x601A		/* TOS executable */
					|| magic == 0x2321)	/* "#!" shell file */
					st->st_mode |= 0111;
			}
			st->st_size = d.dta_size;
			/* in Unix, blocks are measured in 512 bytes */
			st->st_blocks = (st->st_size + 511) / 512;
			st->st_nlink = 1;			/* we dont have hard links */
		} else
		{
		  fill_dir:
			st->st_size = 1024;
			st->st_blocks = 2;
			st->st_nlink = 2;			/* "foo" && "foo/.." */
		}

		st->st_uid = 0;			/* the current user owns every file */
		st->st_gid = 0;
		st->st_blksize = 1024;

	}

	return 0;
}

/* ------------------------------------------------------------------------- */

int stat(const char *path, struct stat *st)
{
	return __do_stat(path, st, 0);
}

/* ------------------------------------------------------------------------- */

int lstat(const char *path, struct stat *st)
{
	return __do_stat(path, st, 1);
}

/* ------------------------------------------------------------------------- */

static long __sys_fstat (short fd, struct stat *st, int exact)
{
	long r;

	/* first try the native syscall */
	r = Ffstat64 (fd, st);
	if (r == -ETOS_NOSYS)
	{
		/* try the stat64 fcntl() */
		r = Fcntl (fd, st, FSTAT64);
		if (r == -ETOS_NOSYS || r == -ETOS_INVAL)
		{
			/* fall back to the xattr fcntl() */
			struct xattr xattr;

			r = Fcntl (fd, &xattr, FSTAT);
			if (r == 0)
			{
				memset(st, 0, sizeof (*st));

				__quad_make(st->st_dev, 0, xattr.st_dev);
				st->st_ino = (ino_t) xattr.st_ino;
				st->st_mode = (mode_t) xattr.st_mode;
				st->st_nlink = (nlink_t) xattr.st_nlink;
				st->st_uid = (uid_t) xattr.st_uid;
				st->st_gid = (gid_t) xattr.st_gid;
				__quad_make(st->st_rdev, 0, xattr.st_rdev);

				if (exact)
				{
					union { unsigned short s[2]; unsigned long l; } data;
					data.l = xattr.st_mtime;
					st->st_mtime = __unixtime (data.s[0], data.s[1]);
					data.l = xattr.st_atime;
					st->st_atime = __unixtime (data.s[0], data.s[1]);
					data.l = xattr.st_ctime;
					st->st_ctime = __unixtime (data.s[0], data.s[1]);
				}

				st->st_size = (off_t) xattr.st_size;
				st->st_blocks = 
					(off_t) (((off_t) xattr.st_blocks 
						 * (off_t) xattr.st_blksize) >> 9);
				st->st_blksize = xattr.st_blksize;
				/* st->st_flags = 0; */
				/* st->st_gen = 0; */
			}
		}
	}
	return r;
}

/* ------------------------------------------------------------------------- */

int __do_fstat(int fd, struct stat *st, int exact)
{
	long r;

	r = __sys_fstat(fd, st, exact);
	if (r != -ETOS_NOSYS)
	{
		if (r)
		{
			__set_errno(_XltErr((int)r));
			return -1;
		}
		return 0;
	}

	/* emulation for TOS */
	memset(st, 0, sizeof (*st));

	{
		long oldplace;
		_DOSTIME timeptr;
		short magic;

		if (!gl_mint && isatty(fd))
			r = -ETOS_BADF;
		else
			r = Fdatime(&timeptr, fd, 0);
		
		if (r < 0)			/* assume TTY */
		{
			st->st_mode = S_IFCHR | 0600;
			st->st_flags = 0;
			if (exact)
				st->st_mtime = st->st_ctime = st->st_atime =
					time ((time_t*) 0) - 2;
					
			st->st_size = 0;
		} else
		{
			if (exact)
				st->st_mtime = st->st_atime = st->st_ctime =
					__unixtime(timeptr.time, timeptr.date);
			st->st_mode = S_IFREG | 0644;	/* this may be false */
			st->st_flags = 0;		/* because this is */

			/* get current file location */
			oldplace = Fseek(0L, fd, SEEK_CUR);
			if (oldplace < 0)	/* can't seek -- must be pipe */
			{
				st->st_mode = S_IFIFO | 0644;
				st->st_size = 1024;
			} else
			{
				/* Go to end of file. */
				r = Fseek(0L, fd, SEEK_END);
				st->st_size = r;
				/* Go to start of file. */
				(void) Fseek (0L, fd, SEEK_SET);
				/* Check for executable file. */
				if (Fread (fd, 2, (char *)&magic) == 2)
				{
					if (magic == 0x601a 
					    || magic == 0x2321)
						st->st_mode |= 0111;
				}
				(void) Fseek (oldplace, fd, SEEK_SET);
			}
		}

		/* all this stuff is likely bogus as well. sigh. */
		__quad_make(st->st_dev, 0, Dgetdrv());
		__quad_make(st->st_rdev, 0, 0);
		st->st_uid = getuid();
		st->st_gid = getgid();
		st->st_blksize = 1024;
		/* Note: most Unixes measure st_blocks in 512 byte units. */
		st->st_blocks = (st->st_size + 511) / 512;
		st->st_ino = __inode++;
		st->st_nlink = 1;
	
	}

	return 0;
}

/* ------------------------------------------------------------------------- */

int fstat(int fd, struct stat *st)
{
	return __do_fstat(fd, st, 1);
}

#endif
