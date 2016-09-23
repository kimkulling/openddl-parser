/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2014-2015 Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once

#include <openddlparser/OpenDDLCommon.h>

#include <string>
#include <cassert>

BEGIN_ODDLPARSER_NS

struct ValueAllocator;

///------------------------------------------------------------------------------------------------
///	@brief  This class implements a value.
///
///	Values are used to store data types like boolean, integer, floats, double and many mode. To get
///	an overview please check the enum VylueType ( @see Value::ValueType ).
/// Values can be single items or lists of items. They are implemented as linked lists.
///------------------------------------------------------------------------------------------------
class DLL_ODDLPARSER_EXPORT Value {
    friend struct ValueAllocator;

public:
    ///	@brief  This class implements an iterator through a Value list.
    ///
    /// When getting a new value you need to know how to iterate through it. The Value::Iterator
    /// will help you here:
    ///	@code
    /// Value *val = node->getValue();
    /// Value::Iterator it( val );
    /// while( it.hasNext() ) {
    ///     Value v( it.getNext );
    /// }
    /// @endcode
    class DLL_ODDLPARSER_EXPORT Iterator {
    public:
        ///	@brief  The default class constructor.
        Iterator();

        ///	@brief  The class constructor with the start value.
        /// @param  start   [in] The first value for iteration,
        Iterator( Value *start );

        Iterator( const Iterator &rhs );

        ///	@brief  The class destructor.
        ~Iterator();

        ///	@brief  Will return true, if another value is in the list.
        /// @return true if another value is there.
        bool hasNext() const;

        ///	@brief  Returns the next item and moves the iterator to it.
        ///	@return The next value, is ddl_nullptr in case of being the last item.
        Value *getNext();

        ///	@brief  The post-increment operator.
        const Iterator operator++( int );

        ///	@brief  The pre-increment operator.
        Iterator &operator++( );

        ///	@brief  The compare operator.
        /// @param  rhs [in] The instance to compare.
        /// @return true if equal.
        bool operator == ( const Iterator &rhs ) const;

        /// @brief  The * operator.
        /// @return The instance or ddl_nullptr if end of list is reached.
        Value *operator->( ) const;

    private:
        Value *m_start;
        Value *m_current;

    private:
        Iterator &operator = ( const Iterator & );
    };

    ///	@brief  This enum describes the data type stored in the value.
    enum ValueType {
        ddl_none = -1,          ///< Nothing specified
        ddl_bool = 0,           ///< A boolean type
        ddl_int8,               ///< Integer type, 8 bytes
        ddl_int16,              ///< Integer type, 16 bytes
        ddl_int32,              ///< Integer type, 32 bytes
        ddl_int64,              ///< Integer type, 64 bytes
        ddl_unsigned_int8,      ///< Unsigned integer type, 8 bytes
        ddl_unsigned_int16,     ///< Unsigned integer type, 16 bytes
        ddl_unsigned_int32,     ///< Unsigned integer type, 32 bytes
        ddl_unsigned_int64,     ///< Unsigned integer type, 64 bytes
        ddl_half,               ///< Half data type.
        ddl_float,              ///< float data type
        ddl_double,             ///< Double data type.
        ddl_string,             ///< String data type.
        ddl_ref,                ///< Reference, used to define references to other data definitions.
        ddl_types_max           ///< Upper limit.
    };


    Value():m_type(ddl_none),
            m_next(ddl_nullptr){}

    Value(bool value):
        m_type(ddl_bool),
        booleanValue(value),
        m_next(ddl_nullptr){}

    Value(int8 value):
            m_type(ddl_int8),
            int8Value(value),
            m_next(ddl_nullptr){}

    Value(int16 value):
            m_type(ddl_int16),
            int16Value(value),
            m_next(ddl_nullptr){}

    Value(int32 value):
            m_type(ddl_int32),
            int32Value(value),
            m_next(ddl_nullptr){}

