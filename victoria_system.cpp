#include "integer.hpp"
#include "floating.hpp"
#include "string.hpp"
int main(int argc, char * * argv) {
    Value * expression = new List(
        new Symbol(hash("%")), new List(
            new Integer(61), new List(
                new List(
                    new Symbol(hash("+")), new List(
                        new Integer(3), new List(
                            new Integer(5), (List *) 0
                        )
                    )
                ), (List *) 0
            )
        )
    );
    return dynamic_cast<Integer *>(expression->evaluate(integer_package()))->value;
};