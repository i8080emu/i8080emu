#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <argp.h>

#include "asm.h"

/* virtual memory to asm (64 Kb) */
char vm[0x10000];
int error_count = 0;

/* level of debugging. 0 - none, 3 - max verbose */
int DEBUG_LEVEL = 0;

struct arguments arguments;

const char *argp_program_version = "argp-ex3 1.0";
const char *argp_program_bug_address = "<bug-gnu-utils@gnu.org>";

     /* Program documentation. */
static char doc[] =
  "Argp example #3 -- a program with options and arguments using argp";

     /* A description of the arguments we accept. */
static char args_doc[] = "FILENAME";

     /* The options we understand. */
static struct argp_option options[] = {
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"quiet", 'q', 0, 0, "Don't produce any output"},
  {"silent", 's', 0, OPTION_ALIAS},
  {"output", 'o', "FILE", 0, "Output to FILE instead of standard output"},
  {"preprocess-only", 'p', 0, 0, "Only preprocess lines"},
  {"only-one-pass", 'n', 0, 0, "Only pass one, don't generate code"},
  {"debug-level", 'd', "LEVEL", 0, "Level of debugging"},
  {0}
};

     /* Used by `main' to communicate with `parse_opt'. */
struct arguments
{
  char *input_file;		/* FILENAME */
  int silent, verbose;
  int preprocess_only, onepass;
  char *output_file;
  int debug_level;		/* LEVEL */
};
     /* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the INPUT argument from `argp_parse', which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'q':
    case 's':
      arguments->silent = 1;
      break;
    case 'v':
      arguments->verbose = 1;
      break;
    case 'o':
      arguments->output_file = arg;
      break;
    case 'p':
      arguments->preprocess_only = 1;
    case 'n':			/* preprocess only - onepass too */
      arguments->onepass = 1;
      break;
    case 'd':
      arguments->debug_level = atoi (arg);
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 1)
	/* Too many arguments. */
	argp_usage (state);

      arguments->input_file = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 1)
	/* Not enough arguments. */
	argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };




/* do one asm pass (read and processing lines) */
void
do_pass (ASMSESSION * as, FILE * fpin)
{
  asmerr_t err;
  char buf[4096];
  char *eol;
  int line_num = 1;

  while (fgets(buf, sizeof(buf), fpin) > 0)
    {
      /* strip end of line */
      if ((eol = strstr (buf, "\n")) != NULL)
		  *eol = '\0';

      err = asm_process (as, buf);
      if (err != ASMERR_NOERROR)
	{
	  fprintf (stderr, "%s:%d: error: %s\n", arguments.output_file, line_num,
		   asm_get_error_text (err));
	  error_count++;
	}
      line_num++;
    }
}


int
main (int argc, char **argv)
{
  /* Default values. */
  arguments.silent = 0;
  arguments.verbose = 0;
  arguments.output_file = "a.bin";
  arguments.preprocess_only = 0;
  arguments.onepass = 0;
  arguments.debug_level = 0;

  /* Assembler session variables */
  ASMSESSION as;
  FILE *fpin, *fout;
  int start, len;


  /* Parse our arguments; every option seen by `parse_opt' will
     be reflected in `arguments'. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  DEBUG_LEVEL = arguments.debug_level;

  asm_start (&as, &vm);

  if (arguments.preprocess_only)
    as.prep_only = 1;


  /* PASS 1 */

  /* attemp to open source file */
  if ((fpin = fopen (arguments.input_file, "r")) == NULL)
    {
      fprintf (stderr, "i8080asm: Can't open file %s\n",
	       arguments.input_file);
      exit (1);
    }
  else
    do_pass (&as, fpin);

  /* if preprocessing only, exit */
  if (arguments.preprocess_only)
    exit (0);

  /* if one pass defined or error occured, exit */
  if (error_count > 0 || arguments.onepass)
    {
      goto end;
    }

  /* PASS 2 */
  printf ("%s: Pass 2, code generation...\n", arguments.input_file);
  rewind (fpin);
  asm_pass2 (&as);
  do_pass (&as, fpin);

end:

  start = as.org;
  len = as.addr - as.org;

  /* free asm session */
  asm_end (&as);
  fclose (fpin);

  // выводим результат
  if (error_count > 0)
    {
      fprintf (stderr, "%s: Found %d error(s)\n", arguments.input_file, error_count);
      return 1;
    }
  else
    {
      fprintf (stdout, "%s: Assemble OK.\n", arguments.input_file);
      fprintf (stdout,
	       "%s: Start address %#4x, lenght %#4x.\n", arguments.input_file, start,
	       len);

      /* Save result in outfile */
      fout = fopen (arguments.output_file, "wb");
      if (fout == NULL)
	{
	  fprintf (stderr, "i8080asm: error: Can't create out file.\n");
	  exit (1);
	}

      fwrite (&vm[start], len, 1, fout);
      fclose (fout);

      return 0;
    }
}
