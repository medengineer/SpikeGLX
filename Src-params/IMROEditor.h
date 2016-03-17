#ifndef IMROEDITOR_H
#define IMROEDITOR_H

#include <QObject>

namespace Ui {
class IMROEditor;
}

struct IMROTbl;

class QDialog;

/* ---------------------------------------------------------------- */
/* Types ---------------------------------------------------------- */
/* ---------------------------------------------------------------- */

class IMROEditor : public QObject
{
    Q_OBJECT

private:
    QDialog         *edDlg;
    Ui::IMROEditor  *edUI;
    IMROTbl         *R0,
                    *R;
    QString         inFile,
                    R0File,
                    lastDir;
    int             probe;

public:
    IMROEditor( QObject *parent, int probe );
    virtual ~IMROEditor();

    QString Edit( const QString &file );

private slots:
    void defaultBut();
    void bankBut();
    void refidBut();
    void apBut();
    void lfBut();
    void loadBut();
    void saveBut();
    void okBut();
    void cancelBut();

private:
    void createR();
    void copyR2R0();
    void loadSettings();
    void saveSettings() const;
    void emptyTable();
    void R2Table();
    bool Table2R();
    int  bankMax( int ic );
    int  refidMax();
    bool gainOK( int val );
    void setAllBank( int val );
    void setAllRefid( int val );
    void setAllAPgain( int val );
    void setAllLFgain( int val );
    void adjustOption();
    void loadFile( const QString &file );
};

#endif  // IMROEDITOR_H


