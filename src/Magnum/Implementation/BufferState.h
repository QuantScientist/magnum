#ifndef Magnum_Implementation_BufferState_h
#define Magnum_Implementation_BufferState_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "Magnum/Buffer.h"

namespace Magnum { namespace Implementation {

struct BufferState {
    #ifndef MAGNUM_TARGET_GLES
    static const std::size_t TargetCount = 13+1;
    #elif !defined(MAGNUM_TARGET_GLES2) && defined(MAGNUM_TARGET_WEBGL)
    static const std::size_t TargetCount = 8+1;
    #elif !defined(MAGNUM_TARGET_GLES2)
    static const std::size_t TargetCount = 12+1;
    #else
    static const std::size_t TargetCount = 2+1;
    #endif

    /* Target <-> index mapping */
    static std::size_t indexForTarget(Buffer::TargetHint target);
    static const Buffer::TargetHint targetForIndex[TargetCount-1];

    explicit BufferState(Context& context, std::vector<std::string>& extensions);

    void reset();

    #ifndef MAGNUM_TARGET_GLES2
    void(*bindBasesImplementation)(Buffer::Target, UnsignedInt, Containers::ArrayReference<Buffer* const>);
    void(*bindRangesImplementation)(Buffer::Target, UnsignedInt, Containers::ArrayReference<const std::tuple<Buffer*, GLintptr, GLsizeiptr>>);
    void(*copyImplementation)(Buffer&, Buffer&, GLintptr, GLintptr, GLsizeiptr);
    #endif
    void(Buffer::*createImplementation)();
    void(Buffer::*getParameterImplementation)(GLenum, GLint*);
    #ifndef MAGNUM_TARGET_GLES2
    void(Buffer::*getSubDataImplementation)(GLintptr, GLsizeiptr, GLvoid*);
    #endif
    void(Buffer::*dataImplementation)(GLsizeiptr, const GLvoid*, BufferUsage);
    void(Buffer::*subDataImplementation)(GLintptr, GLsizeiptr, const GLvoid*);
    void(Buffer::*invalidateImplementation)();
    void(Buffer::*invalidateSubImplementation)(GLintptr, GLsizeiptr);
    #ifndef MAGNUM_TARGET_WEBGL
    void*(Buffer::*mapImplementation)(Buffer::MapAccess);
    void*(Buffer::*mapRangeImplementation)(GLintptr, GLsizeiptr, Buffer::MapFlags);
    void(Buffer::*flushMappedRangeImplementation)(GLintptr, GLsizeiptr);
    bool(Buffer::*unmapImplementation)();
    #endif

    /* Currently bound buffer for all targets */
    GLuint bindings[TargetCount];

    /* Limits */
    #ifndef MAGNUM_TARGET_GLES2
    GLint
        #ifndef MAGNUM_TARGET_GLES
        minMapAlignment,
        #endif
        #ifndef MAGNUM_TARGET_WEBGL
        maxAtomicCounterBindings,
        maxShaderStorageBindings,
        shaderStorageOffsetAlignment,
        #endif
        uniformOffsetAlignment,
        maxUniformBindings;
    #endif
};

}}

#endif
