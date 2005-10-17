/* Game Engine Design */



#ifndef AGEG_VIEWPORT_H

#define AGEG_VIEWPORT_H

/**
 * Einstellungen der Perspektive, Fenstergröße, Linsenkrümmung etc. werden vom
 * Viewport übernommen.
 */
class ageG_Viewport
{

public:



    virtual void set();



    void setSize(float width, float height);

};

#endif //AGEG_VIEWPORT_H