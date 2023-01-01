#include <stdlib.h>
#include "mongoose.h"

static struct mg_str access_token;

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev != MG_EV_HTTP_MSG) {
    return;
  }

  struct mg_http_message *hm = (struct mg_http_message *) ev_data;  
  struct mg_str *user_token = mg_http_get_header(hm, "X-Auth-Token");
  if (user_token == NULL) {
    mg_http_reply(c, 401, "", "%s\n", "Not authenticated");
    return;
  }

  if(mg_strcmp(access_token, *user_token) != 0) {
    mg_http_reply(c, 403, "", "%s\n", "Not authorized");
    return;
  } 

  mg_http_reply(c, 200, "", "%s\n", "It works!");
}

int main(void) {
  access_token = mg_str("DT_RVhBTVBMRSBERU1PIFRPS0VOEXAMPLETOKEN");

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0:8888", fn, &mgr);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);
  return EXIT_SUCCESS;
}

