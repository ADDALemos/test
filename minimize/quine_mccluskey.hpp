/*Copyright (c) 2014, pfpacket All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/
#ifndef QUINE_MCCLUSKEY_HPP
#define QUINE_MCCLUSKEY_HPP


#include <iostream>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include "logical_expr.hpp"


namespace quine_mccluskey {

using namespace std;
using namespace logical_expr;

//
// logical function simplifier
//
// How to simplify:
//  [*] In the case which simplifier is constructed with logical_function
//      In this case, constructor will prepare to simplify a function
//      1. compress_table()     // Compress the compression table
//      2. simplify()           // simplify the function and get simplified
//  [*] In the case which simplifier is default-constructed
//      1. set_function()       // set a target function
//      2. make_std_spf()       // make a standard sum of products form
//      3. make_min_table()     // create a compression table
//      4. same as the case above
//
class simplifier {
public:

    typedef term_mark property_type;
    typedef logical_term<property_type> term_type;
    typedef vector<term_type> set_type;
    typedef vector<set_type> table_type;

    simplifier() : min_level_(0) 
        { add_table(table_type()); make_min_table(); }
    explicit simplifier(const logical_function<term_type> &function) : min_level_(0), func_(function)
        { add_table(table_type()); make_std_spf(); make_min_table(); }
    ~simplifier() {}

    void set_function(const logical_function<term_type> &func) { func_ = func; }
    int get_current_level() const { return min_level_; }
    const logical_function<term_type>& get_std_spf() const { return stdspf_; }
    const set_type& get_prime_implicants() const { return prime_imp; }

    // Make standard sum of products form
    const logical_function<term_type>& make_std_spf();
    const table_type& make_min_table();   
    void compress_table(bool printable = false);
    const vector<logical_function<term_type>>& simplify();    

private:
    void add_table(const table_type& table);
    void clear_table();
    template<typename T>
    static void make_unique(vector<T> &vec);

    // compress compression table
    // return true while trying to compress
    // return false if compression finished
    bool compress_impl(bool printable = false);

    int min_level_;
    logical_function<term_type> func_, stdspf_;
    vector<logical_function<term_type>> simplified_;
    vector<table_type> table_;
    set_type prime_imp;
};



}   // namespace quine_mccluskey


#endif  // QUINE_MCCLUSKEY_HPP
