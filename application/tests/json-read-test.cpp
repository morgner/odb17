#include <iostream>
#include <fstream>

//#include <json/value.h>
#include <json/json.h>
//#include <json/reader.h>

// allocator.h   assertions.h  autolink.h    config.h      features.h    forwards.h    json.h        reader.h      value.h       version.h     writer.h


#include "odb.h"
#include "atom.h"
#include "thing.h"
#include "property.h"
#include "generic.h"

/*
int main() {
    ifstream ifs("alice.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj); // reader can also read strings
    cout << "Book: " << obj["book"].asString() << endl;
    cout << "Year: " << obj["year"].asUInt() << endl;
    const Json::Value& characters = obj["characters"]; // array of characters
    for (int i = 0; i < characters.size(); i++){
        cout << "    name: " << characters[i]["name"].asString();
        cout << " chapter: " << characters[i]["chapter"].asUInt();
        cout << endl;
        }
    }
*/

odb::COdb oOdb;

int main()
    {
    Json::Value json;
    std::ifstream db_dump("db.json", std::ifstream::binary);
    db_dump >> json;

    const Json::Value & properties = json["Object Database Dump"]["Properties"];
    std::cout << "odb read in " << properties.size() << " properties.\n";

    const Json::Value & atoms = json["Object Database Dump"]["Atoms"];
    std::cout << "odb read in " << atoms.size() << " atoms.\n";

    const Json::Value & reasons = json["Object Database Dump"]["Reasons"];
    std::cout << "odb read in " << reasons.size() << " reasons.\n";

    const Json::Value & things = json["Object Database Dump"]["Things"];
    std::cout << "odb read in " << things.size() << " things.\n";

    // Iterate over the sequence elements.
    for ( size_t index = 0; index < properties.size(); ++index )
        {
        auto nId   = properties[(int)index].get("id",    0).asUInt();
        auto sName = properties[(int)index].get("name", "").asString();
        oOdb.MakeProperty(/*nId, */sName);
//        std::cout << "Property ID: " << nId << ", name: " << sName << '\n';
	}
    // Iterate over the sequence elements.
    for ( size_t index = 0; index < atoms.size(); ++index )
        {
        auto nId     = atoms[(int)index].get("id",      0).asUInt();
        auto sName   = atoms[(int)index].get("name",   "").asString();
        auto sPrefix = atoms[(int)index].get("prefix", "").asString();
        auto sSuffix = atoms[(int)index].get("suffix", "").asString();
        auto sFormat = atoms[(int)index].get("format", "").asString();
        auto sData   = atoms[(int)index].get("data",   "").asString();
        oOdb.MakeAtom(/*nId, */sData, sName, sPrefix, sSuffix, sFormat);
//        std::cout << "Atom ID: " << nId << ", name: " << sName << '\n';
	}
    // Iterate over the sequence elements.
    for ( size_t index = 0; index < reasons.size(); ++index )
        {
        auto nId   = reasons[(int)index].get("id",    0).asUInt();
        auto sName = reasons[(int)index].get("name", "").asString();
        oOdb.MakeReason(/*nId, */sName);
//        std::cout << "Reason ID: " << nId << ", name: " << sName << '\n';
	}
    // Iterate over the sequence elements.
    for ( size_t index = 0; index < things.size(); ++index )
        {
        auto nId   = things[(int)index].get("id",    0).asUInt();
        auto sName = things[(int)index].get("name", "").asString();

	auto pThing = oOdb.MakeThing(/*nId, */sName);
        }
    for ( size_t index = 0; index < things.size(); ++index )
        {
        auto nId   = things[(int)index].get("id",    0).asUInt();

        const Json::Value & p = things[(int)index]["properties"];
        for ( size_t i = 0; i < p.size(); ++i )
            {
            auto nPId = p[(int)i].get("id", 0).asUInt();
	    oOdb.AppendProperty2Thing( nId, nPId );
            }

        const Json::Value & a = things[(int)index]["atoms"];
        for ( size_t i = 0; i < a.size(); ++i )
            {
            auto nAId = a[(int)i].get("id", 0).asUInt();
            oOdb.AppendAtom2Thing( nId, nAId );
            }

        const Json::Value & l = things[(int)index]["links"];
        for ( size_t i = 0; i < l.size(); ++i )
            {
            auto nTId = l[(int)i].get("thing-id",  0).asUInt();
            auto nRId = l[(int)i].get("reason-id", 0).asUInt();
            oOdb.LinkThing2Thing( nId, nTId, nRId );
            }
//        std::cout << "Thing ID: " << nId << ", name: " << sName << '\n';
	}
    oOdb.print_json(std::cout);
    }
/*
{
    "Object Database Dump": 
        {
        "Properties": [ { "id": "0", "name": "Person" } ]
        "Atoms": [ { "id": "0", "name": "round", "suffix": "%", "data": "100.2" } ]
        "Reasons": [ { "id": "0", "name": "made" } ]
        "Things": 
            [
                { "id": 0, "name": "Wundertüte",
                    "properties": [ {"id": "0"},{"id": "1"} ],
                    "atoms": [ {"id": "13"},{"id": "14"} ],
                    "links": [ {"thing-id": "6", "reason-id": "3"} ] },
            ]
        }
}
*/
