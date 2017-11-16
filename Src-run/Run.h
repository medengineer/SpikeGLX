#ifndef RUN_H
#define RUN_H

#include "KVParams.h"

#include <QObject>
#include <QMutex>

namespace DAQ {
struct Params;
}

class MainApp;
class GraphsWindow;
class GraphFetcher;
struct GFStream;
class IMReader;
class NIReader;
class Gate;
class Trigger;
class AIQ;

class QFileInfo;

/* ---------------------------------------------------------------- */
/* Types ---------------------------------------------------------- */
/* ---------------------------------------------------------------- */

class Run : public QObject
{
    Q_OBJECT

private:
    MainApp         *app;
    AIQ             *imQ,           // guarded by runMtx
                    *niQ;           // guarded by runMtx
    GraphsWindow    *graphsWindow;  // guarded by runMtx
    GraphFetcher    *graphFetcher;  // guarded by runMtx
    IMReader        *imReader;      // guarded by runMtx
    NIReader        *niReader;      // guarded by runMtx
    Gate            *gate;          // guarded by runMtx
    Trigger         *trg;           // guarded by runMtx
    mutable QMutex  runMtx;
    bool            running,        // guarded by runMtx
                    dumx[3];

public:
    Run( MainApp *app );

// Owned GraphsWindow ops
    bool grfIsUsrOrderIm();
    bool grfIsUsrOrderNi();
    void grfRemoteSetsRunName( const QString &fn );
    void grfSetStreams( QVector<GFStream> &gfs );
    void grfHardPause( bool pause );
    void grfSetFocus();
    void grfShowHide();
    void grfUpdateRHSFlags();
    void grfUpdateWindowTitles();

// Owned AIStream ops
    quint64 getImScanCount() const;
    quint64 getNiScanCount() const;
    const AIQ* getImQ() const;
    const AIQ* getNiQ() const;

// Run control
    bool isRunning() const
        {QMutexLocker ml( &runMtx ); return running;}

    bool startRun( QString &errTitle, QString &errMsg );
    void stopRun();
    bool askThenStopRun();
    bool imecPause( bool pause, bool changed );

public slots:
// GraphFetcher ops
    void grfSoftPause( bool pause );

// Owned Datafile ops
    bool dfIsSaving() const;
    bool dfIsInUse( const QFileInfo &fi ) const;
    void dfSetRecordingEnabled( bool enabled, bool remote = false );
    void dfResetGTCounters();
    void dfForceGTCounters( int g, int t );
    QString dfGetCurNiName() const;
    quint64 dfGetImFileStart() const;
    quint64 dfGetNiFileStart() const;

// Owned gate and trigger ops
    void rgtSetGate( bool hi );
    void rgtSetTrig( bool hi );
    void rgtSetMetaData( const KeyValMap &kvm );

// Audio ops
    void aoStart();
    void aoStop();

private slots:
    void gettingSamples();
    void workerStopsRun();

private:
    void aoStartDev();
    bool aoStopDev();
    void createGraphsWindow( const DAQ::Params &p );
    int streamSpanMax( const DAQ::Params &p );
};

#endif  // RUN_H


