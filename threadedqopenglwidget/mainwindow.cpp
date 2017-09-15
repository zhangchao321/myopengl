
#include "mainwindow.h"
#include "glwidget.h"

MainWindow::MainWindow()
{
    setMinimumSize(800, 400);
    GLWidget *glwidget1 = new GLWidget(this);
    glwidget1->resize(400, 400);

    GLWidget *glwidget2 = new GLWidget(this);
    glwidget2->resize(400, 400);
    glwidget2->move(400, 0);
}
