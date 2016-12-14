#ifndef SHANKVIEW_H
#define SHANKVIEW_H

#include "ShankMap.h"

#include <QMutex>
#include <QAbstractScrollArea>

#ifdef OPENGL54
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#else
#include <QGLWidget>
#include <QGLFunctions>
#endif

#undef max  // inherited from WinDef.h via QGLWidget

/* ---------------------------------------------------------------- */
/* SColor --------------------------------------------------------- */
/* ---------------------------------------------------------------- */

struct SColor {
    quint8  r, g, b;
    SColor() : r(0), g(0), b(0) {}
    SColor( quint8 c ) : r(c), g(c), b(c) {}
    SColor( quint8 r, quint8 g, quint8 b ) : r(r), g(g), b(b) {}
};

/* ---------------------------------------------------------------- */
/* ShankView ------------------------------------------------------ */
/* ---------------------------------------------------------------- */

#ifdef OPENGL54
class ShankView : public QOpenGLWidget, protected QOpenGLFunctions
#else
class ShankView : public QGLWidget, protected QGLFunctions
#endif
{
    Q_OBJECT

    friend class ShankScroll;

// Indexing
// --------
// Rects vR and colors vC store entry (s,c,r) at:
//  [s*(nc*nr) + c*(nr) + r] * sizeof(entry).

private:
    QVector<SColor>         lut;
    const ShankMap          *smap;
    QMap<ShankMapDesc,uint> ISM;
    QVector<float>          vR;
    QVector<SColor>         vC;
    mutable QMutex          dataMtx;
    float                   shkWid,
                            hlfWid,
                            pmrg,
                            colWid;
    int                     rowPix,
                            slidePos,
                            vBot,
                            vTop,
                            sel;
    bool                    inited;

public:
    ShankView( QWidget *parent = 0 );

    void setRowPix( int y )     {QMutexLocker ml( &dataMtx ); rowPix = y;}
    void setSlider( int y )     {QMutexLocker ml( &dataMtx ); slidePos = y;}

    void setShankMap( const ShankMap *map );
    const ShankMap *getSmap()   {return smap;}
    void setSel( int ic );
    int getSel()                {return sel;}

    void colorPads( const QVector<double> &val, double rngMax );

signals:
    void cursorOver( int ic, bool shift );
    void lbutClicked( int ic, bool shift );

public slots:
#ifdef OPENGL54
    void updateNow()    {update();}
#else
    void updateNow()    {updateGL();}
#endif

protected:
    void initializeGL();
    void resizeGL( int w, int h );
    void paintGL();

    void mouseMoveEvent( QMouseEvent *evt );
    void mousePressEvent( QMouseEvent *evt );

private:
    void loadLut();
    float viewportPix();
    float spanPix();
    void setClipping();
    void resizePads();
    void drawTips();
    void drawShks();
    void drawTops();
    void drawPads();
    void drawSel();
    void drawTri( float l, float t, float w, float h, SColor c );
    void drawRect( float l, float t, float w, float h, SColor c );
    bool evt2Pad( int &s, int &c, int &r, const QMouseEvent *evt );
    int getSelY();
};

/* ---------------------------------------------------------------- */
/* ShankScroll ---------------------------------------------------- */
/* ---------------------------------------------------------------- */

class ShankScroll : public QAbstractScrollArea
{
    Q_OBJECT

public:
    ShankView   *theV;

public:
    ShankScroll( QWidget *parent = 0 );

    void setRowPix( int rPix );
    void scrollTo( int y );
    void adjustLayout();

public slots:
    void scrollToSelected();

protected:
    virtual void resizeEvent( QResizeEvent *e );
    virtual void scrollContentsBy( int dx, int dy );
    virtual bool viewportEvent( QEvent *e );
};

#endif  // SHANKVIEW_H

