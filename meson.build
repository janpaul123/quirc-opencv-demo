project('tutorial', 'c', 'cpp', default_options : ['c_std=c11', 'cpp_std=c++11'])
src = ['demo.cpp', 'quirc-lib/decode.c', 'quirc-lib/identify.c', 'quirc-lib/quirc.c', 'quirc-lib/version_db.c']
opencv = dependency('opencv')
executable('demo', sources : src, dependencies : opencv)
