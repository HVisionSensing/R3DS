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


void MarkupWindow::on_actionLoad_images_triggered()
{
    imagesPaths = QFileDialog::getOpenFileNames(this, "load images", "/home/radiatus/Dataset/", "Images (*.png *.jpg)");
    ui->listWidget->clear();
    ui->listWidget->addItems(imagesPaths);
    showImage(0);
}


void MarkupWindow::on_actionNext_triggered()
{
    showImage(indOpenedFile+1);
}


void MarkupWindow::on_actionPast_triggered()
{
    showImage(indOpenedFile-1);
}


void MarkupWindow::on_actionClear_all_triggered()
{
    clearAll();
}


void MarkupWindow::on_actionSave_shape_triggered()
{
    QString imagePath = imagesPaths[indOpenedFile];

    BodyParts body;
    body.leftEyeCorner = ui->graphicsView->getCorner(0);
    body.leftEyeUp = ui->graphicsView->getUpPart(0);
    body.leftEyeDown = ui->graphicsView->getDownPart(0);

    body.rightEyeCorner = ui->graphicsView->getCorner(1);
    body.rightEyeUp = ui->graphicsView->getUpPart(1);
    body.rightEyeDown = ui->graphicsView->getDownPart(1);

    body.mouthCorner = ui->graphicsView->getCorner(2);
    body.mouthUp = ui->graphicsView->getUpPart(2);
    body.mouthDown = ui->graphicsView->getDownPart(2);

    QJsonDocument doc = LoaderJson::createJson(body);
    imagePath.remove(imagePath.size()-3, 3);
    imagePath += "json";
    LoaderJson::saveJson(doc, imagePath);
}


void MarkupWindow::on_actionSave_JSON_triggered()
{
    QString pathFile = QFileDialog::getOpenFileName(this, "loadFiles", "/home/radiatus/Dataset/", "*.json");
    QJsonDocument doc = LoaderJson::loadJson(pathFile);
    BodyParts body = LoaderJson::getBodyParts(doc);

    ui->graphicsView->updateBlocks(body);
}


void MarkupWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString imageName = item->text();
    showImage(imageName);
}


void MarkupWindow::on_actionLeft_Eye_triggered()
{
    ui->graphicsView->changeBodyPart(0);
}


void MarkupWindow::on_actionRight_Eye_triggered()
{
    ui->graphicsView->changeBodyPart(1);
}


void MarkupWindow::on_actionMouth_triggered()
{
    ui->graphicsView->changeBodyPart(2);
}


void MarkupWindow::on_actionClear_triggered()
{
    clear();
}
