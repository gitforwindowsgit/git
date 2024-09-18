#define USE_THE_REPOSITORY_VARIABLE

#include "test-tool.h"
#include "git-compat-util.h"
#include "parse-options.h"
#include "setup.h"
#include "revision.h"
#include "path-walk.h"
#include "oid-array.h"
#include "hex.h"

static const char * const path_walk_usage[] = {
	N_("test-tool path-walk <options>"),
	NULL
};

static int verbose;

static int callback_fn(const char *path,
		       struct oid_array *list,
		       enum object_type type,
		       void *data)
{
	printf("visiting path '%s' (count: %"PRIuMAX") of type '%s'\n",
		path, (uintmax_t)list->nr, type_name(type));
	if (verbose)
		for (size_t i = 0; i < list->nr; i++)
			printf("\t%s\n", oid_to_hex(list->oid + i));

	return 0;
}

int cmd__path_walk(int argc, const char **argv)
{
	struct rev_info revs;
	struct path_walk_info info = PATH_WALK_INFO_INIT;
	struct option options[] = {
		OPT_BOOL(0, "tags", &info.tags, N_("walk tags")),
		OPT_BOOL(0, "commits", &info.commits, N_("walk commits")),
		OPT_BOOL(0, "trees", &info.trees, N_("walk trees")),
		OPT_BOOL(0, "blobs", &info.blobs, N_("walk blobs")),
		OPT__VERBOSE(&verbose, N_("verbose")),
		OPT_END(),
	};
	int ret = 0;

	if (argc == 2 && !strcmp(argv[1], "-h"))
		usage_with_options(path_walk_usage, options);

	argc = parse_options(argc, argv, NULL, options, path_walk_usage,
			     PARSE_OPT_KEEP_ARGV0);

	setup_git_directory();

	repo_init_revisions(the_repository, &revs, "");
	setup_revisions(argc, argv, &revs, NULL);

	info.revs = &revs;
	info.path_fn = callback_fn;

	ret = walk_objects_by_path(&info);

	reset_revision_walk();
	release_revisions(&revs);

	return ret;
}