    Value(int64 value):
            m_type(ddl_int64),
            int64Value(value),
            m_next(ddl_nullptr){}

    Value(uint8 value):
            m_type(ddl_unsigned_int8),
            uint8Value(value),
            m_next(ddl_nullptr){}

    Value(uint16 value):
            m_type(ddl_unsigned_int16),
            uint16Value(value),
            m_next(ddl_nullptr){}

    Value(uint32 value):
            m_type(ddl_unsigned_int32),
            uint32Value(value),
            m_next(ddl_nullptr){}

    Value(uint64 value):
            m_type(ddl_unsigned_int64),
            uint64Value(value),
            m_next(ddl_nullptr){}

    Value(float value):
            m_type(ddl_float),
            floatValue(value),
            m_next(ddl_nullptr){}

    Value(double value):
            m_type(ddl_double),
            doubleValue(value),
            m_next(ddl_nullptr){}

    Value(const std::string &value):
            m_type(ddl_string),
            strValue(ddl_nullptr),
            m_next(ddl_nullptr){
        setString(value);
    }

    Value(const char* str,const size_t len):
            m_type(ddl_string),
            strValue(new char[len+1]),
            m_next(ddl_nullptr) {
        ::strncpy(strValue,str,len);
        strValue[len]='\0'; //since str can be a buffer bigger than len
    }

    /**
     * It will store a copy of the reference!
     * @param value
     * @return
     */
    Value(Reference *value):
            m_type(ddl_ref),
            referenceValue(new Reference(*value)),
            m_next(ddl_nullptr){}


    ///	@brief  The class destructor.
    ~Value();

    ValueType getType()const{
        return m_type;
    }

    ///	@brief  Assigns a boolean to the value.
    /// @param  value       [in9 The value.
    void setBool(const bool value ){
        freeAllocatedMemory();
        m_type=ddl_bool;
        booleanValue=value;
    }

    ///	@brief  Returns the boolean value.
    /// @return The boolean value.
    bool getBool() const{
        return booleanValue;
    }

    ///	@brief  Assigns a int8 to the value.
    /// @param  value       [in] The value.
    void setInt8(const int8 value ){
        freeAllocatedMemory();
        m_type=ddl_int8;
        int8Value=value;
    }

    ///	@brief  Returns the int8 value.
    /// @return The int8 value.
    int8 getInt8()const{
        return int8Value;
    }

    ///	@brief  Assigns a int16 to the value.
    /// @param  value       [in] The value.
    void setInt16(const int16 value ){
        freeAllocatedMemory();
        m_type=ddl_int16;
        int16Value=value;
    }

    ///	@brief  Returns the int16 value.
    /// @return The int16 value.
    int16 getInt16()const{
        return int16Value;
    }

    ///	@brief  Assigns a int32 to the value.
    /// @param  value       [in] The value.
    void setInt32(const int32 value ){
        freeAllocatedMemory();
        m_type=ddl_int32;
        int32Value=value;
    }

    ///	@brief  Returns the int16 value.
    /// @return The int32 value.
    int32 getInt32()const{
        return int32Value;
    }

    ///	@brief  Assigns a int64 to the value.
    /// @param  value       [in] The value.
    void setInt64(const int64 value ){
        freeAllocatedMemory();
        m_type=ddl_int64;
        int64Value=value;
    }

    ///	@brief  Returns the int16 value.
    /// @return The int64 value.
    int64 getInt64()const{
        return int64Value;
    }

    ///	@brief  Assigns a unsigned int8 to the value.
    /// @param  value       [in] The value.
    void setUnsignedInt8(const uint8 value ){
        freeAllocatedMemory();
        m_type=ddl_unsigned_int8;
        uint8Value=value;
    }

    ///	@brief  Returns the unsigned int8 value.
    /// @return The unsigned int8 value.
    uint8 getUnsignedInt8() const{
        return uint8Value;
    }

