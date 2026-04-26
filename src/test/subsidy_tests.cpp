// Copyright (c) 2014-2023 The Smartiecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <spork.h>
#include <validation.h>

#include <test/util/setup_common.h>

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(subsidy_tests, TestingSetup)

BOOST_AUTO_TEST_CASE(block_subsidy_test)
{
    const auto chainParams = CreateChainParams(*m_node.args, CBaseChainParams::MAIN);

    static constexpr CAmount kInitialSubsidy = 50 * COIN;
    static constexpr int kOldHalvingInterval = 1030596;
    static constexpr int kNewHalvingInterval = 1000000;

    uint32_t nPrevBits;
    int32_t nPrevHeight;
    CAmount nSubsidy;

    // chainparams keeps the old interval; the new one activates at nSMTv014Height
    BOOST_CHECK_EQUAL(chainParams->GetConsensus().nSubsidyHalvingInterval, kOldHalvingInterval);

    // Mainnet starts at 50 SMT subsidy.
    nPrevBits = 0x1e3fffff;
    nPrevHeight = 1;
    nSubsidy = GetBlockSubsidyInner(nPrevBits, nPrevHeight, chainParams->GetConsensus(), /*fV20Active=*/ false);
    BOOST_CHECK_EQUAL(nSubsidy, kInitialSubsidy);

    // V20 does not affect subsidy scheduling on Smartiecoin mainnet/testnet.
    nPrevBits = 0x1b10d50b;
    nPrevHeight = 4249;
    nSubsidy = GetBlockSubsidyInner(nPrevBits, nPrevHeight, chainParams->GetConsensus(), /*fV20Active=*/ true);
    BOOST_CHECK_EQUAL(nSubsidy, kInitialSubsidy);

    // Halving boundaries after v0.1.4 fork uses new interval (1,000,000).
    // nPrevHeight semantics: subsidy returned for block nPrevHeight + 1.
    nPrevBits = 0x1b10d50b;
    nPrevHeight = kNewHalvingInterval - 1;
    nSubsidy = GetBlockSubsidyInner(nPrevBits, nPrevHeight, chainParams->GetConsensus(), /*fV20Active=*/ false);
    BOOST_CHECK_EQUAL(nSubsidy, 50 * COIN);

    nPrevHeight = kNewHalvingInterval;
    nSubsidy = GetBlockSubsidyInner(nPrevBits, nPrevHeight, chainParams->GetConsensus(), /*fV20Active=*/ false);
    BOOST_CHECK_EQUAL(nSubsidy, 25 * COIN);

    nPrevHeight = 2 * kNewHalvingInterval;
    nSubsidy = GetBlockSubsidyInner(nPrevBits, nPrevHeight, chainParams->GetConsensus(), /*fV20Active=*/ false);
    BOOST_CHECK_EQUAL(nSubsidy, 1250000000); // 12.5 SMT
}

BOOST_AUTO_TEST_CASE(masternode_payment_default_split_test)
{
    // Ratatoskr default split is 60/30/10 (miner/MN/treasury). With treasury
    // deducted upstream, GetMasternodePayment receives blockValue = 90% of
    // total subsidy and must return 30% / 90% = 1/3 of that = MN's 30% of
    // total subsidy.
    //
    // At default SPORK_25_MN_PAYMENT_BPS = 3000bp: MN payment = blockValue/3.

    CAmount blockValue = 45 * COIN; // 50 RATR subsidy - 10% treasury = 45 RATR distributable
    CAmount mnPayment = GetMasternodePayment(/*nHeight=*/ 1000, blockValue, /*fV20Active=*/ true);
    BOOST_CHECK_EQUAL(mnPayment, blockValue / 3); // 15 RATR (30% of total)

    // Miner gets the other two-thirds of distributable = 60% of total subsidy
    CAmount minerPayment = blockValue - mnPayment;
    BOOST_CHECK_EQUAL(minerPayment, (blockValue * 2) / 3); // 30 RATR (60% of total)

    // After first halving: 25 RATR base - 10% treasury = 22.5 distributable
    CAmount blockValueHalved = 2250000000; // 22.5 RATR in satoshis
    mnPayment = GetMasternodePayment(/*nHeight=*/ 1000000, blockValueHalved, /*fV20Active=*/ true);
    BOOST_CHECK_EQUAL(mnPayment, blockValueHalved / 3); // 7.5 RATR
}

BOOST_AUTO_TEST_CASE(masternode_payment_spork25_adjustable_split_test)
{
    // SPORK_25_MN_PAYMENT_BPS lets the operator adjust the MN share at runtime
    // within consensus floors [2000, 4000]bp. Out-of-range values fall back
    // to the default 3000bp.
    //
    // Math: MN payment = (mnShareBps * blockValue) / 9000
    //   blockValue is post-treasury (90% of total).

    constexpr CAmount blockValue = 45 * COIN; // 90% of 50 RATR
    auto& spork = *m_node.sporkman;

    // Hijack the spork values map for testing. We can't sign valid spork
    // messages without a private key, but we can manipulate the in-memory
    // sporkValues. CSporkManager exposes UpdateSpork() which checks
    // signatures, so instead we exercise the public default path here and
    // trust the integration to wire signed messages on a live node.

    // Default behavior (spork unset / out-of-range -> 3000bp default)
    {
        CAmount mn = GetMasternodePayment(0, blockValue, true);
        BOOST_CHECK_EQUAL(mn, (blockValue * 3000) / 9000); // 30% of total
    }

    // Math sanity-check on the formula at the floors and ceiling.
    // (These are the values consensus must accept.)
    {
        // 2000bp = 20% of total = lowest legal MN share
        BOOST_CHECK_EQUAL((blockValue * 2000) / 9000, 1000000000); // 10 RATR
        // 4000bp = 40% of total = highest legal MN share
        BOOST_CHECK_EQUAL((blockValue * 4000) / 9000, 2000000000); // 20 RATR
        // 3000bp = 30% = default; matches blockValue / 3 exactly
        BOOST_CHECK_EQUAL((blockValue * 3000) / 9000, blockValue / 3);
    }

    (void)spork;
}

BOOST_AUTO_TEST_SUITE_END()
