# Copyright (c) 2010 The ANGLE Project Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'target_defaults': {
    'defines': [
      'ANGLE',
      'OPENGL_ES',
    ],
  },
  'includes': [
  ],
  'targets': [
  ],
  'conditions': [
    ['OS=="win"', {
      'targets': [
        {
          'target_name': 'gltools',
          'type': 'static_library',
          'include_dirs': [
            'GLTools/include',
            '<(DEPTH)/third_party/angle/include',
          ],
          'dependencies': [
            '<(DEPTH)/third_party/angle/src/build_angle.gyp:libEGL',
            '<(DEPTH)/third_party/angle/src/build_angle.gyp:libGLESv2',
          ],
          'sources': [
            'GLTools/include/GLBatch.h',
            'GLTools/include/GLBatchBase.h',
            'GLTools/include/GLFrame.h',
            'GLTools/include/GLFrustum.h',
            'GLTools/include/GLMatrixStack.h',
            'GLTools/include/GLShaderManager.h',
            'GLTools/include/GLTools.h',
            'GLTools/include/GLTriangleBatch.h',
            'GLTools/include/math3d.h',
            'GLTools/include/StopWatch.h',
            'GLTools/src/GLBatch.cpp',
            #'GLTools/src/glew.c',
            'GLTools/src/GLShaderManager.cpp',
            'GLTools/src/GLTools.cpp',
            'GLTools/src/GLTriangleBatch.cpp',
            'GLTools/src/math3d.cpp',
          ],
          'direct_dependent_settings': {
            'include_dirs': [
              'GLTools/include',
              '<(DEPTH)/third_party/angle/include',
            ],
          },
        },
        {
          'target_name': 'examples',
          'type': 'executable',
          'dependencies': [
            '<(DEPTH)/third_party/esGLUT/esGLUT.gyp:esGLUT',
            '<(DEPTH)/third_party/cef3/cefclient.gyp:libcef_dll_wrapper',
            'gltools',
          ],
          'sources': [
            'examples.cpp',
          ],
          'copies': [
            {
              'destination': '<(PRODUCT_DIR)',
              'files': [
                '<(DEPTH)/third_party/cef3/binary/cefclient.exe',
                '<(DEPTH)/third_party/cef3/binary/ffmpegsumo.dll',
                '<(DEPTH)/third_party/cef3/binary/icudt.dll',
                '<(DEPTH)/third_party/cef3/binary/libcef.dll',
                #'<(DEPTH)/third_party/cef3/binary/libEGL.dll',
                #'<(DEPTH)/third_party/cef3/binary/libGLESv2.dll',
              ],
            },
          ],

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
