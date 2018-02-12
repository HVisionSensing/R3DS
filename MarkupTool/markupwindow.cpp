#include "markupwindow.h"
#include "ui_markupwindow.h"


MarkupWindow::MarkupWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MarkupWindow)
{
    ui->setupUi(this);
}


MarkupWindow::~MarkupWindow()
{
    delete ui;
}


void MarkupWindow::showImage(const int &indImage)
{
    clearAll();

    bool isEmptyImages = (imagesPaths.size() == 0);
    bool isUpOutside = (indImage >= imagesPaths.size());
    bool isDownOutside = (indImage < 0);

    if (isEmptyImages || isUpOutside || isDownOutside)
        return;

    indOpenedFile = indImage;
    QImage image(imagesPaths[indImage]);
    ui->graphicsView->drawImage(image);

    QListWidgetItem *itemSelected = ui->listWidget->item(indOpenedFile);
    ui->listWidget->setItemSelected(itemSelected, true);
}


void MarkupWindow::showImage(const QString &imageName)
{
    for (int indImage = 0; indImage < imagesPaths.size(); indImage++)
        if (imageName == imagesPaths[indImage])
            showImage(indImage);
}


void MarkupWindow::clearAll()
{
    ui->graphicsView->clearBodyParts();
}


void MarkupWindow::clear()
{
    ui->graphicsView->clearBodyPart();
}



void MarkupWindow::save()
{
    if (indOpenedFile == -1)
        return;
    QString imagePath = imagesPaths[indOpenedFile];

    Body body;
    body.leftEye.corner = ui->graphicsView->getCorner(0);
    body.leftEye.up = ui->graphicsView->getUpPart(0);
    body.leftEye.down = ui->graphicsView->getDownPart(0);

    body.rightEye.corner = ui->graphicsView->getCorner(1);
    body.rightEye.up = ui->graphicsView->getUpPart(1);
    body.rightEye.down = ui->graphicsView->getDownPart(1);

    body.mouth.corner = ui->graphicsView->getCorner(2);
    body.mouth.up = ui->graphicsView->getUpPart(2);
    body.mouth.down = ui->graphicsView->getDownPart(2);

    QJsonDocument doc = LoaderJson::createJson(body);
    imagePath.remove(imagePath.size()-3, 3);
    imagePath += "json";
    LoaderJson::saveJson(doc, imagePath);
}



void MarkupWindow::loadShape(QString path)
{
    QFile file(path);
    if (!file.exists())
        return;

    QJsonDocument doc = LoaderJson::loadJson(path);
    Body body = LoaderJson::getBodyParts(doc);

    ui->graphicsView->updateBlocks(body);
    ui->graphicsView->changeBodyPart(indPart);
}


void MarkupWindow::on_actionLoad_images_triggered()
{
    imagesPaths = QFileDialog::getOpenFileNames(this, "load images", "/home/radiatus/Dataset/", "Images (*.png *.jpg)");
    ui->listWidget->clear();
    ui->listWidget->addItems(imagesPaths);
    showImage(0);
    QString path = imagesPaths[0];
    path.remove(path.size()-3, 3);
    path += "json";
    loadShape(path);
}


void MarkupWindow::on_actionNext_triggered()
{
    save();
    showImage(indOpenedFile+1);
    if (indOpenedFile == -1)
        return;
    QString path = imagesPaths[indOpenedFile];
    path.remove(path.size()-3, 3);
    path += "json";
    loadShape(path);
}


void MarkupWindow::on_actionPast_triggered()
{
    save();
    showImage(indOpenedFile-1);
    if (indOpenedFile == -1)
        return;
    QString path = imagesPaths[indOpenedFile];
    path.remove(path.size()-3, 3);
    path += "json";
    loadShape(path);
}


void MarkupWindow::on_actionClear_all_triggered()
{
    clearAll();
}


void MarkupWindow::on_actionSave_shape_triggered()
{
    save();
}


void MarkupWindow::on_actionSave_JSON_triggered()
{
    QString pathFile = QFileDialog::getOpenFileName(this, "loadFiles", "/home/radiatus/Dataset/", "*.json");
    loadShape(pathFile);
}


void MarkupWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    save();
    QString imageName = item->text();
    showImage(imageName);
    if (indOpenedFile == -1)
        return;
    QString path = imagesPaths[indOpenedFile];
    path.remove(path.size()-3, 3);
    path += "json";
    loadShape(path);

}


void MarkupWindow::on_actionLeft_Eye_triggered()
{
    indPart = 0;
    ui->graphicsView->changeBodyPart(indPart);
}


void MarkupWindow::on_actionRight_Eye_triggered()
{
    indPart = 1;
    ui->graphicsView->changeBodyPart(indPart);
}


void MarkupWindow::on_actionMouth_triggered()
{
    indPart = 2;
    ui->graphicsView->changeBodyPart(indPart);
}


void MarkupWindow::on_actionClear_triggered()
{
    clear();
}
