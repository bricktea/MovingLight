add_rules("mode.release")

PROJECT_NAME = 'MovingLight'

target(PROJECT_NAME)
set_kind('shared')
set_symbols('debug')
set_languages('c99', 'c++20')
set_basename(PROJECT_NAME)
set_optimize('fastest')
add_files('src/*.cpp')
add_cxflags('-utf-8')
add_linkdirs(
    'sdk/Lib'
)
add_links(
    'bedrock_server_api',
    'bedrock_server_var',
    'SymDBHelper',
    'LiteLoader',
    'Delayimp')
add_shflags(
    '/DELAYLOAD:bedrock_server.dll',
    '/NODEFAULTLIB:msvcrt.lib')
add_includedirs(
    'sdk/include',
    'include')
add_defines(
    'NDEBUG',
    'WIN32_LEAN_AND_MEAN',
    '_CRT_SECURE_NO_WARNINGS',
    '_WINDOWS',
    '_USRDLL',
    '_AMD64_',
    'NOMINMAX',
    'UNICODE')
