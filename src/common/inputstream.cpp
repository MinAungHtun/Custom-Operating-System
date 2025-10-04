 
#include <common/inputstream.h>

namespace MYYOS
{
    namespace common
    {
        
        InputStream::InputStream(InputStreamEventHandler<string>* handler)
            : BasicInputStream<string>::BasicInputStream(handler)
        {
        }
        
    }
}
