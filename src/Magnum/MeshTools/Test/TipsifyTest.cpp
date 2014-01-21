/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014
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

#include <Corrade/TestSuite/Tester.h>

#include "Magnum/Magnum.h"
#include "Magnum/MeshTools/Tipsify.h"

namespace Magnum { namespace MeshTools { namespace Test {

class TipsifyTest: public TestSuite::Tester {
    public:
        TipsifyTest();

        void buildAdjacency();
        void tipsify();

    private:
        std::vector<UnsignedInt> indices;
        std::size_t vertexCount;
};

/*

 0 ----- 1 ----- 2 ----- 3
  \ 0  /  \ 7  /  \ 2  /  \
   \  / 11 \  / 13 \  / 12 \
    4 ----- 5 ----- 6 ----- 7
   /  \ 3  /  \ 8  /  \ 5  /
  / 14 \  / 9  \  / 15 \  /
 8 ----- 9 ---- 10 ---- 11          18 ---- 17
  \ 4  /  \ 1  /  \ 17 /  \           \ 18  /
   \  / 16 \  / 10 \  / 6  \           \  /
    12 ---- 13 ---- 14 ---- 15          16

*/

/* MSVC 2013 can't cope with only {} here */
TipsifyTest::TipsifyTest(): indices({
    4, 1, 0,
    10, 9, 13,
    6, 3, 2,
    9, 5, 4,
    12, 9, 8,
    11, 7, 6,

    14, 15, 11,
    2, 1, 5,
    10, 6, 5,
    10, 5, 9,
    13, 14, 10,
    1, 4, 5,

    7, 3, 6,
    6, 2, 5,
    9, 4, 8,
    6, 10, 11,
    13, 9, 12,
    14, 11, 10,

    16, 17, 18
}), vertexCount(19) {
    addTests<TipsifyTest>({&TipsifyTest::buildAdjacency,
              &TipsifyTest::tipsify});
}

void TipsifyTest::buildAdjacency() {
    std::vector<UnsignedInt> liveTriangleCount, neighborOffset, neighbors;
    Implementation::Tipsify(indices, vertexCount).buildAdjacency(liveTriangleCount, neighborOffset, neighbors);

    CORRADE_COMPARE(liveTriangleCount, (std::vector<UnsignedInt>{
        1, 3, 3, 2,
        4, 6, 6, 2,
        2, 6, 6, 4,
        2, 3, 3, 1,
        1, 1, 1
    }));

    CORRADE_COMPARE(neighborOffset, (std::vector<UnsignedInt>{
        0, 1, 4, 7,
        9, 13, 19, 25,
        27, 29, 35, 41,
        45, 47, 50, 53,
        54, 55, 56, 57
    }));

    CORRADE_COMPARE(neighbors, (std::vector<UnsignedInt>{
        0,
        0, 7, 11,
        2, 7, 13,
        2, 12,

        0, 3, 11, 14,
        3, 7, 8, 9, 11, 13,
        2, 5, 8, 12, 13, 15,
        5, 12,

        4, 14,
        1, 3, 4, 9, 14, 16,
        1, 8, 9, 10, 15, 17,
        5, 6, 15, 17,

        4, 16,
        1, 10, 16,
        6, 10, 17,
        6,

        18, 18, 18
    }));
}

void TipsifyTest::tipsify() {
    MeshTools::tipsify(indices, vertexCount, 3);

    CORRADE_COMPARE(indices, (std::vector<UnsignedInt>{
        4, 1, 0,
        9, 5, 4,
        1, 4, 5,
        9, 4, 8,
        12, 9, 8,
        13, 9, 12,
        10, 9, 13,
        13, 14, 10,
        10, 6, 5,
        10, 5, 9,
        6, 10, 11,
        14, 11, 10,
        6, 3, 2,
        11, 7, 6,
        7, 3, 6,
        6, 2, 5,
        2, 1, 5,
        14, 15, 11, /* from dead-end vertex stack */
        16, 17, 18 /* arbitrary vertex */
    }));
}

}}}

CORRADE_TEST_MAIN(Magnum::MeshTools::Test::TipsifyTest)