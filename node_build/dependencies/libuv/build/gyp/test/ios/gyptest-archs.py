#!/usr/bin/env python

# Copyright (c) 2013 Google Inc. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""
Verifies that device and simulator bundles are built correctly.
"""

import collections
import plistlib
import TestGyp
import os
import re
import struct
import subprocess
import sys
import tempfile


def BuildExpected(file, archs):
  if len(archs) == 1:
    return 'Non-fat file: %s is architecture: %s' % (file, archs[0])
  return 'Architectures in the fat file: %s are: %s' % (file, ' '.join(archs))


def CheckFileType(test, file, archs):
  proc = subprocess.Popen(['lipo', '-info', file], stdout=subprocess.PIPE)
  o = proc.communicate()[0].strip()
  assert not proc.returncode
  if len(archs) == 1:
    pattern = re.compile('^Non-fat file: (.*) is architecture: (.*)$')
  else:
    pattern = re.compile('^Architectures in the fat file: (.*) are: (.*)$')
  match = pattern.match(o)
  if match is None:
    print 'Ouput does not match expected pattern: %s' % (pattern.pattern)
    test.fail_test()
  else:
    found_file, found_archs = match.groups()
    if found_file != file or set(found_archs.split()) != set(archs):
      print 'Expected file %s with arch %s, got %s with arch %s' % (
          file, ' '.join(archs), found_file, ' '.join(found_archs))
      test.fail_test()


def XcodeVersion():
  xcode, build = GetStdout(['xcodebuild', '-version']).splitlines()
  xcode = xcode.split()[-1].replace('.', '')
  xcode = (xcode + '0' * (3 - len(xcode))).zfill(4)
  return xcode


def GetStdout(cmdlist):
  proc = subprocess.Popen(cmdlist, stdout=subprocess.PIPE)
  o = proc.communicate()[0].strip()
  assert not proc.returncode
  return o


if sys.platform == 'darwin':
  test = TestGyp.TestGyp(formats=['ninja', 'xcode'])

  test_cases = [
    ('Default', 'TestNoArchs', ['i386']),
    ('Default', 'TestArch32Bits', ['i386']),
    ('Default', 'TestArch64Bits', ['x86_64']),
    ('Default', 'TestMultiArchs', ['i386', 'x86_64']),
    ('Default-iphoneos', 'TestNoArchs', ['armv7']),
    ('Default-iphoneos', 'TestArch32Bits', ['armv7']),
    ('Default-iphoneos', 'TestArch64Bits', ['arm64']),
    ('Default-iphoneos', 'TestMultiArchs', ['armv7', 'arm64']),
  ]

  xcode_version = XcodeVersion()
  test.run_gyp('test-archs.gyp', chdir='app-bundle')
  for configuration, target, archs in test_cases:
    is_64_bit_build = ('arm64' in archs or 'x86_64' in archs)
    is_device_build = configuration.endswith('-iphoneos')

    kwds = collections.defaultdict(list)
    if test.format == 'xcode' and is_device_build:
      configuration, sdk = configuration.split('-')
      kwds['arguments'].extend(['-sdk', sdk])

    # TODO(sdefresne): remove those special-cases once the bots have been
    # updated to use a more recent version of Xcode.
    if xcode_version < '0500':
      if is_64_bit_build:
        continue
      if test.format == 'xcode':
        arch = 'i386'
        if is_device_build:
          arch = 'armv7'
        kwds['arguments'].extend(['-arch', arch])

    test.set_configuration(configuration)
    filename = '%s.bundle/%s' % (target, target)
    test.build('test-archs.gyp', target, chdir='app-bundle', **kwds)
    result_file = test.built_file_path(filename, chdir='app-bundle')

    test.must_exist(result_file)
    CheckFileType(test, result_file, archs)

  test.pass_test()