    ///	@brief  Assigns a unsigned int16 to the value.
    /// @param  value       [in] The value.
    void setUnsignedInt16(const uint16 value ){
        freeAllocatedMemory();
        m_type=ddl_unsigned_int16;
        uint16Value=value;
    }

    ///	@brief  Returns the unsigned int16 value.
    /// @return The unsigned int16 value.
    uint16 getUnsignedInt16() const{
        return uint16Value;
    }

    ///	@brief  Assigns a unsigned int32 to the value.
    /// @param  value       [in] The value.
    void setUnsignedInt32(const uint32 value ){
        freeAllocatedMemory();
        m_type=ddl_unsigned_int32;
        uint32Value=value;
    }

    ///	@brief  Returns the unsigned int8 value.
    /// @return The unsigned int32 value.
    uint32 getUnsignedInt32() const{
        return uint32Value;
    }

    ///	@brief  Assigns a unsigned int64 to the value.
    /// @param  value       [in] The value.
    void setUnsignedInt64(const uint64 value ){
        freeAllocatedMemory();
        m_type=ddl_unsigned_int64;
        uint64Value=value;
    }

    ///	@brief  Returns the unsigned int64 value.
    /// @return The unsigned int64 value.
    uint64 getUnsignedInt64() const{
        return uint64Value;
    }

    ///	@brief  Assigns a float to the value.
    /// @param  value       [in] The value.
    void setFloat(const float value ){
        freeAllocatedMemory();
        m_type=ddl_float;
        floatValue=value;
    }

    ///	@brief  Returns the float value.
    /// @return The float value.
    float getFloat() const{
        return floatValue;
    }

    ///	@brief  Assigns a double to the value.
    /// @param  value       [in] The value.
    void setDouble(const double value ){
        freeAllocatedMemory();
        m_type=ddl_double;
        doubleValue=value;
    }

    ///	@brief  Returns the double value.
    /// @return The double value.
    double getDouble() const{
        return doubleValue;
    }

    ///	@brief  Assigns a std::string to the value.
    /// @param  value       [in] The value.
    void setString( const std::string &str ){
        freeAllocatedMemory();
        m_type=ddl_string;
        strValue = new char[str.size()+1];
        ::strncpy(strValue,str.c_str(),str.size());
        strValue[str.size()]='\0';
    }

    ///	@brief  Returns the std::string value.
    /// @return The std::string value.
    const char *getString() const{
        assert( ddl_string == m_type );
        return strValue;
    }

    /// @brief  Set the reference.
    /// @param  ref     [in] Pointer showing to the reference.
    void setRef(const Reference *const ref ){
        freeAllocatedMemory();
        m_type=ddl_ref;
        referenceValue=new Reference(*ref);
    }

    ///	@brief  Dumps the value.
    void dump();

    /// @brief  Returns the pointer showing to the reference.
    /// @return Pointer showing to the reference.
    Reference *getRef() const{
        assert( ddl_ref == m_type );
        return referenceValue;
    }

    ///	@brief  Assigns the next value.
    ///	@param  next        [n] The next value.
    void setNext( Value *next ){
        m_next=next;
    }

    ///	@brief  Returns the next value.
    /// @return The next value.s
    Value *getNext() const{
        return m_next;
    }

    /// @brief  Gets the length of the array.
    /// @return The number of items in the array.
    size_t size();

private:
    Value &operator =( const Value & ) ddl_no_copy;
    Value( const Value  & ) ddl_no_copy;

    void freeAllocatedMemory(){
        if(m_type==ddl_string)
            delete [] strValue;
        else if(m_type==ddl_ref)
            delete referenceValue;
    }

    ValueType m_type;
    union{
        bool booleanValue;
        int8  int8Value;
        int16 int16Value;
        int32 int32Value;
        int64 int64Value;
        uint8  uint8Value;
        uint16 uint16Value;
        uint32 uint32Value;
        uint64 uint64Value;
        float floatValue;
        double doubleValue;
        char *strValue;
        Reference *referenceValue;
    };
    Value *m_next;

};

END_ODDLPARSER_NS
