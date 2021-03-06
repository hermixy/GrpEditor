#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    grpImage = NULL;

    /* load data files */
    data = Data::getInstance();
    loadData();

    ui->grpImageScrollArea->setBackgroundRole(QPalette::Dark);
    connect(ui->act_open_grp, SIGNAL(triggered()), this, SLOT(loadGrp()));
    connect(ui->act_pallete, SIGNAL(toggled(bool)), this, SLOT(openPallete()));
    connect(data, SIGNAL(grpChanged(int, int, char *)),
            this, SLOT(updatePixelData(int, int, char *)));
    connect(data, SIGNAL(colorTableChanged(QVector<QRgb>)),
            this, SLOT(updatePallete(QVector<QRgb>)));

    connect(ui->frameListWidget, SIGNAL(currentRowChanged(int)),
            data, SLOT(setGrpIndex(int)));

    connect(ui->newFrameButton, SIGNAL(clicked(bool)),
            this, SLOT(frame_new()));
    connect(ui->loadFrameButton, SIGNAL(clicked(bool)),
            this, SLOT(frame_load()));
    connect(ui->saveFrameButton, SIGNAL(clicked(bool)),
            this, SLOT(frame_save()));
    connect(ui->copyFrameButton, SIGNAL(clicked(bool)),
            this, SLOT(frame_copy()));
    connect(ui->deleteFrameButton, SIGNAL(clicked(bool)),
            this, SLOT(frame_delete()));
    connect(ui->upFrameButton, SIGNAL(clicked(bool)),
            this, SLOT(frame_up()));
    connect(ui->downFrameButton, SIGNAL(clicked(bool)),
            this, SLOT(frame_down()));
    connect(ui->upmostFrameButton, SIGNAL(clicked(bool)),
            this, SLOT(frame_upmost()));
    connect(ui->downmostFrameButton, SIGNAL(clicked(bool)),
            this, SLOT(frame_downmost()));

    show();
    palleteWindow = NULL;
    openPallete();
}

void MainWindow::loadData()
{
    loadWpe();
    loadMapping();
    loadRemapping();
    loadColorCycling();
    data->updateColorTable();
}

void MainWindow::loadWpe()
{

    /* wpe */
    data->appendWpe(Wpe::loadDefault());
    QDir dir("data/wpe/");
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QStringList filters;
    filters << "*.wpe";
    dir.setNameFilters(filters);
    QDirIterator iter(dir);
    while (iter.hasNext()) {
        iter.next();
        Wpe *wpe = Wpe::load(iter.fileInfo().completeBaseName(),
                             iter.filePath());
        if (wpe == NULL) {
            /* fail */
        } else {
            data->appendWpe(wpe);
        }
    }
}

void MainWindow::loadMapping()
{
    data->appendMapping(Mapping::loadDefault());
    QDir dir("data/mapping/");
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QStringList filters;
    filters << "*.mapping";
    dir.setNameFilters(filters);
    QDirIterator iter(dir);
    while (iter.hasNext()) {
        iter.next();
        Mapping *mapping = Mapping::load(iter.fileInfo().completeBaseName(),
                             iter.filePath());
        if (mapping == NULL) {
            /* fail */
        } else {
            data->appendMapping(mapping);
        }
    }
}

void MainWindow::loadRemapping()
{
    data->appendRemapping(Remapping::loadDefault());
    QDir dir("data/remapping/");
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QStringList filters;
    filters << "*.remapping";
    dir.setNameFilters(filters);
    QDirIterator iter(dir);
    while (iter.hasNext()) {
        iter.next();
        Remapping *remapping = Remapping::load(iter.fileInfo().completeBaseName(),
                             iter.filePath());
        if (remapping == NULL) {
            /* fail */
        } else {
            data->appendRemapping(remapping);
        }
    }
}

void MainWindow::loadColorCycling()
{
    data->appendColorCycling(ColorCycling::loadDefault());
    QDir dir("data/colorcycling/");
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QStringList filters;
    filters << "*.colorcycling";
    dir.setNameFilters(filters);
    QDirIterator iter(dir);
    while (iter.hasNext()) {
        iter.next();
        ColorCycling *colorCycling = ColorCycling::load(iter.fileInfo().completeBaseName(),
                             iter.filePath());
        if (colorCycling == NULL) {
            /* fail */
        } else {
            data->appendColorCycling(colorCycling);
        }
    }
}

void MainWindow::loadGrp()
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::loadGrp";
#endif
    QString fname = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                tr(""),
                tr("Grp files (*.grp)"));
    qDebug() << fname;

    Grp *new_grp = Grp::load(fname);
    if (new_grp == NULL) {
        QMessageBox::critical(this, "Error", "Error happened in grp loading");
        return;
    }

    Grp *old_grp = data->getGrp();
    ui->frameListWidget->blockSignals(true);
    if (old_grp != NULL) {
        delete old_grp;
        ui->frameListWidget->clear();
    }
    for (int i=0; i<new_grp->getFrameCount(); i++) {
        ui->frameListWidget->addItem(QString::asprintf("Frame #%u", i));
    }
    data->setGrp(new_grp);
    ui->frameListWidget->setCurrentRow(0);
    ui->frameListWidget->blockSignals(false);
}

