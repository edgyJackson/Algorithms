#ifndef _TPF_TYPE_NAME_H
#define _TPF_TYPE_NAME_H
/*
    Author: Thomas Kim
    Date: Nov. 01, 2018
    Fix: Jan. 28, 2019 - for compatiblity with C++14
    Fix: Feb. 01, 2019 - for iterator manipulation
    Fix: Feb, 02, 2019 - for additional iterator manipulation
*/

#include <string>
#include <typeinfo>
#include <type_traits>
#include <iterator>

namespace tpf
{
    template<typename T>
    std::string type_to_string()
    {
        #if defined(_MSC_VER)
            
            std::string type_name (__FUNCSIG__);
            auto start_pos = type_name.find_first_of('<', 
                std::string(typeid(std::string).name()).size())+1;
            auto end_pos = type_name.find_last_of('>');

            return type_name.substr(start_pos, (end_pos-start_pos));
            
        #elif defined(__clang__)
            std::string type_name (__PRETTY_FUNCTION__);
            auto start_pos = type_name.find_first_of('=')+2;
            auto end_pos = type_name.find_first_of(']', start_pos);
            return type_name.substr(start_pos, (end_pos - start_pos));

        #elif defined(__GNUC__)
            std::string type_name (__PRETTY_FUNCTION__);
            auto start_pos = type_name.find_first_of('=')+2;
            auto end_pos = type_name.find_first_of(';', start_pos);
            return type_name.substr(start_pos, (end_pos - start_pos));
        #else
            return "Failed To Convert to String";
        #endif
    }

    namespace hide
    {
        //  primary template class
        template<class _Ty>
        struct st_remove_const
        {	
            using type = _Ty;
        };

        // specialization const _Ty
        template<class _Ty>
        struct st_remove_const<const _Ty>
        {	
            using type = _Ty;
        };

        // specialization const _Ty
        template<class _Ty>
        struct st_remove_const<const _Ty&>
        {	
            using type = _Ty&;
        };

        // specialization const _Ty
        template<class _Ty>
        struct st_remove_const<const _Ty&&>
        {	
            using type = _Ty&&;
        };

        template<class _Ty>
            using remove_const_helper = typename st_remove_const<_Ty>::type;
            
        template<typename RefType>
            using add_const_ref_helper = std::conditional_t<
                std::is_lvalue_reference<RefType>::value,
                std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<RefType>>>,
                std::add_rvalue_reference_t<std::add_const_t<std::remove_reference_t<RefType>>>>;

            template<typename T>
            using add_const_helper = std::conditional_t<
                std::is_reference<T>::value,
                add_const_ref_helper<T>, std::add_const_t<T>>;
                
    } // name of namespace hide

    template<typename T>
    using remove_const_reference_t = 
        std::remove_const_t<std::remove_reference_t<T>>;

    namespace hide
    {
        template<typename T> struct 
        element_type_st
        {
            using type = T;
            static constexpr bool value = true;
        };

        template<template<typename, typename...> class ClassType, typename T, typename... Types>
        struct element_type_st<ClassType<T, Types...>>
        {
            using type = T;
            static constexpr bool value = false;
        };

        template<typename T>
        using ele_type_t = typename element_type_st<T>::type;

        struct NoTypeDummy{ };

        template<typename Type>
        constexpr std::enable_if_t<true, typename std::iterator_traits<Type>::value_type> is_iterator_fn(Type);
        constexpr NoTypeDummy is_iterator_fn(...);

        template<typename Type>
        constexpr auto iterator_value_fn(Type)->decltype(std::declval<typename std::iterator_traits<Type>::value_type>());

      ///////////////////////////////////
      // if T is an iterator, this function is enabled
        // and returns the value type of the iterator type T
        // if T is not an iterator, then this function is SFINAEd out
        template<typename T> constexpr std::enable_if_t<true,
            typename std::iterator_traits<T>::value_type>
            iterator_value_type_fn(T&&);

        // this is catch all function for iterator_value_type_fn()
        constexpr NoTypeDummy iterator_value_type_fn(...);

        // if T is an iterator iterator_value_type_fn returns value_type of the 
        // iterator, otherwise NoTypeDummy is returned.
        // if the returned value is Not equal to NoTypeDummy,
        // then T is an iterator, otherwise not an iterator
        template<typename T>
        constexpr bool is_iterator_type_v = !std::is_same<NoTypeDummy,
             decltype(iterator_value_type_fn(std::declval<T>()))>::value;

        template<typename T>
        constexpr bool is_iterator_type_excluding_pointer_v = 
            is_iterator_type_v<T> && !std::is_pointer<T>::value;

        // if T is an iterator, it returns T::value_type
        // otherwise, it returns NoTypeDummy
        template<typename T>
        using iterator_value_type_t = decltype(iterator_value_type_fn(std::declval<T>()));

