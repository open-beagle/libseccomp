/**
 * Seccomp Library test program
 *
 * Copyright (c) 2012 Red Hat <pmoore@redhat.com>
 * Author: Paul Moore <paul@paul-moore.com>
 */

/*
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of version 2.1 of the GNU Lesser General Public License as
 * published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses>.
 */

#include <errno.h>
#include <unistd.h>

#include <seccomp.h>

#include "util.h"

int main(int argc, char *argv[])
{
	int rc;
	struct util_options opts;
	scmp_filter_ctx ctx = NULL;

	rc = util_getopt(argc, argv, &opts);
	if (rc < 0)
		goto out;

	ctx = seccomp_init(SCMP_ACT_KILL);
	if (ctx == NULL)
		return ENOMEM;

	rc = seccomp_arch_remove(ctx, SCMP_ARCH_NATIVE);
	if (rc != 0)
		goto out;

	rc = seccomp_arch_add(ctx, seccomp_arch_resolve_name("x86"));
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, seccomp_arch_resolve_name("x86_64"));
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, seccomp_arch_resolve_name("x32"));
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, seccomp_arch_resolve_name("arm"));
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, seccomp_arch_resolve_name("aarch64"));
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, seccomp_arch_resolve_name("loongarch64"));
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, seccomp_arch_resolve_name("mipsel"));
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, seccomp_arch_resolve_name("mipsel64"));
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, seccomp_arch_resolve_name("mipsel64n32"));
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, seccomp_arch_resolve_name("ppc64le"));
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, seccomp_arch_resolve_name("riscv64"));
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, seccomp_arch_resolve_name("sh"));
	if (rc != 0)
		goto out;

	rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 1,
			      SCMP_A0(SCMP_CMP_EQ, STDIN_FILENO));
	if (rc != 0)
		goto out;

	rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 1,
			      SCMP_A0(SCMP_CMP_EQ, STDOUT_FILENO));
	if (rc != 0)
		goto out;

	rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 1,
			      SCMP_A0(SCMP_CMP_EQ, STDERR_FILENO));
	if (rc != 0)
		goto out;

	rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(close), 0);
	if (rc != 0)
		goto out;

	rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigreturn), 0);
	if (rc != 0)
		goto out;

	rc = util_filter_output(&opts, ctx);
	if (rc)
		goto out;

out:
	seccomp_release(ctx);
	return (rc < 0 ? -rc : rc);
}
