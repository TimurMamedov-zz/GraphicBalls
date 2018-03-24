#include "mainwindow.h"
#include <QGraphicsView>
#include "graphicscene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto view = new QGraphicsView;
    auto scene = new GraphicScene;
    auto rect = scene->sceneRect().toAlignedRect();
    view->setScene(new GraphicScene);
    view->setFixedSize(rect.width()+5, rect.height()+5);
    setCentralWidget(view);
    setFixedSize(view->size());
}

MainWindow::~MainWindow()
{

}
