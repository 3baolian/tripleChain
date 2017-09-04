// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"
/*
#include <stdio.h>
#include "hash.h"
#include "scrypt.h"
bool static ScanHash(CBlock* pblock, uint256& thash)
{
    uint256 hashTarget = CBigNum().SetCompact(pblock->nBits).getuint256();
    while(true)
    {
        pblock->nNonce +=1;
        thash = scrypt_blockhash(CVOIDBEGIN(pblock->nVersion));
        if(thash <= hashTarget)
            break;
    }
    return true;
}
        uint256 hash;
        if(ScanHash(&genesis, hash))
        {
            printf("nonce: %lld\n", genesis.nNonce);
            printf("hash: %s\n", hash.ToString().data());
            printf("genesis.hash: %s\n", genesis.GetHash().ToString().data());
            printf("genesis.merkle: %s\n", genesis.hashMerkleRoot.ToString().data());
        }
*/
//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xeb;
        pchMessageStart[1] = 0xe3;
        pchMessageStart[2] = 0xee;
        pchMessageStart[3] = 0xe7;
        vAlertPubKey = ParseHex("04a8dc25b4dde15260549dd3991139ce283ded007c6cbd83649387b09c9ccd17f28b0d6ae0603f27773f40ea13f5158568545575cfd7740618f7e4a822fdef49bd");
        nDefaultPort = 20271;
        nRPCPort = 20272;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 12);

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        const char* pszTimestamp = "";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
		CTransaction txNew(1, 1501862400, vin, vout, 0); // 2017-08-08 00:00:00 UTC+8
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
		genesis.nTime = 1501862400;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 400;
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xa33748e4c7736108376f4460e91a99bd236dcb8fb39e9e948136b6b6e113a6d5"));
        assert(genesis.hashMerkleRoot == uint256("0x244a1760fb71ab59c173f3984f5f4c981c5f51d298af642281f4921c88a8bb6f"));
		//printf( "CMain===hashGenesisBlock: %s\n", hashGenesisBlock.ToString().c_str());
		//printf( "CMain===hashMerkleRoot: %s\n", genesis.hashMerkleRoot.ToString().c_str());

        vSeeds.push_back(CDNSSeedData("47.92.96.98", "47.92.96.98"));
        vSeeds.push_back(CDNSSeedData("106.14.203.191","106.14.203.191"));
        vSeeds.push_back(CDNSSeedData("39.108.175.188","39.108.175.188"));
        
        base58Prefixes[PUBKEY_ADDRESS] = list_of(65);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(127);
        base58Prefixes[SECRET_KEY] =     list_of(253);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nLastPOWBlock = 3000;
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xdd;
        pchMessageStart[1] = 0xe4;
        pchMessageStart[2] = 0xe8;
        pchMessageStart[3] = 0xd9;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 10);
        vAlertPubKey = ParseHex("04b99534f71e0f874e3ed9c804873605be75efb5ede2937467e86062ed471cecf0f59d8a77270e30a7e4856e8205b17d8b97ca660875ab3949f19c171d27ca6ede");
        nDefaultPort = 30271;
        nRPCPort =30272;
        strDataDir = "testnet";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 1104;
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xd21187816800c8d9b01316cf25bc32da4de1a912f52b5326fcb137456b7fa122"));
		//printf( "TestNet===hashGenesisBlock: %s\n", hashGenesisBlock.ToString().c_str());

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = list_of(65);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(127);
        base58Prefixes[SECRET_KEY]     = list_of(253);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nLastPOWBlock = 0xffffffff;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xd6;
        pchMessageStart[1] = 0xda;
        pchMessageStart[2] = 0xb2;
        pchMessageStart[3] = 0xb3;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 1;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 40271;
        nRPCPort = 40272;
        strDataDir = "regtest";
        assert(hashGenesisBlock == uint256("0xec78f7b887818e72d411325d0095041377a9e0de736ea8ae97f79a3d7fd53c7c"));
		//printf("CRegTest===hashGenesisBlock: %s\n", hashGenesisBlock.ToString().c_str());

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
