#ifndef TRAITS_H
#define TRAITS_H

#include "../utility/Declval.hpp"

namespace Traits
{
    /**** type transformation traits ****/

    /* remove reference */
    template <typename T>
    struct RemoveReference
    {
        using Type = T;
    };

    template <typename T>
    struct RemoveReference<T&>
    {
        using Type = T;
    };

    template <typename T>
    struct RemoveReference<T&&>
    {
        using Type = T;
    };

    template <typename T>
    using RemoveReferenceT = typename RemoveReference<T>::Type;

    /* add lvalue reference */
    template <typename T>
    struct AddLValueReference
    {
        using Type = T&;
    };

    template <>
    struct AddLValueReference<void>
    {
        using Type = void;
    };

    template <>
    struct AddLValueReference<const void>
    {
        using Type = const void;
    };

    template <>
    struct AddLValueReference<volatile void>
    {
        using Type = volatile void;
    };

    template <>
    struct AddLValueReference<const volatile void>
    {
        using Type = const volatile void;
    };

    template <typename T>
    using AddLValueReferenceT = typename AddLValueReference<T>::Type;

    // template <typename T>           // if partial specializations are not needed (i.e. for void)
    // using AddLValueReferenceT = T&;

    /* add rvalue reference */
    template <typename T>
    struct AddRValueReference
    {
        using Type = T&&;
    };

    template <>
    struct AddRValueReference<void>
    {
        using Type = void;
    };

    template <>
    struct AddRValueReference<const void>
    {
        using Type = const void;
    };

    template <>
    struct AddRValueReference<volatile void>
    {
        using Type = volatile void;
    };

    template <>
    struct AddRValueReference<const volatile void>
    {
        using Type = const volatile void;
    };

    template <typename T>
    using AddRValueReferenceT = typename AddRValueReference<T>::Type;

    // template <typename T>       // if partial specializations are not needed (i.e. for void)
    // using AddRValueReferenceT = T&&;

    /* add pointer */
    template <typename T>
    struct AddPointer
    {
        using Type = T*;
    };

    template <typename T>
    struct AddPointer<T&>
    {
        using Type = typename RemoveReference<T>::Type*;
    };

    template <typename T>
    struct AddPointer<T&&>
    {
        using Type = typename RemoveReference<T>::Type *;
    };

    template <typename T>
    using AddPointerT = typename AddPointer<T>::Type;

    /* remove array extent */
    template <typename T>
    struct RemoveExtent
    {
        using Type = T;
    };

    template <typename T, unsigned N>
    struct RemoveExtent<T[N]>
    {
        using Type = T;
    };

    template <typename T>
    struct RemoveExtent<T[]>
    {
        using Type = T;
    };

    template <typename T>
    using RemoveExtentT = typename RemoveExtent<T>::Type;

    /* remove const qualifier */
    template <typename T>
    struct RemoveConst
    {
        using Type = T;
    };

    template <typename T>
    struct RemoveConst<const T>
    {
        using Type = T;
    };

    template <typename T>
    using RemoveConstT = typename RemoveConst<T>::Type;

    /* remove volatile qualifier */
    template <typename T>
    struct RemoveVolatile
    {
        using Type = T;
    };

    template <typename T>
    struct RemoveVolatile<volatile T>
    {
        using Type = T;
    };

    template <typename T>
    using RemoveVolatileT = typename RemoveVolatile<T>::Type;

    /* remove const and volatile qualifiers */
    template <typename T>
    struct RemoveConstVolatile : RemoveVolatile<typename RemoveConst<T>::Type>
    {
       //using Type = typename RemoveVolatile<typename RemoveConst<T>::Type>>::Type;
    };

    //template <typename T>
    //using RemoveConstVolatileT = typename RemoveConstVolatile<T>::Type;

    template <typename T>
    using RemoveConstVolatileT = RemoveVolatileT<RemoveConstT<T>>;  // no need to partial specialize class template

    /**** conditional ****/
    template <bool Condition, typename A, typename B>
    struct Conditional 
    {
        using Type = A;
    };

    template <typename A, typename B>
    struct Conditional<false, A, B>
    {
        using Type = B;
    };

    template <bool Condition, typename A, typename B>
    using ConditionalT = typename Conditional<Condition, A, B>::Type;

    /**** decay (alternative implementation below) ****/
    // template <typename T>
    // struct Decay : RemoveConstVolatile<typename RemoveReference<T>::Type>
    // {
    // };

    // template <typename T, unsigned N>
    // struct Decay<T[N]>
    // {
    //     using Type = T*;
    // };

    // template <typename T>
    // struct Decay<T[]>
    // {
    //     using Type = T*;
    // };

    // template <typename Ret, typename... Args>
    // struct Decay<Ret(Args...)>
    // {
    //     using Type = Ret(*)(Args...);
    // };

    // template <typename Ret, typename... Args>
    // struct Decay<Ret(Args..., ...)>
    // {
    //     using Type = Ret(*)(Args..., ...);
    // };

    /**** integral constant, bool constant, true type and false type ****/

    template <typename T, T Val>
    struct IntegralConstant
    {
        using ValueType = T;
        //using Type = IntegralConstant<T,Val>;
        using Type = IntegralConstant;   // injected class name
        static constexpr T Value = Val;
    };

    // template <bool b>
    // struct BoolConstant : IntegralConstant<bool,b>   // BoolConstant is-a IntegralConstant
    // {
    // };

    // template <bool b>
    // struct BoolConstant 
    // {
    //     using ValueType = bool;
    //     using Type = typename IntegralConstant<bool,b>::Type;
    //     static constexpr Value = IntegralConstant<bool,b>::Value;
    // };

