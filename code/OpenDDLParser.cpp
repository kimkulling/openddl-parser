#include "AssimpPCH.h"


#include "OpenDDLParser.h"

#include <vector>


namespace Assimp {

    struct DDLNode {
        DDLNode *m_parent;
        std::vector<DDLNode*> m_children;
    };

    template<class T>
    inline
    bool isValidToken( std::string &token, size_t index ) {

        return true;
    }

    int getNextToken( const std::vector<char> &buffer, size_t index ) {
        return true;
    }

    bool parse( const std::vector<char> &buffer ) {
        size_t index( 0 );
        const size_t buffersize( buffer.size() );
        while( index < buffersize ) {
            int next = getNextToken( buffer, index );
            if( -1 == next ) {
                break;
            }

            index = ( int ) next;
        }
        return true;
    }

    bool parseDataList( const std::vector<char> &buffer, size_t index ) {
        return true;
    }

    bool parseName() {
        return true;
    }

    bool parseDataArrayList( const std::vector<char> &buffer, size_t index ) {
        return true;
    }

    bool parseIdentifier() {
        return true;
    }

    bool parsePrimitiveDataType( const std::vector<char> &buffer, size_t index ) {
        return true;
    }


    bool parseReference() {
        return true;
    }

    bool parseProperty() {
        return true;
    }

} // Namespace Assimp
