/* Game Engine Design */



#ifndef MESHLIST_H

#define MESHLIST_H


class Mesh;



/** Liste von <class>Mesh</class> Objekten */

class MeshList {

private:



    /**
     * @supplierCardinality 1..*
     * @clientCardinality 1
     */

    Mesh * lnkMesh;

};

#endif //MESHLIST_H