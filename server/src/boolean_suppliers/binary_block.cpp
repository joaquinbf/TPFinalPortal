#include "../../include/boolean_suppliers/binary_block.h"

BinaryBlock::BinaryBlock(const BooleanOperator *boolean_operator):
    boolean_operator(boolean_operator) {
}

BinaryBlock::~BinaryBlock() {
}

bool BinaryBlock::getAsBoolean() const {
    bool acum = this->booleans.at(0)->getAsBoolean();

    for (unsigned int i = 1; i < this->booleans.size(); i++) {
        bool a_boolean = this->booleans.at(i)->getAsBoolean();
        acum = this->boolean_operator->evualuate(acum, a_boolean);
    }

    return acum;
}

void BinaryBlock::pushBack(BooleanSupplier *boolean) {
    this->booleans.push_back(boolean);
}
