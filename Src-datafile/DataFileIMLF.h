#ifndef DATAFILEIMLF_H
#define DATAFILEIMLF_H

#include "DataFile.h"

/* ---------------------------------------------------------------- */
/* Types ---------------------------------------------------------- */
/* ---------------------------------------------------------------- */

class DataFileIMLF : public DataFile
{
private:
    // Input mode
    VRange  imRange;
    int     imCumTypCnt[CimCfg::imNTypes];
    IMROTbl roTbl;

public:
    virtual QString typeFromObj() const     {return "imec";}
    virtual QString subtypeFromObj() const  {return "imec.lf";}

    // ---------
    // Meta data
    // ---------

    const VRange &imRng() const {return imRange;}
    int origID2Type( int ic ) const;
    double origID2Gain( int ic ) const;
    ChanMapIM chanMap() const;

protected:
    virtual void subclassParseMetaData();
    virtual void subclassStoreMetaData( const DAQ::Params &p );
    virtual int subclassGetAcqChanCount( const DAQ::Params &p );
    virtual int subclassGetSavChanCount( const DAQ::Params &p );

    virtual void subclassSetSNSChanCounts(
        const DAQ::Params   *p,
        const DataFile      *dfSrc );

private:
    void parseChanCounts();
};

#endif  // DATAFILEIMLF_H

