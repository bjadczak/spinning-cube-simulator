//
// Created by Bartosz Jadczak on 09/10/2024.
//

#ifndef VARIANTUTIL_H
#define VARIANTUTIL_H

// helper type for the visitor #4
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

#endif //VARIANTUTIL_H
