// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 500;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        ( 0,     uint256("0xa33748e4c7736108376f4460e91a99bd236dcb8fb39e9e948136b6b6e113a6d5") ) // genesis block
        ( 1000,  uint256("0x623660c8f8538c53f88eff59f892f4540388e54fd1eedcaad4f40fb8e2e103cf") ) // the 1000th pow block
        ( 2000,  uint256("0xc8c91b5b48310d439d0e120a50c9923cdd886c2fb3bda3006ad75c1746639d99") ) // the 2000th pow block
        ( 3000,  uint256("0xd4a3a16fad81d38e1bcdf3af86dd42c36b09ee45e1096c2513021a0a70d143ba") ) // the last(3000th) pow block
        ( 3001,  uint256("0x3d5af1ba0422e7f112b5c02de9eb9feee89ef9ff60e601e247712f321a4b50cc") ) // the first pos block, hardfork
   ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();

        if (nHeight <= pindexSync->nHeight)
            return false;
        return true;
    }

    double GetVerificationProcess()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        int nHeight = 0;
        BOOST_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
            nHeight = i.first;
        return nBestHeight > nHeight ? 1 : (double)nBestHeight / nHeight;
    }
}
