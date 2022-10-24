#include "argp.h"

#include "cp_lib/algorithm.cc"
#include "cp_lib/string.cc"
#include "cp_lib/io.cc"

#include "json-c/json.h"
#include "packcomp.h"


using namespace cp;

#ifdef VERSION
const char *argp_program_version = "packcomp " VERSION;
#endif
// const char *argp_program_bug_address = "<your@email.address>";

static char doc[] = 
"packcomp - fetches two branches, finds unique packages in both of them and also packages "
"version of which in the first branch is greater than version of the corresponding packages "
"in the second branch.";

static char args_doc[] = "[BRANCH1] [BRANCH2]...";

static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output"},
    {"arch", 'a', "ARCH", 0, "Specify architecture"},
    {"output", 'o', "FILE", 0, "Output to FILE instead of standard output"},
    {0}};

struct InputArgs {
    const char *arch = "";
    const char *out_file = "";
    const char *branches[2];
};

// static error_t parse_opt(int key, char *arg, struct argp_state *state) {
//     InputArgs *input = (InputArgs*)state->input;
//     switch (key) {
//     case 'b':
//         input->branch = arg;
//         break;
//     case ARGP_KEY_ARG:
//         return 0;
//     default:
//         return ARGP_ERR_UNKNOWN;
//     }   
//     return 0;
// }


static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  InputArgs *input_args = (InputArgs*) state->input;

    switch (key)
    {
    case 'v':
        g_packcomp_verbose = 1;
        break;
    case 'a':
        input_args->arch = arg;
        break;
    case 'o':
        input_args->out_file = arg;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 2)
        {
            argp_usage(state);
        }
        input_args->branches[state->arg_num] = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 2)
        {
            argp_usage(state);
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char *argv[])
{
    InputArgs input_args;
    g_packcomp_verbose = 0;

    argp_parse(&argp, argc, argv, 0, 0, &input_args);
    // auto jsons = package_compare_all_archs(input_args.branches[0], input_args.branches[1]);

    auto branch1 = input_args.branches[0];
    auto branch2 = input_args.branches[1];

    dbuff<const char*> common_archs;
    if (!get_common_archs(branch1, branch2, &common_archs)) {
        fprintf(stderr, "fetching arches failed");
        return EXIT_FAILURE;
    }
    // parse archs
    dbuff<const char*> archs;
    if (input_args.arch != "") {
        darr<str> tokens; init(&tokens);
        split(&tokens, str{input_args.arch}, ',');

        darr<const char*> b; init(&b, len(tokens));
        for (int i = 0; i < len(tokens); i++) {
            const char *s = to_c_str(tokens[i]);

            if (find(begin(common_archs), end(common_archs), s, 
                [](auto s1, auto s2) {return strcmp(s1, s2) == 0;}) != end(common_archs)) 
            {
                push(&b, s);
            } else {
                fprintf(stderr, "there is no such arch: %s\n", s);
            }
        }

        shrink_to_fit(&b);
        archs = to_dbuff(b);
    } else {
        archs = common_archs;
    }

    if (g_packcomp_verbose)
        print("archs: ", archs, "\n");

    auto jsons = package_compare(branch1, branch2, archs, compare_sorted);


    FILE* ofile = stdout;
    if (input_args.out_file != "") {
        ofile = fopen(input_args.out_file, "w");
    }

    // fprintf(ofile, "arch: %s\n", json_object_get_string(json_object_object_get(jsons[i], "arch")));
    for (int i=0; i < len(jsons); i++) {
        fprintf(ofile, "%s\n", json_object_to_json_string(jsons[i]));
    }

    if (input_args.out_file != "") {
        fclose(ofile);
    }
}
