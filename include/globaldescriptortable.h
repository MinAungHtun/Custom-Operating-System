




#ifndef __GLOBALDESCRIPTORTABLE_H
    #define __GLOBALDESCRIPTORTABLE_H

    #include<common/types.h>
    #include<multiboot.h>
    
    namespace MYYOS
    {

        class GlobalDescriptorTable
        {
            public:

                class SegmentSelector
                {
                    private:
                        uint16_t limit_lo;
                        uint16_t base_lo;
                        uint8_t base_hi;
                        uint8_t type;
                        uint8_t limit_hi;
                        uint8_t base_vhi;

                    public:
                        SegmentSelector(uint32_t base, uint32_t limit, uint8_t type);
                        uint32_t Base();
                        uint32_t Limit();
                } __attribute__((packed));

            private:
                SegmentSelector nullSegmentSelector;
                SegmentSelector unusedSegmentSelector;
                SegmentSelector codeSegmentSelector;
                SegmentSelector dataSegmentSelector;
                SegmentSelector taskStateSegmentSelector;

            public:

                GlobalDescriptorTable(const multiboot_info& multibootHeader);
                ~GlobalDescriptorTable();

                uint16_t CodeSegmentSelector();
                uint16_t DataSegmentSelector();
                uint16_t TaskStateSegmentSelector();
        };

    }

#endif