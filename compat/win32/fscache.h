#ifndef FSCACHE_H
#define FSCACHE_H

int fscache_enable(int enable, int explicit_only);
#define enable_fscache(x) fscache_enable(x, 0)

#define enable_explicit_fscache(x) fscache_enable(x, 1)

int fscache_enabled(const char *path);
#define is_fscache_enabled(path) fscache_enabled(path)

DIR *fscache_opendir(const char *dir);
int fscache_lstat(const char *file_name, struct stat *buf);

#endif