        // if T is an iterator, this returns T
        // otherwise, it is SFINAEd out
        template<typename T>
        using enable_if_iterator_type_t = std::enable_if_t<is_iterator_type_v<T>, T>;

        // if T is an iterator, this returns T::value_type
        // otherwise, it is SFINAEd out
        template<typename T>
        using enable_if_iterator_value_type_t = std::enable_if_t<is_iterator_type_v<T>, iterator_value_type_t<T>>;

        // if T is an iterator, this returns T::value_type
        // otherwise, it returns T
        template<typename T>
        using conditional_iterator_value_type_t = 
            typename std::conditional<is_iterator_type_v<T>, iterator_value_type_t<T>, T>::type;

    } // end of namespace hide

    ////////////////////////
    // if T is an iterator or a pointer, it returns true
    // otherwise, returns false
    template<typename T>
        constexpr bool is_iterator_type_v = hide::is_iterator_type_v<T>;

    // if T is an iterator and is not a pointer, it returns true
    // otherwise, returns false
     template<typename T>
        constexpr bool is_iterator_type_excluding_pointer_v = hide::is_iterator_type_excluding_pointer_v<T>;

    // if T is an iterator, it returns T::value_type
    // otherwise, it returns NoTypeDummy
    template<typename T>
        using iterator_value_type_t = hide::iterator_value_type_t<T>;

    // if T is an iterator, this returns T
    // otherwise, it is SFINAEd out
    template<typename T>
    using enable_if_iterator_type_t = hide::enable_if_iterator_type_t<T>;

    // if T is an iterator, this returns T::value_type
    // otherwise, it is SFINAEd out
    template<typename T>
    using enable_if_iterator_value_type_t = hide::enable_if_iterator_value_type_t<T>;

    // if T is an iterator, this returns T::value_type
    // otherwise, it returns T
    template<typename T>
    using conditional_iterator_value_type_t = hide::conditional_iterator_value_type_t<T>;

    template<typename T>
    constexpr bool is_iterator_v = !std::is_same<decltype(hide::is_iterator_fn(std::declval<T>())), hide::NoTypeDummy>::value;

    template<typename T>
    using iterator_value_type = std::remove_reference_t<decltype(hide::iterator_value_fn(std::declval<T>()))>;

    template<typename T>
    using element_t = hide::ele_type_t<remove_const_reference_t<T>>;

    template<typename T>
    constexpr bool is_scalar_v = hide::element_type_st<remove_const_reference_t<T>>::value;

    template<typename T>
    constexpr bool is_not_scalar_v = !is_scalar_v<T>;

    template<typename LType, typename RType>
    constexpr bool is_same_element_v = 
        std::is_same<tpf::element_t<LType>, tpf::element_t<RType>>::value;

    namespace hide
    {
        template<typename T>
        constexpr bool is_const_helper = std::is_const<std::remove_reference_t<T>>::value;

        template<typename T>
        constexpr bool is_lvalue_helper = std::is_lvalue_reference<T>::value;

        template<typename T>
        constexpr bool is_xvalue_helper = std::is_rvalue_reference<T>::value;

        template<typename T>
        constexpr bool is_prvalue_helper = !(is_lvalue_helper<T> || is_xvalue_helper<T>);

        template<typename T>
        constexpr bool is_rvalue_helper = is_xvalue_helper<T> || is_prvalue_helper<T>;

        template<typename T>
        constexpr bool is_glvalue_helper = is_xvalue_helper<T> || is_lvalue_helper<T>;
    }

}

#define Tpf_GetTypeName(type_name)  tpf::type_to_string<type_name>()
#define Tpf_GetTypeCategory(type_instance) tpf::type_to_string<decltype(type_instance)>()
#define Tpf_GetValueCategory(type_instance) tpf::type_to_string<decltype((type_instance))>()

#define Tpf_IsTypeConst(type_name) tpf::hide::is_const_helper<type_name>
#define Tpf_AddTypeConst(type_name) tpf::hide::add_const_helper<type_name>
#define Tpf_RemoveTypeConst(type_name) tpf::hide::remove_const_helper<type_name>

#define Tpf_IsLvalue(type_instance) tpf::hide::is_lvalue_helper<decltype((type_instance))>
#define Tpf_IsXvalue(type_instance) tpf::hide::is_xvalue_helper<decltype((type_instance))>
#define Tpf_IsPRvalue(type_instance) tpf::hide::is_prvalue_helper<decltype((type_instance))>
#define Tpf_IsRvalue(type_instance) tpf::hide::is_rvalue_helper<decltype((type_instance))>
#define Tpf_IsGLvalue(type_instance) tpf::hide::is_glvalue_helper<decltype((type_instance))>

#define Tpf_IsConst(type_instance) tpf::hide::is_const_helper<decltype(type_instance)>

#endif // end of _TPF_TYPE_NAME_H
