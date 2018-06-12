
#include "datahub.h"
#include <limits>
#include <cassert>

#define CHECK(x) assert(x)

namespace datahub {
    decltype(unittest) unittest;
    
    void runUnitTests() {
        // Initial value test
        
        CHECK(unittest.teststring == "unittest");
        CHECK(unittest.testvalue == 99);
        CHECK(unittest.testbool == true);
    
        // EventHandler tests
        {
            int firedEventCount = 0;
            datahub::EventToken token0 = unittest.teststring.onValueChanged += [&firedEventCount](const char *str) {
                firedEventCount++;
            };
            datahub::EventToken token1 = unittest.teststring.onValueChanged += [&firedEventCount](const char *str) {
                firedEventCount++;
            };

            CHECK(firedEventCount == 0);
            unittest.teststring = "aaa";
            CHECK(firedEventCount == 2);
            unittest.teststring.onValueChanged -= token0;
            unittest.teststring = "bbb";
            CHECK(firedEventCount == 3);
            unittest.teststring.onValueChanged -= token1;
            unittest.teststring = "ccc";
            CHECK(firedEventCount == 3);
        }
        
        // Value tests
        {
            bool b;
            float f;
            unsigned u;
            const char *s;

            unittest.teststring = 10;
            f = unittest.teststring;
            u = unittest.teststring;
            s = unittest.teststring;
            b = unittest.teststring;
            
            CHECK((f - 10.0f) <= std::numeric_limits<float>::epsilon());
            CHECK(u == 10);
            CHECK(::strcmp(s, "10") == 0);
            CHECK(b == false);

            unittest.teststring = 10.01;
            f = unittest.teststring;
            u = unittest.teststring;
            s = unittest.teststring;
            
            CHECK((f - 10.01f) <= std::numeric_limits<float>::epsilon());
            CHECK(u == 10);
            CHECK(::strcmp(s, "10.01") == 0);

            unittest.testvalue = 10.01;
            f = unittest.testvalue;
            u = unittest.testvalue;
            s = unittest.testvalue;
            b = unittest.testvalue;
            
            CHECK((f - 10.01f) <= std::numeric_limits<float>::epsilon());
            CHECK(u == 10);
            CHECK(::strcmp(s, "10.01") == 0);
            CHECK(b == true);

            unittest.testbool = true;
            f = unittest.testbool;
            u = unittest.testbool;
            s = unittest.testbool;
            b = unittest.testbool;
            
            CHECK(f >= std::numeric_limits<float>::epsilon());
            CHECK(u == 1);
            CHECK(::strcmp(s, "true") == 0);
        }
        
        // Array tests
        {
            int firedEventCount = 0;
            
            datahub::EventToken eventAdd = unittest.testarray.onArrayElementAdded += [&firedEventCount](datahub::ArrayToken token, unittest::ArrayElement &element) {
                firedEventCount++;
            };
            datahub::EventToken eventRemove = unittest.testarray.onArrayElementRemoving += [&firedEventCount](datahub::ArrayToken token) {
                firedEventCount++;
            };

            datahub::ArrayToken token0 = unittest.testarray.addArrayElement([](unittest::ArrayElement &element) {
                element.teststring = 1;
            });
            
            CHECK(firedEventCount == 1);
            
            datahub::ArrayToken token1 = unittest.testarray.addArrayElement([](unittest::ArrayElement &element) {
                element.teststring = 2;
            });
            
            CHECK(firedEventCount == 2);
            CHECK(unittest.testarray[token0].teststring == "1");
            CHECK(unittest.testarray[token1].teststring == "2");

            unittest.testarray.removeArrayElement(token0);
            
            CHECK(firedEventCount == 3);
            CHECK(unittest.testarray[token1].teststring == "2");

            unittest.testarray.removeArrayElement(token1);
            unittest.testarray.onArrayElementAdded -= eventAdd;
            unittest.testarray.onArrayElementRemoving -= eventRemove;

        }
    }
}


