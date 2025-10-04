

#ifndef __GRAPHICALOBJECTS__GRAPHICALOBJECT_TEMPLATES_CPP
    #define __GRAPHICALOBJECTS__GRAPHICALOBJECT_TEMPLATES_CPP

    namespace MYYOS
    {
        namespace graphicalobjects
        {

            template<int L, int T, int W, int H> GraphicalObjectMoverResizer<L,T,W,H>::GraphicalObjectMoverResizer(GraphicalObject* target)
                : drivers::mouse::MouseEventHandler()
            {
                this->target = target;
            }

            template<int L, int T, int W, int H> GraphicalObjectMoverResizer<L,T,W,H>::~GraphicalObjectMoverResizer()
            {
            }

            template<int L, int T, int W, int H> void GraphicalObjectMoverResizer<L,T,W,H>::OnMouseMoved(int8_t x, int8_t y)
            {
                if(L != 0 || T != 0)
                    target->Move( target->position.left + L*x, target->position.top + T*y);
                if(W != 0 || H != 0)
                    target->Resize( target->position.width + W*x, target->position.height + H*y);
            }

        }
    }

#endif
