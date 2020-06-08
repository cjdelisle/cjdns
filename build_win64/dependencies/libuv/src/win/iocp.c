/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <assert.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "uv.h"
#include "internal.h"
#include "handle-inl.h"
#include "stream-inl.h"
#include "req-inl.h"

void uv_iocp_endgame(uv_loop_t* loop, uv_iocp_t* handle) {
  uv__handle_close(handle);
}

int uv_iocp_close(uv_loop_t* loop, uv_iocp_t* handle) {
  uv_iocp_stop(handle);
  uv_want_endgame(loop, (uv_handle_t*) handle);
  uv__handle_closing(handle);
  return 0;
}

void uv_process_iocp_req(uv_loop_t* loop, uv_req_t* req) {
  uv_iocp_t* handle = (uv_iocp_t*) req->data;
  if (handle->iocp_cb) {
    handle->iocp_cb(handle);
  }
}

int uv_iocp_stop(uv_iocp_t* handle) {
  if (!handle->activecnt) {
    return UV_EINVAL;
  }
  /* TODO(cjd): DeleteIoCompletionPort()? */
  handle->iocp_cb = NULL;
  UNREGISTER_HANDLE_REQ(handle->loop, handle, &handle->req);
}

int uv_iocp_start(uv_loop_t* loop,
                  uv_iocp_t* handle,
                  uv_os_file_t fd,
                  uv_iocp_cb cb) {
  NTSTATUS nt_status;
  IO_STATUS_BLOCK io_status;
  FILE_MODE_INFORMATION mode_info;

  if (handle->activecnt) {
    return UV_EINVAL;
  }

  /* Check if the handle was created with FILE_FLAG_OVERLAPPED. */
  nt_status = pNtQueryInformationFile(fd,
                                      &io_status,
                                      &mode_info,
                                      sizeof(mode_info),
                                      FileModeInformation);
  if (nt_status != STATUS_SUCCESS) {
    return uv_translate_sys_error(GetLastError());
  }

  if (mode_info.Mode & FILE_SYNCHRONOUS_IO_ALERT ||
      mode_info.Mode & FILE_SYNCHRONOUS_IO_NONALERT) {
    /* Not overlapped. */
    return UV_EINVAL;
  } else {
    /* Try to associate with IOCP. */
    if (!CreateIoCompletionPort(fd, loop->iocp, (ULONG_PTR)handle, 0)) {
      if (ERROR_INVALID_PARAMETER == GetLastError()) {
        // Already associated.
      } else {
        return uv_translate_sys_error(GetLastError());
      }
    }
  }

  memset(handle, 0, sizeof(uv_iocp_t));
  uv__handle_init(loop, (uv_handle_t*) handle, UV_IOCP);
  handle->handle = fd;
  handle->iocp_cb = cb;
  uv_req_init(loop, &handle->req);
  handle->req.data = handle;
  handle->req.type = UV_IOCP;
  handle->overlapped = &handle->req.overlapped;
  REGISTER_HANDLE_REQ(loop, handle, &handle->req);

  return 0;
}
