#ifndef __BOOLEAN_BLOCK_FACTORY_TEST_H__
#define __BOOLEAN_BLOCK_FACTORY_TEST_H__

#include "../../server/include/boolean_suppliers/boolean_block_factory.h"
#include <cxxtest/TestSuite.h>

class BooleanBlockFactoryTest: public CxxTest::TestSuite {
public:
    void testCreaUnBlockYNoPierdeMemoria() {
        //Test para probar con valrind.
        BooleanBlockFactory bbf;
        BooleanBlock *block = bbf.createAndBlock();

        // para que no de error de variable sin usar.
        block++;

        // Al eliminarse la fabrica se debe elimnar los blockes pedidos.
    }
};

#endif
