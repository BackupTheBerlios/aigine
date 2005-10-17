/* Game Engine Design */



#ifndef AGEGM_VIEWPORT_H

#define AGEGM_VIEWPORT_H

/**
 * Einstellungen der Perspektive, Fenstergröße, Linsenkrümmung etc. werden vom
 * Viewport übernommen.
 */
class ageGM_Viewport
{

public:



    virtual void set();



    void setSize(float width, float height);

};

#endif //AGEGM_VIEWPORT_H