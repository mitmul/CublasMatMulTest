TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

SOURCES += main.cpp


# CUDA
INCLUDEPATH += /Developer/NVIDIA/CUDA-5.0/include
LIBS += -L/Developer/NVIDIA/CUDA-5.0/lib \
  -lcublas \
  -lcublas_device \
  -lcudadevrt \
  -lcudart \
  -lcufft \
  -lcuinj_50_36 \
  -lcurand \
  -lcusparse \
  -lnpp \
  -lnvToolsExt \
  -ltlshook \
  -framework AppKit

# Eigen
INCLUDEPATH += /usr/local/include/eigen3

# boost
INCLUDEPATH += /usr/local/include
LIBS += /usr/local/lib/libboost_atomic-mt.dylib \
  /usr/local/lib/libboost_chrono-mt.dylib \
  /usr/local/lib/libboost_context-mt.dylib \
  /usr/local/lib/libboost_date_time-mt.dylib \
  /usr/local/lib/libboost_filesystem-mt.dylib \
  /usr/local/lib/libboost_graph-mt.dylib \
  /usr/local/lib/libboost_graph_parallel-mt.dylib \
  /usr/local/lib/libboost_iostreams-mt.dylib \
  /usr/local/lib/libboost_locale-mt.dylib \
  /usr/local/lib/libboost_math_c99-mt.dylib \
  /usr/local/lib/libboost_math_c99f-mt.dylib \
  /usr/local/lib/libboost_math_c99l-mt.dylib \
  /usr/local/lib/libboost_math_tr1-mt.dylib \
  /usr/local/lib/libboost_math_tr1f-mt.dylib \
  /usr/local/lib/libboost_math_tr1l-mt.dylib \
  /usr/local/lib/libboost_mpi-mt.dylib \
  /usr/local/lib/libboost_mpi_python-mt.dylib \
  /usr/local/lib/libboost_prg_exec_monitor-mt.dylib \
  /usr/local/lib/libboost_program_options-mt.dylib \
  /usr/local/lib/libboost_python-mt.dylib \
  /usr/local/lib/libboost_random-mt.dylib \
  /usr/local/lib/libboost_regex-mt.dylib \
  /usr/local/lib/libboost_serialization-mt.dylib \
  /usr/local/lib/libboost_signals-mt.dylib \
  /usr/local/lib/libboost_system-mt.dylib \
  /usr/local/lib/libboost_thread-mt.dylib \
  /usr/local/lib/libboost_timer-mt.dylib \
  /usr/local/lib/libboost_unit_test_framework-mt.dylib \
  /usr/local/lib/libboost_wave-mt.dylib \
  /usr/local/lib/libboost_wserialization-mt.dylib
