/* Game Engine Design */



#ifndef AGEGM_OBJECTTREE_H

#define AGEGM_OBJECTTREE_H


class ageGC_Object;



class ageGM_ObjectTree
{

private:



    /**
     * @clientCardinality 1
     * @supplierCardinality 1..*
     */

    ageGC_Object * lnkObject;

};

#endif //AGEGM_OBJECTTREE_H