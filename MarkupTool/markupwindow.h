#ifndef MARKUPWINDOW_H
#define MARKUPWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFileDialog>

#include "loaderjson.h"
#include "markupview.h"
#include "bodyparts.h"

namespace Ui {
class MarkupWindow;
}

class MarkupWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MarkupWindow(QWidget *parent = 0);
    ~MarkupWindow();

    void showImage(const int &indImage);
    void showImage(const QString &nameImage);
    void clearAll();
    void clear();
    void save();
    void loadShape(QString path);

private slots:
    void on_actionLoad_images_triggered();
    void on_actionNext_triggered();
    void on_actionPast_triggered();
    void on_actionClear_all_triggered();
    void on_actionSave_shape_triggered();
    void on_actionSave_JSON_triggered();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_actionLeft_Eye_triggered();
    void on_actionRight_Eye_triggered();
    void on_actionMouth_triggered();

    void on_actionClear_triggered();

private:
    Ui::MarkupWindow *ui = nullptr;
    QStringList imagesPaths;
    int indOpenedFile = -1;
    int indPart = 0;
};

#endif // MARKUPWINDOW_H
