//
// Created by Jeramy on 4/19/2015.
//

#ifndef LUDUMDARE32_ECHO_HPP
#define LUDUMDARE32_ECHO_HPP

#ifndef NDEBUG
#include <iostream>
template <typename Head>
void ECHO(const Head& head) {
    std::clog << head << std::endl;
}
template <typename Head, typename... Tail>
void ECHO(const Head& head, const Tail&... tail) {
    std::clog << head << " ";
    ECHO(tail...);
}
#else
template <typename... Ts>
void ECHO(Ts&&...) {}
#endif

#endif //LUDUMDARE32_ECHO_HPP
