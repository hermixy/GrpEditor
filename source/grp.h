#ifndef GRP_H
#define GRP_H

#include <QtCore>

class Grp
{
private:
    unsigned int framecount;
    unsigned int width;
    unsigned int height;

    QVector<QByteArray *> frames;

    Grp(unsigned int framecount, unsigned int width, unsigned int height);
    Grp(unsigned short framecount, unsigned short width, unsigned short height);

    struct GrpHeader {
        unsigned short count;
        unsigned short width;
        unsigned short height;
    };
    struct GrpFrameHeader {
        unsigned char x, y, w, h;
        unsigned int offset;
    };

public:
    ~Grp();
    static Grp * load(QString fname);
};

#endif // GRP_H
