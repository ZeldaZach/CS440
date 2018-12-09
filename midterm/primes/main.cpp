#include <assert.h>
#include "Primes.h"

// Write a class and it's iterator that will generate primes, and then iterate
// through them.

int
main() {
    
    // Primes in the range of 2 to 1000.  Define the constructor as:
    // Primes(unsigned long);
    cs540::Primes primes(1000);

    for (auto it = primes.begin(); it != primes.end(); ++it) {
        // Type of prime should be unsigned long.
        std::cout << *it << std::endl;
        assert(typeid(*it) == typeid(unsigned long));
        // Should be compile-time error.
        // *it = 1234;
    }

    auto it = primes.begin();
    // Comparison should work as expected.
    assert(*it == 2);
    auto it2 = it;
    ++it2;
    assert(*it2 == 3);
    assert(it2 != it);
    assert(--it2 == it);
    
    cs540::Primes primes1(100);
    auto it1 = primes.begin();


    assert(*it1++ == 2   );
    assert(*it1++ == 3   );
    assert(*it1++ == 5   );
    assert(*it1++ == 7   );
    assert(*it1++ == 11  );
    assert(*it1++ == 13  );
    assert(*it1++ == 17  );
    assert(*it1++ == 19  );
    assert(*it1++ == 23  );
    assert(*it1++ == 29  );
    assert(*it1++ == 31  );
    assert(*it1++ == 37  );
    assert(*it1++ == 41  );
    assert(*it1++ == 43  );
    assert(*it1++ == 47  );
    assert(*it1++ == 53  );
    assert(*it1++ == 59  );
    assert(*it1++ == 61  );
    assert(*it1++ == 67  );
    assert(*it1++ == 71  );
    assert(*it1++ == 73  );
    assert(*it1++ == 79  );
    assert(*it1++ == 83  );
    assert(*it1++ == 89  );
    assert(*it1-- == 97  );
    assert(*it1-- == 89  );
    assert(*it1-- == 83  );
    assert(*it1-- == 79  );
    assert(*it1-- == 73  );
    assert(*it1-- == 71  );
    assert(*it1-- == 67  );
    assert(*it1-- == 61  );
    assert(*it1-- == 59  );
    assert(*it1-- == 53  );
    assert(*it1-- == 47  );
    assert(*it1-- == 43  );
    assert(*it1-- == 41  );
    assert(*it1-- == 37  );
    assert(*it1-- == 31  );
    assert(*it1-- == 29  );
    assert(*it1-- == 23  );
    assert(*it1-- == 19  );
    assert(*it1-- == 17  );
    assert(*it1-- == 13  );
    assert(*it1-- == 11  );
    assert(*it1-- == 7  );
    assert(*it1-- == 5  );
    assert(*it1-- == 3  );
    assert(*it1-- == 2  );
}