    template <bool b>
    using BoolConstant = IntegralConstant<bool,b>;

    using TrueType = BoolConstant<true>;
    using FalseType = BoolConstant<false>;

    /**** predicate traits derive from true type and false type ****/

    /* is same */
    template <typename T1, typename T2>
    struct IsSame : FalseType
    {
    };

    template <typename T>
    struct IsSame<T,T> : TrueType
    {
    };

    template <typename T1, typename T2>
    constexpr bool IsSameV = IsSame<T1,T2>::Value;

    /**** result type traits ****/

    template <typename T1, typename T2>
    struct PlusResult
    {
        using Type = decltype(Declval<T1>() + Declval<T2>());
    };

    template <typename T1, typename T2>
    using PlusResultT = typename PlusResult<T1,T2>::Type;

    /**** type classification ****/

    /* pointer type */
    template <typename T>
    struct IsPointer : FalseType
    {
    };

    template <typename T>
    struct IsPointer<T*> : TrueType
    {
    };

    template <typename T>
    constexpr bool IsPointerV = IsPointer<T>::Value;

    /* lvalue reference type */
    template <typename T>
    struct IsLValueReference : FalseType
    {
    };

    template <typename T>
    struct IsLValueReference<T&> : TrueType
    {
    };

    template <typename T>
    constexpr bool IsLValueReferenceV = IsLValueReference<T>::Value;

    /* rvalue reference type */
    template <typename T>
    struct IsRValueReference : FalseType
    {
    };

    template <typename T>
    struct IsRValueReference<T&&> : TrueType
    {
    };

    template <typename T>
    constexpr bool IsRValueReferenceV = IsRValueReference<T>::Value;

    /* reference type */
    template <typename T>
    struct IsReference : Conditional<IsLValueReferenceV<T>, IsLValueReference<T>, IsRValueReference<T>>::Type
    {
    };

    template <typename T>
    constexpr bool IsReferenceV = IsReference<T>::Value;

    /* array type */
    template <typename T>
    struct IsArray : FalseType
    {
    };

    template <typename T, unsigned N>
    struct IsArray<T[N]> : TrueType
    {  
    };

    template <typename T>
    struct IsArray<T[]> : TrueType
    {  
    };

    template <typename T>
    constexpr bool IsArrayV = IsArray<T>::Value;

    /* function type */
    template <typename T>
    struct IsFunction : FalseType
    {
    };

    template <typename Ret, typename... Args>
    struct IsFunction<Ret(Args...)> : TrueType
    {
    };

    template <typename T>
    constexpr bool IsFunctionV = IsFunction<T>::Value;

    /**** decay ****/
    template <typename T>
    struct Decay
    {
    private:
        using U = RemoveReferenceT<T>;
    public:
        using Type = typename Conditional<
                        IsArray<U>::Value, 
                        typename RemoveExtent<U>::Type*, 
                        typename Conditional<
                            IsFunction<U>::Value, 
                            typename AddPointer<U>::Type, 
                            typename RemoveConstVolatile<U>::Type
                        >::Type
                    >::Type;
    };

    template <typename T>
    using DecayT = typename Decay<T>::Type;

    /**** SFINAE traits ****/

    /* enable if */
    template <bool b, typename = void>
    struct EnableIf
    {
    };

    template <typename T>
    struct EnableIf<true,T>
    {
        using Type = T;
    };

    template <bool b, typename T = void>
    using EnableIfT = typename EnableIf<b,T>::Type;

    /**** SFINAE-based traits ****/

    /* is default constructible */
    // template <typename T>
    // struct IsDefaultConstructible
    // {
    // private:
    //     template <typename U, typename = decltype(U())>
    //     static char Test(void*);
    //     template <typename>
    //     static long Test(...);
    // public:
    //     static constexpr bool Value = IsSame<Test<T>(nullptr),char>::Value;
    // };

    // template <typename T>
    // constexpr bool IsDefaultConstructibleV = IsDefaultConstructible<T>::Value;

    // template <typename T>                                // overloaded function template specialization
    // struct IsDefaultConstructibleH
    // {
    // private:
    //     template <typename U, typename = decltype(U())>
    //     static TrueType Test(void*);
    //     template <typename>
    //     static FalseType Test(...);
    // public:
    //     using Type = decltype(Test<T>(nullptr));
    // };

    // template <typename T>
    // struct IsDefaultConstructible : IsDefaultConstructibleH<T>::Type
    // {
    // };

    // template <typename T>
    // constexpr bool IsDefaultConstructibleV = IsDefaultConstructible<T>::Value;

    template <typename... Types>
    using VoidT = void;

    template <typename, typename = void>             // class template specialization implementation
    struct IsDefaultConstructible : FalseType 
    {
    };

    template <typename T>
    struct IsDefaultConstructible<T, VoidT<decltype(T())>> : TrueType
    {
    };

    /* is convertible */
    template <typename FROM, typename TO>               // overloaded function template specialization
    struct IsConvertibleH
    {
    private:
        static void Aux(TO);
        template <typename F, typename = decltype(Aux(Declval<F>()))>
        static TrueType Test(void*);
        template <typename>
        static FalseType Test(...);
    public:
        using Type = decltype(Test<FROM>(nullptr));
    };

    template <typename FROM, typename TO>
    struct IsConvertible : IsConvertibleH<FROM,TO>::Type
    {
    };

    template <typename FROM, typename TO>
    constexpr bool IsConvertibleV = IsConvertible<FROM,TO>::Value;

}  // namespace Traits

#endif  // TRAITS_H
