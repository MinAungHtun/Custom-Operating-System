
#ifndef __APPLICATIONS__myynet_H
    #define __APPLICATIONS__myynet_H

    #include<common/string.h>
    #include<graphicalobjects/window.h>
    #include<graphicalobjects/textbox.h>
    #include<graphicalobjects/button.h>
    #include<graphicalobjects/label.h>

    #include<protocols/ethernet/transmissioncontrolprotocol.h>

    namespace MYYOS
    {
        namespace applications
        {

            class myynetClient;

            class ConnectButtonEventHandler : public graphicalobjects::ButtonEventHandler
            {
                protected:
                    myynetClient* target;
                public:
                    ConnectButtonEventHandler(myynetClient* target);
                    ~ConnectButtonEventHandler();
                    void OnClick(graphicalobjects::Button* sender);
            };

            class SendButtonEventHandler : public graphicalobjects::ButtonEventHandler
            {
                protected:
                    myynetClient* target;
                public:
                    SendButtonEventHandler(myynetClient* target);
                    ~SendButtonEventHandler();
                    void OnClick(graphicalobjects::Button* sender);
            };

            class myynetClient : public graphicalobjects::GraphicalObjectContainer,
                                 public protocols::ethernet::TransmissionControlProtocolPayloadHandler
            {
                protected:
                    ConnectButtonEventHandler connectButtonEventHandler;
                    SendButtonEventHandler sendButtonEventHandler;

                    graphicalobjects::Label ipAddressLabel;
                    graphicalobjects::TextBox ipAddressTextBox;
                    graphicalobjects::Button connectButton;

                    graphicalobjects::TextBox messageTextBox;
                    graphicalobjects::Button sendButton;

                    protocols::ethernet::TransmissionControlProtocolConnection* connection;
                    protocols::ethernet::TransmissionControlProtocolHandler* transmissionControlProtocolHandler;

                public:
                    myynetClient(uint32_t width, uint32_t height, protocols::ethernet::TransmissionControlProtocolHandler* handler);
                    ~myynetClient();

                    void ConnectButtonClicked();
                    void SendButtonClicked();
                    virtual void HandleTransmissionControlProtocolPayload(protocols::ethernet::TransmissionControlProtocolConnection* connection, uint8_t* data, uint32_t length);
            };

        }
    }

#endif
