/* Game Engine Design */



#ifndef AGEGM_VIEWPORT_H

#define AGEGM_VIEWPORT_H

/**
 * Einstellungen der Perspektive, Fenstergr��e, Linsenkr�mmung etc. werden vom
 * Viewport �bernommen.
 */
class ageGM_Viewport
{

public:



    virtual void set();



    void setSize(float width, float height);

};

#endif //AGEGM_VIEWPORT_H