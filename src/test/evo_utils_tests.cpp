// Copyright (c) 2022-2025 The Ratatoskr Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test/util/setup_common.h>

#include <chain.h>
#include <chainparams.h>
#include <llmq/options.h>
#include <sync.h>
#include <validation.h>

#include <limits>

#include <boost/test/unit_test.hpp>

using node::NodeContext;

/* TODO: rename this file and test to llmq_options_test */
BOOST_AUTO_TEST_SUITE(evo_utils_tests)

void Test(NodeContext& node)
{
    using namespace llmq;
    auto tip = node.chainman->ActiveTip();
    const auto& consensus_params = Params().GetConsensus();
    BOOST_CHECK_EQUAL(node.chainman->IsQuorumTypeEnabled(consensus_params.llmqTypeDIP0024InstantSend, tip,
                                                         /*optDIP0024IsActive=*/false, /*optHaveDIP0024Quorums=*/false),
                      false);
    BOOST_CHECK_EQUAL(node.chainman->IsQuorumTypeEnabled(consensus_params.llmqTypeDIP0024InstantSend, tip,
                                                         /*optDIP0024IsActive=*/true, /*optHaveDIP0024Quorums=*/false),
                      true);
    BOOST_CHECK_EQUAL(node.chainman->IsQuorumTypeEnabled(consensus_params.llmqTypeDIP0024InstantSend, tip,
                                                         /*optDIP0024IsActive=*/true, /*optHaveDIP0024Quorums=*/true),
                      true);
    BOOST_CHECK_EQUAL(node.chainman->IsQuorumTypeEnabled(consensus_params.llmqTypeChainLocks, tip,
                                                         /*optDIP0024IsActive=*/false, /*optHaveDIP0024Quorums=*/false),
                      true);
    BOOST_CHECK_EQUAL(node.chainman->IsQuorumTypeEnabled(consensus_params.llmqTypeChainLocks, tip,
                                                         /*optDIP0024IsActive=*/true, /*optHaveDIP0024Quorums=*/false),
                      true);
    BOOST_CHECK_EQUAL(node.chainman->IsQuorumTypeEnabled(consensus_params.llmqTypeChainLocks, tip,
                                                         /*optDIP0024IsActive=*/true, /*optHaveDIP0024Quorums=*/true),
                      true);
    BOOST_CHECK_EQUAL(node.chainman->IsQuorumTypeEnabled(consensus_params.llmqTypePlatform, tip,
                                                         /*optDIP0024IsActive=*/false, /*optHaveDIP0024Quorums=*/false),
                      Params().IsTestChain());
    BOOST_CHECK_EQUAL(node.chainman->IsQuorumTypeEnabled(consensus_params.llmqTypePlatform, tip,
                                                         /*optDIP0024IsActive=*/true, /*optHaveDIP0024Quorums=*/false),
                      Params().IsTestChain());
    BOOST_CHECK_EQUAL(node.chainman->IsQuorumTypeEnabled(consensus_params.llmqTypePlatform, tip,
                                                         /*optDIP0024IsActive=*/true, /*optHaveDIP0024Quorums=*/true),
                      Params().IsTestChain());
    BOOST_CHECK_EQUAL(node.chainman->IsQuorumTypeEnabled(consensus_params.llmqTypeMnhf, tip,
                                                         /*optDIP0024IsActive=*/false, /*optHaveDIP0024Quorums=*/false),
                      true);
    BOOST_CHECK_EQUAL(node.chainman->IsQuorumTypeEnabled(consensus_params.llmqTypeMnhf, tip,
                                                         /*optDIP0024IsActive=*/true, /*optHaveDIP0024Quorums=*/false),
                      true);
    BOOST_CHECK_EQUAL(node.chainman->IsQuorumTypeEnabled(consensus_params.llmqTypeMnhf, tip,
                                                         /*optDIP0024IsActive=*/true, /*optHaveDIP0024Quorums=*/true),
                      true);
}

BOOST_FIXTURE_TEST_CASE(utils_IsQuorumTypeEnabled_tests_regtest, RegTestingSetup)
{
    Test(m_node);
}

