#ifndef GRP_H
#define GRP_H

#include <QtCore>

class Grp
{
private:
    int framecount;
    int width;
    int height;

    QVector<QByteArray *> frames;

    Grp(int framecount, int width, int height);
    Grp(short framecount, short width, short height);

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

    int getWidth() {return width;}
    int getHeight() {return height;}
    int getFrameCount() {return framecount;}
    QByteArray * getFrame(int i) {return frames[i];}

    void insertFrame(int i);
    void insertFrame(int i, QByteArray qb);
    void copyFrame(int i);
    void deleteFrame(int i);
    void swapFrame(int i, int j);
    void upmostFrame(int i);
    void downmostFrame(int i);
};

#endif // GRP_H
