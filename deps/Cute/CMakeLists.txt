project(CUTE)

add_library(Cute
  include/Cute/Vec3f.h
  src/Vec3f.c
)

target_include_directories(Cute PUBLIC include/)
