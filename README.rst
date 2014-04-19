-----------
Variant CPP
-----------

A flexible C++ data structure, which can contain any type

Just for fun. But I will continue to improve it. :P

*Note: This library is written in C++11 standard*

Usage
=====

.. code:: cpp

    #include "VariantList.hpp"
    #include <iostream>
    #include <string>
    using namespace std;

    int main(int argc, char *argv[]) {
        VariantList variant_list;

        variant_list.push_back(1); // int
        variant_list.push_back(1.2); // double
        variant_list.push_back<string>("Hello"); // string

        VariantList another_list;
        another_list.push_back(10);
        another_list.push_back(10.1f);

        variant_list.push_back(another_list); // !!

        variant_list.push_back(variant_list); // !!

        cout << variant_list << endl;

        auto itr = variant_list.begin();
        cout << "First: " << itr->get<int>() << endl;
        ++ itr;
        cout << "Second: " << itr->get<double>() << endl;
        ++ itr;
        cout << "Third: " << itr->get<string>() << endl;

        *itr = make_variant(10.2); // Change element 3 "Hello" (std::string) to 10.2 (Double)
        cout << "Third: " << itr->get<double>() << endl;

        cout << variant_list << endl;

        return 0;
    }

Run the previous code section will print

.. code::

    [1, 1.2, Hello, [10, 10.1], [1, 1.2, Hello, [10, 10.1]]]
    First: 1
    Second: 1.2
    Third: Hello
    Third: 10.2
    [1, 1.2, 10.2, [10, 10.1], [1, 1.2, Hello, [10, 10.1]]]

TODO
====

* Improve usability

* Implement other kind of variant data structure

License
=======

MIT License
