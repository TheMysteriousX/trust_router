/*
 * Copyright (c) 2012, JANET(UK)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of JANET(UK) nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>

#include <tid.h>

int tids_req_handler (TIDS_INSTANCE * tids,
		      TID_REQ *req, 
		      TID_RESP *resp,
		      void *cookie)
{
  printf("Request received! Realm = %s, Comm = %s\n", req->realm->buf, req->comm->buf);
  if (tids)
    tids->req_count++;

  if ((NULL == (resp->realm = tr_dup_name(req->realm))) ||
      (NULL == (resp->comm = tr_dup_name(req->comm)))) {
    printf ("Error in tid_dup_name, not responding.\n");
    return 1;
  }

  return 0;
}


int main (int argc, 
	  const char *argv[]) 
{
  static TIDS_INSTANCE *tids;
  int rc = 0;

  /* Parse command-line arguments */ 
  if (argc != 1)
    printf("Unexpected arguments, ignored.\n");

  /* Create a TID server instance */
  if (NULL == (tids = tids_create())) {
    printf("Error in tids_create().  Exiting.\n");
    return 1;
  }

  /* Start-up the server, won't return unless there is an error. */
  rc = tids_start(tids, &tids_req_handler , NULL);
  
  printf("Error in tids_start(), rc = %d. Exiting.\n");

  /* Clean-up the TID server instance */
  tids_destroy(tids);

  return 1;
}
