#ifndef INTEGER_SEQUENCE_H
#define INTEGER_SEQUENCE_H

#include <cstddef>

using std::size_t;

// sequence of integer values
template <typename T, T... Values>
struct IntegerSequence
{
};

// sequence of std::size_t values (indices)
template <size_t... indices>
using IndexSequence = IntegerSequence<std::size_t, indices...>;

// index sequence - make a sequence of std::size_t values from 0 to N - 1
template <size_t Index, size_t... Sequence>
struct MakeIndexSequenceImpl
{
    using Type = typename MakeIndexSequenceImpl<Index - 1, Index - 1, Sequence...>::Type;
};

template <size_t... Sequence>
struct MakeIndexSequenceImpl<0U, Sequence...>
{
    using Type = IndexSequence<Sequence...>;
};

template <size_t N>
using MakeIndexSequence = typename MakeIndexSequenceImpl<N>::Type;

// index sequence - make a sequence of std::size_t values from From to N - 1
template <size_t From, size_t Index, size_t... Sequence>
struct MakeIndexSequenceFromImpl
{
    using Type = typename MakeIndexSequenceFromImpl<From, Index - 1, Index - 1, Sequence...>::Type;
};

template <size_t From, size_t... Sequence>
struct MakeIndexSequenceFromImpl<From, From, Sequence...>
{
    using Type = IndexSequence<Sequence...>;
};

template <size_t From, std::size_t N>
using MakeIndexSequenceFrom = typename MakeIndexSequenceFromImpl<From, N>::Type;

#include "Typelist.hpp"

template <unsigned int N>
struct MakeIndexList
{
    using Type = typename PushBack<typename MakeIndexList<N - 1>::Type, Traits::IntegralConstant<unsigned int, N>>::Type;
};

template <>
struct MakeIndexList<0U>
{
    using Type = Valuelist<unsigned int, 0U>;
};

template <unsigned int N>
using MakeIndexListT = typename MakeIndexList<N>::Type;

#endif  // INTEGER_SEQUENCE_H