
#ifndef __APPLICATIONS__APPLICATION_H
    #define __APPLICATIONS__APPLICATION_H

    #include<graphicalobjects/graphicalobject.h>
    #include<graphicalobjects/desktop.h>
    #include<graphicalobjects/window.h>

    namespace MYYOS
    {
        namespace applications
        {

            class Application : public graphicalobjects::GraphicalObject
            {
                public:
                    Application();
                    ~Application();
            };

        }
    }

#endif
