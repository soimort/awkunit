#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "gawkapi.h"
#include "gettext.h"
#define _(msgid) gettext(msgid)
#define N_(msgid) msgid

#include "gawkfts.h"
#include "stack.h"

#define BUFFER_SIZE 256

int plugin_is_GPL_compatible;

static const gawk_api_t *api;
static awk_ext_id_t *ext_id;
static const char *ext_version = "AwkUnit: version 0.0.1";

static awk_bool_t (*init_func)(void) = NULL;

static awk_value_t *do_assertIO(int nargs, awk_value_t *result)
{
     awk_value_t scriptFile, inFile, outFile;
     int ret = -1;
     FILE *fpipe, *fo;
     char *command = NULL, pbuf[BUFFER_SIZE], obuf[BUFFER_SIZE];
     
     assert(result != NULL);
     
     if (do_lint && nargs != 3)
          lintwarn(ext_id,
                   _("assertIO: called with incorrect number of arguments, "
                     "expecting 3"));
     
     if (get_argument(0, AWK_STRING, &scriptFile) &&
         get_argument(1, AWK_STRING, &inFile) &&
         get_argument(2, AWK_STRING, &outFile)) {
          ret = 0;
     } else {
          fprintf(stderr, "Error: incorrect number of arguments\n");
          exit(-1);
     }
     command = (char *)malloc(scriptFile.str_value.len +
                              inFile.str_value.len + 12);
     strcpy(command, "gawk -f ");
     strcat(command, scriptFile.str_value.str);
     strcat(command, " < ");
     strcat(command, inFile.str_value.str);
     
     if (!(fpipe = (FILE *)popen(command, "r"))) {
          perror("Fatal error: cannot open pipe");
          exit(-1);
     }
     if (!(fo = fopen(outFile.str_value.str, "r"))) {
          perror("Fatal error: cannot open file");
          exit(-1);
     }
     
     while (fgets(pbuf, BUFFER_SIZE, fpipe)) {
          fgets(obuf, BUFFER_SIZE, fo);
          if (strcmp(pbuf, obuf) != 0) {
               fprintf(stderr, "Assertion failed: %s: output differs from file (%s)\n",
                       inFile.str_value.str, outFile.str_value.str);
               sym_update("_assert_exit", make_number(-1, result));
               pclose(fpipe);
               fclose(fo);
               exit(-1);
          }
     }
     
     pclose(fpipe);
     fclose(fo);
     return make_number(ret, result);
}

static awk_ext_func_t func_table[] = {
     {"assertIO", do_assertIO, 3},
};

dl_load_func(func_table, some_name, "name_space_in_quotes")