BOOST_FIXTURE_TEST_CASE(utils_IsQuorumTypeEnabled_tests_mainnet, TestingSetup)
{
    Test(m_node);
}

// v1.0.3: at/after the fork height the four oversized quorum types — LLMQ_60_75, LLMQ_100_67,
// LLMQ_400_60, LLMQ_400_85 — stop forming on a network not yet large enough to support them,
// while the small quorums keep carrying InstantSend / ChainLocks / Platform. Runs on mainnet
// params so it exercises the REAL activation height (nSMTOversizedQuorumDisableHeight = 36000).
BOOST_FIXTURE_TEST_CASE(oversized_quorum_disable_at_fork, TestingSetup)
{
    using LLMQType = Consensus::LLMQType;
    LOCK(cs_main);
    auto& chainman = *m_node.chainman;
    const auto& cp = Params().GetConsensus();
    const int disableH = cp.nSMTOversizedQuorumDisableHeight;
    BOOST_REQUIRE(disableH > 0 && disableH != std::numeric_limits<int>::max());
    BOOST_REQUIRE(disableH > cp.nSMTSmallQuorumsHeight);  // fork is well past the small-quorum takeover

    // Synthetic block indices straddling the fork height. The guard and the flag-provided
    // returns we assert here read only pindexPrev->nHeight, so a height-only CBlockIndex is
    // sufficient (no chain walk, no real tip required).
    CBlockIndex before; before.nHeight = disableH - 1;
    CBlockIndex after;  after.nHeight  = disableH;

    const LLMQType oversized[] = {LLMQType::LLMQ_60_75, LLMQType::LLMQ_100_67,
                                  LLMQType::LLMQ_400_60, LLMQType::LLMQ_400_85};

    // AT/AFTER the fork: ALL FOUR oversized types are disabled. DIP0024 flags are supplied so the
    // call never walks the chain — the guard fires purely on height.
    for (const auto t : oversized) {
        BOOST_CHECK_EQUAL(
            chainman.IsQuorumTypeEnabled(t, &after, /*optDIP0024IsActive=*/true, /*optHaveDIP0024Quorums=*/true),
            false);
    }

    // BELOW the fork: the guard is NOT firing — types unconditionally enabled (400_60 / 400_85)
    // and the DIP0024 InstantSend type (60_75, enabled when DIP0024 active) are still enabled.
    // Proves the disable is height-gated, not a blanket off-switch.
    BOOST_CHECK_EQUAL(chainman.IsQuorumTypeEnabled(LLMQType::LLMQ_400_60, &before, true, true), true);
    BOOST_CHECK_EQUAL(chainman.IsQuorumTypeEnabled(LLMQType::LLMQ_400_85, &before, true, true), true);
    BOOST_CHECK_EQUAL(chainman.IsQuorumTypeEnabled(LLMQType::LLMQ_60_75,  &before, true, true), true);

    // SAFETY: disabling the oversized quorums cannot strand InstantSend / ChainLocks / Platform,
    // because past the fork those services are carried by the SMALL quorums (which the guard does
    // not touch) — not by the oversized 60_75 / 100_67 we disable.
    BOOST_REQUIRE(cp.llmqTypeSmallInstantSend != LLMQType::LLMQ_NONE);
    BOOST_CHECK(cp.GetInstantSendType(disableH) == cp.llmqTypeSmallInstantSend);
    BOOST_CHECK(cp.GetChainLocksType(disableH)  == cp.llmqTypeSmallChainLocks);
    BOOST_CHECK(cp.GetInstantSendType(disableH) != cp.llmqTypeDIP0024InstantSend);  // not the disabled 60_75
    // and 60_75 was only ever the InstantSend type BELOW the small-quorum height:
    BOOST_CHECK(cp.GetInstantSendType(cp.nSMTSmallQuorumsHeight - 1) == cp.llmqTypeDIP0024InstantSend);
}

BOOST_AUTO_TEST_SUITE_END()
