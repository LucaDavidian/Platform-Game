#ifndef UTILITY_H
#define UTILITY_H

#include "Traits.hpp"

namespace Utility
{

    template <typename T>
    Traits::RemoveReferenceT<T>&& Move(T &&t)
    {
        using ReturnType = Traits::RemoveReferenceT<T>&&;
        return static_cast<ReturnType>(t);
    }

    // template <typename T>
    // T &&Forward(typename Traits::Identity<T>::Type &t)   // non deduced context (avoids argument deduction - forward(t))
    // {                                                    
    //     return static_cast<T&&>(t);
    // }

    template <typename T>
    T &&Forward(typename Traits::RemoveReference<T>::Type &t)   // non deduced context (avoids argument deduction - forward(t))
    {                                                    
        return static_cast<T&&>(t);
    }

}  // end Utility namespace

#endif  // UTILITY_H