/* Game Engine Design */



#ifndef VIEWPORT_H

#define VIEWPORT_H

/**
 * Einstellungen der Perspektive, Fenstergr��e, Linsenkr�mmung etc. werden vom Viewport �bernommen. 
 */
class Viewport
{

public:



    virtual void set();



    void setSize(float width, float height);

};

#endif //VIEWPORT_H