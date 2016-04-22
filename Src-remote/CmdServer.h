#ifndef COMMANDSERVER_H
#define COMMANDSERVER_H

#include "SockUtil.h"

#include <QTcpServer>
#include <QStringList>

class Par2Worker;

class QTcpSocket;

/* ---------------------------------------------------------------- */
/* Types ---------------------------------------------------------- */
/* ---------------------------------------------------------------- */

#define CMD_DEF_PORT    4142
#define CMD_TOUT_MS     10000


class CmdServer : protected QTcpServer
{
private:
    int timeout_msecs;

public:
    CmdServer( QObject *parent )
    : QTcpServer(parent), timeout_msecs(CMD_TOUT_MS)    {}
    virtual ~CmdServer();

    bool beginListening(
        const QString   &iface = "0.0.0.0",
        ushort          port = CMD_DEF_PORT,
        uint            timeout_ms = CMD_TOUT_MS );

    static void deleteAllActiveConnections();

protected:
    void incomingConnection( int sockFd ); // from QTcpServer
};


// CmdServer creates a new CmdWorker instance
// to handle each remote connection. If CmdWorker
// remains innactive for timeout_ms (~10s) it
// deletes itself (see CmdWorker::run()).
//
class CmdWorker : public QObject
{
    Q_OBJECT

private:
    QString     errMsg;
    Par2Worker  *par2;
    QTcpSocket  *sock;
    SockUtil    SU;
    int         sockFd,
                timeout;

public:
    CmdWorker( int sockFd, int timeout )
    :   QObject(0), par2(0),
        sock(0), sockFd(sockFd),
        timeout(timeout)  {}
    virtual ~CmdWorker();

signals:
    void finished();

public slots:
    void run();
    void sha1Progress( int pct );
    void sha1Result( int res );
    void par2Report( const QString &s );
    void par2Error( const QString &s );

private:
    void sendOK();
    void sendError( const QString &errMsg );
    void getAcqChanCounts( QString &resp );
    void setRunDir( const QString &path );
    void enumRunDir();
    void setParams();
    void setAOParams();
    void setAOEnable( const QStringList &toks );
    void setRecordingEnabled( const QStringList &toks );
    void setRunName(const QStringList &toks );
    void setDigOut( const QStringList &toks );
    void fetchIm( const QStringList &toks );
    void fetchNi( const QStringList &toks );
    void consoleShow( bool show );
    void verifySha1( QString file );
    void par2Start( QStringList toks );
    bool doQuery( const QString &cmd );
    bool doCommand(
        const QString       &line,
        const QString       &cmd,
        const QStringList   &toks );
    bool processLine( const QString &line );
};

#endif  // COMMANDSERVER_H


