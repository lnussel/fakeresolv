/*
 * q2hack Copyright (C) 2001 Zinx Verituse <zinx@magenet.net>
 * Copyright (C) 2007 SUSE Linux Products GmbH
 * Copyright (C) 2017 SUSE LLC
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#define __USE_GNU 1 /* RTLD_NEXT isn't defined on some systems
			(namely glibc 2.2+ systems) unless __USE_GNU is */
#include <dlfcn.h>

static FILE* (*real_fopen64)(const char* path, const char* mode);
static int got_fopen64 = 0;

/****/

static void *get_addr(void *handle, char *name)
{
	char *err;

	void* sym = dlsym(handle, name);
	if (!sym && (err=dlerror())) {
		fprintf(stderr, "Error getting real %s address: %s\n", name, err);
		exit(-1);
	}
	return sym;
}

#define GET_NEXTADDR(x) \
	do { if (!got_##x) real_##x = get_addr(RTLD_NEXT, #x), got_##x = 1; } while(0)

FILE* fopen64(const char* path, const char* mode)
{
	GET_NEXTADDR(fopen64);

	if (!strcmp(path, "/etc/resolv.conf")) {
		const char* p = getenv("RESOLV_CONF");
		if (p)
			path = p;
	}

	return real_fopen64(path, mode);
}
