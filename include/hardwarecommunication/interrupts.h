
#ifndef __HARDWARECOMMUNICATION__INTERRUPTMANAGER_H
    #define __HARDWARECOMMUNICATION__INTERRUPTMANAGER_H

    #include <globaldescriptortable.h>
    #include <common/types.h>
    #include <common/outputstream.h>
    #include <hardwarecommunication/port.h>

    namespace MYYOS
    {
        namespace hardwarecommunication
        {

            class InterruptManager;
            class InterruptHandler
            {
                friend class InterruptManager;
                private:
                    uint8_t interrupt;
                    InterruptManager* owner;

                protected:
                    InterruptHandler(uint8_t interrupt, InterruptManager* owner = 0);
                    // FIXME: Must be virtual (currently isnt because the kernel has no memory management yet)
                    ~InterruptHandler();
                    virtual void HandleInterrupt();
            };


            class InterruptManager : public common::InputStream
            {
                friend class InterruptHandler;
                protected:

                    struct GateDescriptor
                    {
                        uint16_t handlerAddressLowBits;
                        uint16_t gdt_codeSegmentSelector;
                        uint8_t reserved;
                        uint8_t access;
                        uint16_t handlerAddressHighBits;
                    } __attribute__((packed));
                    static GateDescriptor interruptDescriptorTable[256];

                    struct InterruptDescriptorTablePointer
                    {
                        uint16_t size;
                        uint32_t base;
                    } __attribute__((packed));

                    uint16_t hardwareInterruptOffset;
                    static common::OutputStream* errorMessages;
                    static InterruptManager* ActiveInterruptManager;
                    InterruptHandler* handlers[256];
                    static void SetInterruptDescriptorTableEntry(uint8_t interrupt,
                        uint16_t codeSegmentSelectorOffset, void (*handler)(),
                        uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType);

                    static void InterruptIgnore();

                    static void HandleInterruptRequest0x00();
                    static void HandleInterruptRequest0x01();
                    static void HandleInterruptRequest0x02();
                    static void HandleInterruptRequest0x03();
                    static void HandleInterruptRequest0x04();
                    static void HandleInterruptRequest0x05();
                    static void HandleInterruptRequest0x06();
                    static void HandleInterruptRequest0x07();
                    static void HandleInterruptRequest0x08();
                    static void HandleInterruptRequest0x09();
                    static void HandleInterruptRequest0x0A();
                    static void HandleInterruptRequest0x0B();
                    static void HandleInterruptRequest0x0C();
                    static void HandleInterruptRequest0x0D();
                    static void HandleInterruptRequest0x0E();
                    static void HandleInterruptRequest0x0F();
                    static void HandleInterruptRequest0x31();

                    static void HandleException0x00();
                    static void HandleException0x01();
                    static void HandleException0x02();
                    static void HandleException0x03();
                    static void HandleException0x04();
                    static void HandleException0x05();
                    static void HandleException0x06();
                    static void HandleException0x07();
                    static void HandleException0x08();
                    static void HandleException0x09();
                    static void HandleException0x0A();
                    static void HandleException0x0B();
                    static void HandleException0x0C();
                    static void HandleException0x0D();
                    static void HandleException0x0E();
                    static void HandleException0x0F();
                    static void HandleException0x10();
                    static void HandleException0x11();
                    static void HandleException0x12();
                    static void HandleException0x13();

                    static uint32_t HandleInterrupt(uint8_t interrupt, uint32_t esp);
                    static uint32_t HandleInterruptRequest(uint32_t esp);
                    void DoHandleInterruptRequest(uint8_t interrupt);

                    Port8BitSlow programmableInterruptControllerMasterCommandPort;
                    Port8BitSlow programmableInterruptControllerMasterDataPort;
                    Port8BitSlow programmableInterruptControllerSlaveCommandPort;
                    Port8BitSlow programmableInterruptControllerSlaveDataPort;

                public:
                    InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* globalDescriptorTable, common::OutputStream* handler);
                    ~InterruptManager();
                    uint16_t HardwareInterruptOffset();
                    void SetInterruptHandler(uint8_t interrupt, InterruptHandler* handler);
                    void UnsetInterruptHandler(uint8_t interrupt);
                    void Activate();
                    void Deactivate();
            };

        }
    }
#endif