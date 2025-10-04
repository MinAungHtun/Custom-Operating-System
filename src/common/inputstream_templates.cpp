
#ifndef __COMMON__INPUTSTREAM_TEMPLATES_CPP
    #define __COMMON__INPUTSTREAM_TEMPLATES_CPP

    #include<common/vector.h>

    using namespace MYYOS::common;

    namespace MYYOS
    {
        namespace common
        {

            template<class T> InputStreamEventHandler<T>::InputStreamEventHandler()
            {
            }

            template<class T> InputStreamEventHandler<T>::~InputStreamEventHandler()
            {
                while(!streams.empty())
                    (*streams.begin())->DisconnectEventHandler(this);
            }

            template<class T> void InputStreamEventHandler<T>::OnRead(T)
            {
            }

            template<class T> void InputStreamEventHandler<T>::OnEndOfStream(BasicInputStream<T>* stream)
            {
                streams.erase(stream);
            }




            template<class T> InputStreamBuffer<T>::InputStreamBuffer(T eventFireElement, T terminationElement)
                : InputStreamProcessor<T,T*>::InputStreamProcessor()
            {
                offset = 0;
                this->eventFireElement = eventFireElement;
                this->terminationElement = terminationElement;
            }

            template<class T> InputStreamBuffer<T>::~InputStreamBuffer()
            {
            }

            template<class T> void InputStreamBuffer<T>::OnRead(T c)
            {
                if(c == eventFireElement)
                {
                    Flush();
                }
                else
                {
                    if(offset >= 10238)
                        Flush();
                    buffer[offset++] = c;
                }
            }

            template<class T> void InputStreamBuffer<T>::OnEndOfStream(BasicInputStream<T>* stream)
            {
                if(offset > 0)
                    Flush();
                InputStreamProcessor<T,T*>::OnEndOfStream(stream);
            }

            template<class T> void InputStreamBuffer<T>::Flush()
            {
                InputStreamEventHandler<T*>* handler = this->handler;
                if(handler != 0)
                {
                    buffer[offset] = terminationElement;
                    handler->OnRead(&buffer[0]);
                }
                offset = 0;
            }




            template<class T, class U> InputStreamProcessor<T,U>::InputStreamProcessor()
                : InputStreamEventHandler<T>::InputStreamEventHandler(),
                BasicInputStream<U>::BasicInputStream()
            {
            }

            template<class T, class U> InputStreamProcessor<T,U>::InputStreamProcessor(InputStreamEventHandler<U>* pipe)
                : InputStreamEventHandler<T>::InputStreamEventHandler(),
                BasicInputStream<U>::BasicInputStream(pipe)
            {
            }

            template<class T, class U> InputStreamProcessor<T,U>::~InputStreamProcessor()
            {
            }

            template<class T, class U> void InputStreamProcessor<T,U>::OnEndOfStream(BasicInputStream<T>* stream)
            {
                for(typename Vector<InputStreamEventHandler<U>*>::iterator handler = this->handlers.begin(); handler != this->handlers.end(); ++handler)
                    (*handler)->OnEndOfStream(this);
                InputStreamEventHandler<T>::OnEndOfStream(stream);
            }




            template<class T> InputStreamSocket<T>::InputStreamSocket()
                : InputStreamProcessor<T,T>::InputStreamProcessor()
            {
            }

            template<class T> InputStreamSocket<T>::InputStreamSocket(InputStreamEventHandler<T>* handler)
                : InputStreamProcessor<T,T>::InputStreamProcessor(handler)
            {
            }

            template<class T> InputStreamSocket<T>::~InputStreamSocket()
            {
            }

            template<class T> void InputStreamSocket<T>::OnRead(T read)
            {
                for(typename Vector<InputStreamEventHandler<T>*>::iterator handler = this->handlers.begin(); handler != this->handlers.end(); ++handler)
                    (*handler)->OnRead(read);
            }




            template<class T> BasicInputStream<T>::BasicInputStream()
            {
            }

            template<class T> BasicInputStream<T>::BasicInputStream(InputStreamEventHandler<T>* handler)
            {
                ConnectEventHandler(handler);
            }

            template<class T> BasicInputStream<T>::~BasicInputStream()
            {
                while(!handlers.empty())
                    DisconnectEventHandler(*(handlers.begin()));
            }

            template<class T> string BasicInputStream<T>::GetName()
            {
                return "InputStream";
            }

            template<class T> void BasicInputStream<T>::ConnectEventHandler(InputStreamEventHandler<T>* handler)
            {
                if(handlers.find(handler) == handlers.end())
                    handlers.push_back(handler);
            }

            template<class T> void BasicInputStream<T>::DisconnectEventHandler(InputStreamEventHandler<T>* handler)
            {
                if(handlers.find(handler) != handlers.end())
                {
                    handlers.erase(handler);
                    handler->OnEndOfStream(this);
                }
            }





            template<class T, class U> BasicInputStream<U>& operator>>(BasicInputStream<T>& source, InputStreamProcessor<T, U>& processor)
            {
                source.ConnectEventHandler(&processor);
                return processor;
            }

            template<class T> void operator>>(BasicInputStream<T>& source, InputStreamEventHandler<T>& handler)
            {
                source.ConnectEventHandler(&handler);
            }



        }
    }

#endif