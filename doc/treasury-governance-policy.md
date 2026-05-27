# Treasury Governance Policy

**Effective:** 2026-06-01 (Ratatoskr mainnet launch)
**Status:** Operator-committed public policy.

---

## Statement

The Ratatoskr treasury — the per-block 10% drip recipient defined in
`consensus.treasuryPaymentScript` — may be deployed as collateral for
masternodes operated by the project. Treasury-operated masternodes earn
their proportional share of masternode rewards from the consensus subsidy,
which flows back to the treasury balance.

**Every treasury-operated masternode casts `ABSTAIN` on every on-chain
governance proposal. No exceptions.**

This applies to:
- Budget / superblock proposals
- Spork-style emergency votes (when applicable)
- Any future governance mechanism that treasury masternodes participate in
- Proposals that specifically benefit the treasury (these especially)

---

## Why this policy exists

The 10% treasury drip generates approximately 7,200 RATR per day
(see whitepaper §4.4 for breakdown). At masternode collateral rates of
7,500 RATR (Regular) / 30,000 RATR (EvoNode), the treasury accumulates
enough RATR to fund several masternodes within the first month of
operation.

Without an abstention policy, a treasury that operates N EvoNodes would
control N × 4 governance votes — potentially a meaningful share of total
voting weight in the early life of the chain when total masternode count
is small. That concentration would make it impossible to credibly claim
the chain is governed by its broad masternode operator base.

By committing to abstain on all chain governance, the treasury becomes an
infrastructure participant only:

- **Yes**, treasury masternodes contribute to network security (block
  validation, ChainLocks signing, InstantSend signing).
- **Yes**, treasury masternodes earn their share of masternode rewards as
  consensus subsidy designed.
- **No**, treasury masternodes do not steer chain governance outcomes.

This separates the treasury's economic role (fund ecosystem development,
LP seeding, audits, listings) from any potential governance role. The
operator's individual voice in governance — when desired — is exercised
through personal masternodes registered to addresses the operator owns
directly, not via the treasury.

---

## How "abstain" is implemented

In Dash-lineage governance, each masternode casts one of:
- `YES` (or `FUNDING_YES`)
- `NO` (or `FUNDING_NO`)
- `ABSTAIN`

A masternode that does not cast a vote at all is functionally equivalent
to abstaining — its weight does not count toward either side. Treasury
masternodes will operate in one of two modes (operator's choice per
deployment cycle):

1. **Explicit ABSTAIN** — every proposal receives an explicit abstain
   vote from each treasury masternode. Visible on-chain as concrete
   participation without taking a side.
2. **No vote cast** — treasury masternodes simply never participate in
   governance polling. Functionally identical neutralization of weight.

Either implementation satisfies this policy. Both are externally
auditable: anyone can query the chain for governance votes and verify
that no `YES`/`NO` ever originated from a treasury-operated masternode
collateral address.

---

## Treasury masternode disclosure

To make verification practical, the operator commits to maintaining a
public list of all treasury masternode collateral addresses. This list
will be:

- Published in `doc/treasury-masternodes.md` of this repository (added
  when the first treasury masternode is registered)
- Updated within 24 hours of any treasury masternode registration or
  retirement
- Signed with the treasury private key to prove operator authorship
- Cross-referenced by collateral address (15-confirmation `protx
  register_fund` output)

This list lets anyone independently verify the abstention property:

```bash
ratatoskr-cli gobject getvotes <proposal_hash> | \
    jq -r '.[] | select(.MasternodeAddress | IN($treasury_addresses))'
# expected: only ABSTAIN entries, or empty
```

---

## Edge cases the policy covers

### Security / emergency proposals

If a chain-security or emergency upgrade proposal arises, treasury
masternodes still abstain. The operator's personal masternodes (if
registered separately to the operator's own collateral) may vote. This
keeps the operator's individual voice present without amplifying it
through treasury weight.

If the operator runs zero personal masternodes at the moment of an
emergency vote, the operator does not exploit the treasury masternodes
to participate. Either:
- Trust the broader masternode network to surface the right outcome; or
- Register a personal masternode rapidly using non-treasury RATR

### Self-interested proposals

Proposals that specifically benefit the treasury — for example, raising
the per-block drip percentage, allocating treasury funds to operator-
controlled projects, or modifying the treasury script address — are
EXACTLY the cases where treasury abstention matters most. The policy is
absolute on these.

### Hard fork / consensus upgrade votes

Same rule. Treasury masternodes abstain. The operator may publicly
advocate via blog post, Discord, GitHub PR review, or personal masternode
votes — but never via treasury weight.

### Future governance mechanisms

If Ratatoskr adopts new governance primitives in v1.1+ hard forks (e.g.,
on-chain quadratic voting, time-locked stake voting, etc.), the policy
extends to those primitives. Any future governance vector available to
treasury-controlled assets is subject to abstention.

---

## Treasury masternode count: what's allowed

Adopting abstention does not impose a numerical cap on treasury MN count.
The operator may run any number of treasury masternodes, scaling with
treasury balance, as long as:

1. All such masternodes abstain per this policy.
2. The treasury masternode list is kept current and auditable.
3. The collateral remains intact and recoverable (paper + metal backups
   per `treasury-keygen-runbook.md` pattern, applied to each treasury
   masternode collateral address).

Initial deployment plan (subject to operator review):

- **T-0 to T+17 days** (pre-MN-activation): treasury accumulates ~125k RATR
- **T+17 days** (block 25,000, MN payments activate): treasury registers
  initial 5-7 EvoNodes (150k-210k RATR collateral)
- **Year 1**: treasury MN count grows organically as MN income
  re-accumulates

Final count to be settled in a separate operator-internal decision,
captured in `doc/treasury-masternodes.md` once committed.

---

## Operator commitment

This policy is binding on the project operator. Violation — defined as
any treasury masternode casting `YES` or `NO` on any governance proposal
— constitutes a public breach of the launch commitment and would need to
be remediated transparently:

1. Disclosure of the violating vote within 24 hours
2. Justification of why it occurred (mistake, attack, compromise)
3. Corrective action (rollback if possible, abstention-by-design tooling
   if recurrence-prevention is needed)

The operator does not anticipate this policy ever being violated. It is
recorded here so the standard is unambiguous.

---

## How to verify this policy is being followed

1. Pull the current treasury masternode list from this doc / repo
2. Cross-reference each address against the active masternode set:
   ```
   ratatoskr-cli protx list registered
   ```
3. For any active proposal, check the votes from those addresses:
   ```
   ratatoskr-cli gobject getvotes <hash>
   ```
4. Filter for `treasury_addresses` and confirm only `ABSTAIN` entries
   (or no entries at all) appear.

This procedure is intended to be doable by any masternode operator,
analyst, or community member without operator cooperation.

---

## References

- Whitepaper §4.4 — treasury drip rationale (60/30/10 split)
- `doc/treasury-keygen-runbook.md` — air-gapped key ceremony (treasury
  address generation pattern)
- `doc/treasury-masternodes.md` — list of treasury MN collateral
  addresses (created when first treasury MN registers)
- `chainparams.cpp` — `consensus.treasuryPaymentScript` (the actual
  bytes of the treasury recipient script, baked into v1.0.0 final)

---

*Operator: NexusAether (`releases@enchantedforestdefi.com`).
Repository: `EnchantedForestDeFi/ratatoskr`.
Policy may be amended by future hard fork; the abstention principle is
not amendable without a vote that does NOT include treasury masternode
weight.*
