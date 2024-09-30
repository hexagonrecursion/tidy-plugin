cmake_path(APPEND STR_TO_PATH_CHECK_PATH "${PROJECT_BINARY_DIR}" "${CMAKE_SHARED_MODULE_PREFIX}StrToPathCheck${CMAKE_SHARED_MODULE_SUFFIX}")
configure_file(lit.cfg.py.in lit.cfg.py)
