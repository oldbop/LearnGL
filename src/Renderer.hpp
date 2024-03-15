#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cassert>

#define GL_CALL(x) gl_clear_error(); x; assert(gl_log_call())

void gl_clear_error(void);
bool gl_log_call(void);

#endif
