# odb17

odb started as an Object Database. The planned functionaliy forced the
development into the ***Graph Database*** direction, where it finaly arrived.

The ***17*** stands for the C++ version vecessary ti have a compiler to compile it.

Here some simple samples

**C++ Code**
```cpp
#include <iostream>

#include "odb.h"
#include "atom.h"
#include "thing.h"

int main()
       {
       auto oOdb    = odb::COdb();
       auto pThing1 = oOdb.MakeThing("Ulrich");
       auto pThing2 = oOdb.MakeThing("Fred");
       auto pAtom1  = oOdb.MakeAtom("Leader", "Role");
       auto pAtom2  = oOdb.MakeAtom("Member", "Role");
       auto pReason = oOdb.MakeReason("pays");
       pThing1->Append(pAtom1);
       pThing2->Append(pAtom2);
       pThing1->Link(pThing2, pReason);
       oOdb.print_json(std::cout);
       }
```
 **Output**

```json
    {
    "Object Database Dump":
        {
        "Sizes": [ {"P": 0},{"A": 2},{"R": 1},{"T": 2} ],
        "Properties":
            [
            ],
        "Atoms":
            [
                { "id": 0, "data": "Leader" },
                { "id": 1, "data": "Member" }
            ],
        "Reasons":
            [
                { "id": 0, "name": "pays" }
            ],
        "Things":
            [
                { "id": 0, "name": "Ulrich",
                    "properties": [  ],
                    "atoms": [ {"id": 0} ],
                    "links": [ {"thing-id": 1, "reason-id": 0} ] },
                { "id": 1, "name": "Fred",
                    "properties": [  ],
                    "atoms": [ {"id": 1} ],
                    "links": [  ] }
            ]
        }
     }
```

