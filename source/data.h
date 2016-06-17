#ifndef DATA_H
#define DATA_H

#include "wpe.h"
#include "mapping.h"
#include "remapping.h"
#include "colorcycling.h"
#include "grp.h"


class Data
{
private:
    static Data * instance;

    /* objects
     * and index of current selected one of them */
    Grp *grp;
    int icGrp; // frame index

    QVector<Wpe *> wpes;
    int icWpe;

    QVector<Mapping *> mappings;
    int icMapping;

    QVector<Remapping *> remappings;
    int icRemapping;

    QVector<ColorCycling *> colorCyclings;
    int icColorCycling;


    char background;

    Data();
public:
    static Data * getInstance();

    void setGrp(Grp *grp) {this->grp = grp;}
    void appendWpe(Wpe *wpe) {wpes.append(wpe);}
    void appendMapping (Mapping *map) {mappings.append(map);}
    void appendRemapping (Remapping *rem) {remappings.append(rem);}
    void appendColorCycling (ColorCycling *cc) {colorCyclings.append(cc);}
    QVector<Wpe *> const &getWpes() const {return wpes;}
    QVector<Mapping *> const &getMappings() const {return mappings;}
    QVector<Remapping *> const &getRemappings() const {return remappings;}
    QVector<ColorCycling *> const &getColorCyclings() const {return colorCyclings;}

};

#endif // DATA_H
