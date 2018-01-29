// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"
#include "arith_uint256.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Today begins a humble project with great hopes - 28/01/2018";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 0; // Note: actual number of blocks per calendar year with DGW v3 is ~334500
        consensus.nMasternodePaymentsStartBlock = 101; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 10512000; // 30 y disable
        consensus.nMasternodePaymentsIncreasePeriod = 576*30; // 17280 - not used
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 328008; // Not use
        consensus.nBudgetPaymentsCycleBlocks = 27692; // ~(60*24*30)/1.56, actual number of blocks per month is 334500 / 12 = 27875
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 60*60*24;
        consensus.nSuperblockStartBlock = 124805; // The block at which 12.1 goes live
        consensus.nSuperblockCycle = 27692; // ~(60*24*30)/1.56, actual number of blocks per month is 334500 / 12 = 27875
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 10;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 227931;
        consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8");
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 12 * 60 * 60; // Digifel: 12 h
        consensus.nPowTargetSpacing = 1 * 60; // Digifel: 60 sec
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1486252800; // Feb 5th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517788800; // Feb 5th, 2018

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xae;
        pchMessageStart[1] = 0x0e;
        pchMessageStart[2] = 0x6d;
        pchMessageStart[3] = 0xed;
        vAlertPubKey = ParseHex("0425ef115e98deb6637b6317397f02828ce2adeec9aa89b379d35c2b8fafc0d737d8522a1661b046d3a2899284766c03da33a4626f09f54dac56d7cd52e2eb97da");
        nDefaultPort = 10070;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1517011200, 340632, 0x1e0ffff0, 1, 10 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        // std::cout << consensus.hashGenesisBlock.ToString() << '\n';
        // if ( genesis.GetHash() != uint256S("0x000007bd4ee14c2f9e236d0cfe79997a5921a6bd95247abf7be71a1b5b0b8875") )
        // {
        //     arith_uint256 targetHash;
        //     targetHash.SetCompact(genesis.nBits);
        //
        //     while (UintToArith256(genesis.GetHash()) > targetHash)
        //     {
        //         ++genesis.nNonce;
        //         if (genesis.nNonce == 0)
        //         {
        //             ++genesis.nTime;
        //         }
        //     }
        //     std::cout << genesis.ToString() << '\n';
        // }


        assert(consensus.hashGenesisBlock == uint256S("0x000007bd4ee14c2f9e236d0cfe79997a5921a6bd95247abf7be71a1b5b0b8875"));
        assert(genesis.hashMerkleRoot == uint256S("0x8be55d9fe580ad3d3a909bfeb3a5b111be06a4ae492a0f7359511feac356dece"));

        vSeeds.push_back(CDNSSeedData("digifel0", "dnsseed.digifel.net"));
        vSeeds.push_back(CDNSSeedData("digifel1", "wallet.payfel.io"));

        // Digifel addresses start with Q-Z, a-k, m-o
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,1);
        // Digifel script addresses start with Q-Z, a-k, m-o
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,1);
        // Digifel private keys start with Q-Z, a-k, m-o
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,1);
        // Digifel BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Digifel BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // Digifel BIP44 coin type is '5'
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x05).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "04903fae9fbebf40353d66d755a198a2476b80a727cf20e6fa71647969758c76af1635ffe9fe923c846afd01115762016485b8bd8c482cba565c521d2b731fa6df";
        strMasternodePaymentsPubKey = "04d5f383bcf39ac04a621e9ce5fd80698f9b6d9fc09b4b78f68b66fa4668298283e249be896a619f351f4cf98fc07fcbe0eefe864d8cc149d29dd689af3ff8802f";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (     0, uint256S("0x000007bd4ee14c2f9e236d0cfe79997a5921a6bd95247abf7be71a1b5b0b8875"))
            (   500, uint256S("0x0000000000004c5f531722082ebae7ed05227cc5378ddcfa9947e19021d66777"))
            (  2500, uint256S("0x000000000000096d912b21a12f963b58e72c48268e1240d9d74ebfaa53b5cdf4")),
            1517171906, // * UNIX timestamp of last checkpoint block
            2890,       // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            1900        // * estimated number of transactions per day after checkpoint
        };
    }
};

static CMainParams mainParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    return mainParams;
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
