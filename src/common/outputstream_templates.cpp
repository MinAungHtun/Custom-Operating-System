
#ifndef __COMMON__OUTPUTSTREAM_TEMPLATES_CPP
    #define __COMMON__OUTPUTSTREAM_TEMPLATES_CPP

    #include<common/vector.h>

    namespace MYYOS
    {
        namespace common
        {

            template<class T> BasicOutputStream<T>::BasicOutputStream()
            {
            }

            template<class T> BasicOutputStream<T>::~BasicOutputStream()
            {
            }

            template<class T> string BasicOutputStream<T>::GetName()
            {
                return "Unknown OutputStream";
            }

            template<class T> void BasicOutputStream<T>::OnRead(T c)
            {
                Write(c);
                for(typename Vector<InputStreamEventHandler<T>*>::iterator handler = this->handlers.begin(); handler != this->handlers.end(); ++handler)
                    (*handler)->OnRead(c);
            }

            template<class T> void BasicOutputStream<T>::OnEndOfStream(BasicInputStream<T>* stream)
            {
                Close();
                for(typename Vector<InputStreamEventHandler<T>*>::iterator handler = this->handlers.begin(); handler != this->handlers.end(); ++handler)
                    (*handler)->OnEndOfStream(this);
                InputStreamProcessor<T,T>::OnEndOfStream(stream);
            }

            template<class T> void BasicOutputStream<T>::Close()
            {
            }

            template<class T> void BasicOutputStream<T>::Write(T)
            {
            }

            template<class T> BasicOutputStream<T>& BasicOutputStream<T>::operator<<(T t)
            {
                Write(t);
                return *this;
            }




            template<class T> NullStream<T>::NullStream()
                : BasicOutputStream<T>::BasicOutputStream()
            {
            }

            template<class T> NullStream<T>::~NullStream()
            {
            }

            template<class T> string NullStream<T>::GetName()
            {
                return "NULL";
            }

        }
    }

#endif
