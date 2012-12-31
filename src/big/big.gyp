# Copyright (c) 2010 The ANGLE Project Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'conditions': [
    ['OS=="win"', {
      'targets': [
        {
          'target_name': 'big',
          'type': 'static_library',
          'include_dirs': [
            '<(DEPTH)/third_party/cef3',
          ],
          'dependencies': [
            '<(DEPTH)/third_party/cef3/cefclient.gyp:libcef_dll_wrapper',
          ],
          'link_settings': {
            'libraries': [
              '<(DEPTH)/third_party/cef3/binary/libcef.lib',
            ],
          },
          'sources': [
            'browser/big_browser.h',
            'browser/big_browser.cpp',
            'browser/big_handler.h',
            'browser/big_handler.cpp',
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
            {
              'destination': '<(PRODUCT_DIR)/locales',
              'files': [
                '<(DEPTH)/third_party/cef3/binary/locales/en-US.pak',
                '<(DEPTH)/third_party/cef3/binary/locales/zh-CN.pak',
              ],
            },
          ],
          'direct_dependent_settings': {
            'include_dirs': [
              '<(DEPTH)/third_party/cef3',
            ],
            'msvs_settings': {
              'VCLinkerTool': {
                'DelayLoadDlls': [
                  'libcef.dll',
                ],
              }
            }
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
