#include "mainwindow.h"
#include <QGraphicsView>
#include "graphicscene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto view = new QGraphicsView;
    view->setScene(new GraphicScene);
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{

}
