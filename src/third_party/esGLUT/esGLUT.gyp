# Copyright (c) 2010 The ANGLE Project Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [
  ],
  'conditions': [
    ['OS=="win"', {
      'targets': [
        {
          'target_name': 'esGLUT',
          'type': 'static_library',
          'include_dirs': [
            'include/',
          ],
          'sources': [
            'include/esGLUT/esGLUT.h',
            'src/esGLUT.c',
            'src/esGLUT_get.c',
            'src/esGLUT_internal.h',
            'src/esGLUT_utils.c',
            'src/esGLUT_win32.c',
          ],
          'direct_dependent_settings': {
            'include_dirs': [
              'include/',
            ],
          },
        },
      ],
    }],
  ],
}

# Local Variables:
# tab-width:2
# indent-tabs-mode:nil
# End:
# vim: set expandtab tabstop=2 shiftwidth=2:
