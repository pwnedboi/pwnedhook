/*
 *  cutlmemory.h
 */
#pragma once

template <class T, class I = int>
class CUtlMemory
{
public:
    
    T& operator[](I i)
    {
        return m_memory[i];
    }
    
protected:
    
    T*  m_memory;
    int m_allocation_count;
    int m_grow_size;
    
};

template <class T, class A = CUtlMemory<T>>
class CUtlVector
{
    typedef A CAllocator;
    
public:
    
    T& operator[](int i)
    {
        return m_memory[i];
    }
    
    int count() const
    {
        return m_size;
    }
    
protected:
    
    CAllocator     m_memory;
    int         m_size;
    T*          m_elements;
    
};
