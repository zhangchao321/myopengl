
#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include "mainwindow.h"
#include "glwidget.h"

static QString getGlString(QOpenGLFunctions *functions, GLenum name)
{
    if (const GLubyte *p = functions->glGetString(name))
        return QString::fromLatin1(reinterpret_cast<const char *>(p));
    return QString();
}

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );

    QSurfaceFormat format;
    format.setDepthBufferSize(16);
    QSurfaceFormat::setDefaultFormat(format);

    // Two top-level windows with two QOpenGLWidget children in each.
    // The rendering for the four QOpenGLWidgets happens on four separate threads.

    GLWidget topLevelGlWidget;
    QPoint pos = QApplication::desktop()->availableGeometry(&topLevelGlWidget).topLeft() + QPoint(200, 200);
    topLevelGlWidget.setWindowTitle(QStringLiteral("Threaded QOpenGLWidget example top level"));
    topLevelGlWidget.resize(200, 200);
    topLevelGlWidget.move(pos);
    topLevelGlWidget.show();

    const QString glInfo = getGlString(topLevelGlWidget.context()->functions(), GL_VENDOR)
        + QLatin1Char('/') + getGlString(topLevelGlWidget.context()->functions(), GL_RENDERER);

    const bool supportsThreading = !glInfo.contains(QLatin1String("nouveau"), Qt::CaseInsensitive)
        && !glInfo.contains(QLatin1String("ANGLE"), Qt::CaseInsensitive)
        && !glInfo.contains(QLatin1String("llvmpipe"), Qt::CaseInsensitive);

    const QString toolTip = supportsThreading ? glInfo : glInfo + QStringLiteral("\ndoes not support threaded OpenGL.");
    topLevelGlWidget.setToolTip(toolTip);

    QScopedPointer<MainWindow> mw1;
    QScopedPointer<MainWindow> mw2;
    if (!QApplication::arguments().contains(QStringLiteral("--single"))) {
        if (supportsThreading) {
            pos += QPoint(100, 100);
            mw1.reset(new MainWindow);
            mw1->setToolTip(toolTip);
            mw1->move(pos);
            mw1->setWindowTitle(QStringLiteral("Threaded QOpenGLWidget example #1"));
            mw1->show();
            pos += QPoint(100, 100);
            mw2.reset(new MainWindow);
            mw2->setToolTip(toolTip);
            mw2->move(pos);
            mw2->setWindowTitle(QStringLiteral("Threaded QOpenGLWidget example #2"));
            mw2->show();
        } else {
            qWarning() << toolTip;
        }
    }

    return a.exec();
}
