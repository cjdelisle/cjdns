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
#ifndef _WIN32

/* This test makes no sense outside the context of windows. */

#else

#include "uv.h"
#include "task.h"

#include <fcntl.h>
#include <direct.h>
#include <io.h>
#define unlink _unlink

int iocp_cb_count;

static uv_loop_t* loop;

static uv_fs_t open_req;
static uv_fs_t write_req;
static uv_fs_t close_req;
static uv_iocp_t iocp_handle;

static char test_buf[] = "test-buffer\n";

static void iocp_cb(uv_iocp_t* handle)
{
  iocp_cb_count++;
  uv_stop(loop);
}

TEST_IMPL(iocp) {
  int r;
  HANDLE file;

  unlink("test_file");

  loop = uv_default_loop();

  r = uv_fs_open(loop, &open_req, "test_file", O_WRONLY | O_CREAT,
      S_IWUSR | S_IRUSR, NULL);
  ASSERT(r >= 0);
  ASSERT(open_req.result >= 0);
  uv_fs_req_cleanup(&open_req);

  r = uv_fs_write(loop, &write_req, open_req.result, test_buf,
      sizeof(test_buf), -1, NULL);
  ASSERT(r >= 0);
  ASSERT(write_req.result >= 0);
  uv_fs_req_cleanup(&write_req);

  r = uv_fs_close(loop, &close_req, open_req.result, NULL);
  ASSERT(r == 0);
  ASSERT(close_req.result == 0);
  uv_fs_req_cleanup(&close_req);

  /* Test. */

  file = CreateFile("test_file",
                    GENERIC_READ | GENERIC_WRITE,
                    0,
                    0,
                    OPEN_EXISTING,
                    FILE_FLAG_OVERLAPPED,
                    0);

  uv_iocp_start(loop, &iocp_handle, file, iocp_cb);

  if (!ReadFile(file,
                test_buf,
                sizeof(test_buf),
                NULL,
                (OVERLAPPED*) iocp_handle.overlapped)) {
    ASSERT(GetLastError() == ERROR_IO_PENDING);
  }

  uv_run(loop, UV_RUN_DEFAULT);
  ASSERT(iocp_cb_count == 1);

  CloseHandle(file);
  unlink("test_file");
  MAKE_VALGRIND_HAPPY();
  return 0;
}

#endif
