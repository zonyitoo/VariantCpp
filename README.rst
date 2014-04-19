------------
Variant List
------------

A flexible C++ list, which can contain any type

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

        cout << "First element is " << (*variant_list.begin())->get<int>() << endl;

        return 0;
    }

The previous code section will print

    [1, 1.2, Hello, [10, 10.1], [1, 1.2, Hello, [10, 10.1]]]
    First element is 1
