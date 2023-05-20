add_rules("mode.release")

PROJECT_NAME = 'MovingLight'
BDS_DIRECTORY = '../../'

target(PROJECT_NAME)
    set_kind('shared')
    set_symbols('debug')
    set_languages('c17','c++17')
    set_basename(PROJECT_NAME)
    set_optimize('fastest')
    add_files('src/*.cpp')
    set_targetdir(BDS_DIRECTORY..'plugins/')
    add_cxflags('-utf-8')
    add_linkdirs(
        BDS_DIRECTORY,
        BDS_DIRECTORY..'SDK/Lib'
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
        BDS_DIRECTORY..'SDK/include',
        'src',
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