
#ifndef __GRAPHICALOBJECTS__LABEL_H
    #define __GRAPHICALOBJECTS__LABEL_H

    #include<common/string.h>
    #include<common/font.h>
    #include<graphicalobjects/graphicalobject.h>

    namespace MYYOS
    {
        namespace graphicalobjects
        {


            class Label : public GraphicalObject
            {
                protected:
                    char text[256];
                public:
                    Label(int32_t left, int32_t top, uint32_t width, uint32_t height, common::string);
                    ~Label();

                    void Draw(common::GraphicsContext* context, common::Rectangle<int>& area);
                    void SetText(string text);
                    Font font;
            };


        }
    }


#endif