void MainWindow::saveGrp()
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::saveGrp";
#endif

}

void MainWindow::openPallete()
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::openPallete";
#endif

    if (palleteWindow == NULL) {
        palleteWindow = new PalleteWindow(this);
        connect(palleteWindow, SIGNAL(closing()),
                this, SLOT(palleteClosed()));
        palleteWindow->show();
    } else {
        palleteWindow->close();
    }
}

void MainWindow::palleteClosed()
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::palleteClosed";
#endif
    palleteWindow = NULL;
}

void MainWindow::updatePixelData(int width, int height, char *frame)
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::updatePixelData";
#endif
    if (grpImage == NULL) {
        grpImage = new QImage(width, height, QImage::Format_Indexed8);
        grpImage->setColorTable(data->getColorTable());
    } else if ((grpImage->width() != width)
               || (grpImage->height() != height)) {
        delete grpImage;
        grpImage = new QImage(width, height, QImage::Format_Indexed8);
        grpImage->setColorTable(data->getColorTable());
    }
    for (int i=0; i<height; i++) {
        memcpy(grpImage->scanLine(i), frame+i*width, width);
    }
    grpPixmap.convertFromImage(*grpImage);
    ui->grpImageLabel->setPixmap(grpPixmap);
}

void MainWindow::updatePallete(QVector<QRgb> colorTable)
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::updatePallete";
#endif
    if (grpImage == NULL)
        return;
    grpImage->setColorTable(colorTable);
    grpPixmap.convertFromImage(*grpImage);
    ui->grpImageLabel->setPixmap(grpPixmap);
}

void MainWindow::frame_new()
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::frame_new";
#endif
    Grp *grp = data->getGrp();
    if (grp == NULL)
        return;

    int index = data->getGrpIndex();
    grp->insertFrame(index+1);
    ui->frameListWidget->addItem(QString::asprintf("Frame #%u", grp->getFrameCount()-1));
    ui->frameListWidget->setCurrentRow(index+1);
}

void MainWindow::frame_load()
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::frame_load";
#endif
}

void MainWindow::frame_save()
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::frame_save";
#endif
}

void MainWindow::frame_copy()
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::frame_copy";
#endif
    Grp *grp = data->getGrp();
    if (grp == NULL)
        return;

    int index = data->getGrpIndex();
    grp->copyFrame(index);
    ui->frameListWidget->addItem(QString::asprintf("Frame #%u", grp->getFrameCount()-1));
    ui->frameListWidget->setCurrentRow(index+1);
}

void MainWindow::frame_delete()
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::frame_delete";
#endif

    Grp *grp = data->getGrp();
    if (grp == NULL)
        return;

    if (grp->getFrameCount() == 1)
        return;
    int index = data->getGrpIndex();
    grp->deleteFrame(index);
    delete ui->frameListWidget->takeItem(grp->getFrameCount());
    ui->frameListWidget->setCurrentRow((index == 0)?0:index-1);
}

void MainWindow::frame_up()
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::frame_up";
#endif
    Grp *grp = data->getGrp();
    if (grp == NULL)
        return;

    int index = data->getGrpIndex();
    if (index == 0)
        return;
    grp->swapFrame(index, index-1);
    ui->frameListWidget->setCurrentRow(index-1);
}

void MainWindow::frame_down()
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::frame_down";
#endif
    Grp *grp = data->getGrp();
    if (grp == NULL)
        return;

    int index = data->getGrpIndex();
    if (index == grp->getFrameCount()-1)
        return;
    grp->swapFrame(index, index+1);
    ui->frameListWidget->setCurrentRow(index+1);
}

void MainWindow::frame_upmost()
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::frame_upmost";
#endif
    Grp *grp = data->getGrp();
    if (grp == NULL)
        return;

    int index = data->getGrpIndex();
    if (index == 0)
        return;
    grp->upmostFrame(index);
    ui->frameListWidget->setCurrentRow(0); // @Think
}

void MainWindow::frame_downmost()
{
#ifdef QT_DEBUG
    qDebug() << "SLOT MainWindow::frame_downmost";
#endif
    Grp *grp = data->getGrp();
    if (grp == NULL)
        return;

    int index = data->getGrpIndex();
    if (index == grp->getFrameCount()-1)
        return;
    grp->downmostFrame(index);
    ui->frameListWidget->setCurrentRow(grp->getFrameCount()-1); // @Think
}


MainWindow::~MainWindow()
{
    delete ui;
    if (palleteWindow != NULL)
        delete palleteWindow;

    if (grpImage != NULL)
        delete grpImage;
}
