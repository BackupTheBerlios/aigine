/* Game Engine Design */



#ifndef AGEG_VIEWPORT_H

#define AGEG_VIEWPORT_H

/**
 * Einstellungen der Perspektive, Fenstergr��e, Linsenkr�mmung etc. werden vom
 * Viewport �bernommen.
 */
class ageG_Viewport
{

public:



    virtual void set();



    void setSize(float width, float height);

};

#endif //AGEG_VIEWPORT_H