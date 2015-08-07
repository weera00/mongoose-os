#include "smartjs.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sj_hal.h>
#include <sj_v7_ext.h>
#include <sj_conf.h>
#include <string.h>

struct v7 *v7;

/* TODO(lsm): implement this */
int sj_http_call(struct v7 *v7, const char *url, const char *body,
                 size_t body_len, const char *method, v7_val_t cb) {
  return 0;
}

void init_conf(struct v7 *v7) {
  /* TODO(alashkin): make the filename overridable */
  const char *conf_file_name = "smartjs.conf";
  FILE *fconf = NULL;
  struct stat st;
  char *buf = NULL;

  if ((fconf = fopen(conf_file_name, "r")) == NULL ||
      stat(conf_file_name, &st) == -1) {
    printf("Cannot open %s\n", conf_file_name);
    return;
  }

  buf = calloc(st.st_size + 1, 1);

  if (fread(buf, 1, st.st_size, fconf) != st.st_size) {
    printf("Cannot read %s\n", conf_file_name);
    goto cleanup;
  }

  sj_init_conf(v7, buf);

cleanup:
  if (fconf != NULL) {
    fclose(fconf);
  }

  if (buf != NULL) {
    free(buf);
  }
}

void init_v7() {
  struct v7_create_opts opts = {0, 0, 0};

  v7 = v7_create_opt(opts);

  sj_init_v7_ext(v7);
  init_conf(v7);
}
