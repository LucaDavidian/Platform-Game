#ifndef DECLVAL_H
#define DECLVAL_H

namespace Traits
{

template <typename T>
struct AddRValueReference;

} 

/* declval */
template <typename T>
typename Traits::AddRValueReference<T>::Type Declval();

#endif  // DECLVAL_H