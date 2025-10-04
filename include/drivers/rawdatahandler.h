
#ifndef __DRIVERS__RAWDATAHANDLER_H
    #define __DRIVERS__RAWDATAHANDLER_H
    
    #include<common/types.h>
    
    namespace MYYOS
    {
        namespace drivers
        {
            
            class RawDataHandler
            {
                public:
                    RawDataHandler();
                    ~RawDataHandler();
                    virtual bool HandleRawData(uint8_t* buffer, uint32_t size);
            };
            
        }
    }
    
    
#endif