/* Game Engine Design */

#ifndef MESHLIST_H
#define MESHLIST_H
#include "Model/List.h"
class Mesh;

/** Liste von <class>Mesh</class> Objekten */
class MeshList : public List
{
private:

    /**
     * @supplierCardinality 1..*
     * @clientCardinality 1
     */
    Mesh * lnkMesh;
};
#endif //MESHLIST_H