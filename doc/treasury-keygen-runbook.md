# Treasury Address Generation Runbook (v1.0.0-alpha)

**Goal:** Generate the Ratatoskr mainnet treasury address using an air-gapped
machine, in a way that is (a) reproducible by outside verifiers, (b) backed up
physically, and (c) never exposes the private key to any networked system.

**Output:**
1. A P2PKH mainnet address starting with `R...`
2. A `CScript` opcode-push form ready to paste into `src/chainparams.cpp`
3. Two offline backups of the WIF-encoded private key (paper + metal)
4. A `signmessage` signature proving operator control, produced air-gapped

**Time budget:** **~90 minutes** end-to-end (unhurried, first-time). Prior
runbook estimates of 45 min were optimistic. Do not rush.

**When to run this:**
- **NEVER before** a successful testnet dress rehearsal (§0.5)
- **No earlier than** 2 weeks before mainnet launch (2026-06-01)
- **No later than** 1 week before mainnet (need buffer for tarball rebuild,
  hash publication, community verification)

---

## ⚠️ CRITICAL WARNINGS — READ BEFORE STARTING

### No recovery path at v1.0

The v1.0 treasury is **single-sig with zero recovery**. If both paper AND
metal backups are lost or destroyed, the treasury is permanently frozen at
the next hard fork migration point. The v1.1 multisig migration can move the
treasury forward but cannot resurrect lost keys. **Make at least two backups
stored in two physically separated locations.**

### Placeholder is WORSE than nothing

The current placeholder in `src/chainparams.cpp` is:
```cpp
consensus.treasuryPaymentScript = CScript() << OP_RETURN << ParseHex("52415452");
```

This is an unspendable `OP_RETURN "RATR"` script. Consensus code still treats
the placeholder as non-empty and WILL drip 10% of every block subsidy into
it forever. **If mainnet launches with the placeholder unchanged, ~5% of max
supply (5,152,980 RATR in year 1) is literally set on fire.**

A pre-tag CI guard (§5) MUST verify the placeholder was replaced before the
`v1.0.0` tag can be built.

### Consensus is byte-exact

`masternode/payments.cpp:301`:
```cpp
if (txout.scriptPubKey == m_consensus_params.treasuryPaymentScript && ...)
```

The CScript baked into `chainparams.cpp` must be **byte-identical** to the
scriptPubKey the wallet produces. One byte off = every block invalid = chain
doesn't start. This is why §5 uses the `<< OP_DUP << OP_HASH160 << ... <<
OP_EQUALVERIFY << OP_CHECKSIG` form: it guarantees byte-for-byte match with
the canonical P2PKH layout `76 a9 14 <20-byte-hash> 88 ac`.

---

## 0. What you need (hard prerequisites — no substitutions)

### Hardware

- [ ] An air-gap machine — laptop you will keep offline permanently, or a
      fresh Raspberry Pi / old ThinkPad. **Do NOT use your daily-driver
      Windows box.**
- [ ] USB stick #1 — **factory-sealed**, never previously plugged into any
      machine. For the RATR binary transfer only.
- [ ] USB stick #2 — **factory-sealed**, different brand/batch from #1.
      For transporting only public data (address, pubkey, signature) back
      to online machine.
- [ ] Paper + pen — two sheets, black/blue ink (not gel, which fades)
- [ ] **Metal backup plate** — REQUIRED (was optional in older runbook
      revisions; upgraded to required for mainnet treasury). CryptoSteel,
      Blockplate, or stamped stainless steel bolts. Budget: $20–80.
- [ ] Camera-off phone, webcam-covered laptop. No ambient recording
      devices within sight of the screen.
- [ ] Laptop plugged into mains power — battery > 50% as backup, but mains
      primary. A dying laptop mid-ceremony loses the WIF with the session.

### Software

- [ ] Ubuntu live USB (22.04 or 24.04) — sha256-verified against
      ubuntu.com. **Do not use a live USB that has been plugged into any
      untrusted machine since it was created.**
- [ ] Ratatoskr Linux tarball (`ratatoskr-v1.0.0-alpha-linux-x86_64.tar.gz`)
      plus its sha256, both copied to USB stick #1.

### Environment

- [ ] Private room, blinds drawn, door closable
- [ ] No networked devices in the room (phone, Alexa, smart TV — all out)
- [ ] Ethernet cable physically unplugged from the laptop
- [ ] WiFi physically off (hardware kill-switch if available, BIOS-disabled
      if no switch)

### Gating rule

**If ANY item above is not physically in front of you, DO NOT start the
ceremony.** Reschedule. This list is a checkpoint, not a suggestion.

---

## 0.5. Testnet dress rehearsal (MANDATORY)

Do the entire ceremony on testnet BEFORE mainnet. Testnet is a separate
2–3 hour session on a different day. Do not proceed to mainnet ceremony
without a successful dress rehearsal.

### Rehearsal sequence

1. **Generate testnet treasury key** using this same runbook, but:
   - Launch daemon with `-testnet` flag
   - Address will start with `y` (testnet PUBKEY_ADDRESS prefix = 140),
     NOT `R`
   - Sanity check: `[[ "$ADDR" =~ ^y ]]`

2. **Bake into `CTestNetParams` block of `chainparams.cpp`** (line ~401 of
   current file — the one immediately after `strNetworkID =
   CBaseChainParams::TESTNET`).

3. **Rebuild Linux tarball** from that commit.

4. **Run local testnet:**
   ```bash
   ./ratatoskrd -testnet -datadir=/tmp/testnet-treasury-test \
                -connect=0 -dnsseed=0 -listen=0 -maxconnections=0 \
                -gen=1 -genproclimit=1
   ```

5. **Mine ≥25 blocks**, then verify:
   ```bash
   ./ratatoskr-cli -testnet getblock $(./ratatoskr-cli -testnet getblockhash 1) 2
   # Look for a vout entry where scriptPubKey.addresses includes your testnet y... address
   # and value == 5 RATR (10% of 50 subsidy)
   ```

6. **Verify every block 1..25 contains the treasury output** at correct
   amount. Any missing = consensus bug. Do not proceed.

7. **Sign and verify testnet message:**
   ```bash
   ./ratatoskr-cli -testnet signmessage "y..." "Testnet rehearsal $(date -u +%Y-%m-%d)"
   ./ratatoskr-cli -testnet verifymessage "y..." "<signature>" "Testnet rehearsal $(date -u +%Y-%m-%d)"
   # must return true
   ```

8. **Revert the testnet chainparams change** before mainnet ceremony (do
   not ship the rehearsal testnet script — generate a fresh testnet one
   after mainnet launch if testnet is to be maintained).

Only after all 8 rehearsal steps pass: proceed to mainnet ceremony on a
separate day.

---

## 1. Prep the air-gapped machine

1. Boot the target laptop from Ubuntu live USB. Pick **"Try Ubuntu"**
   (NOT install — we want no persistence).

2. **Confirm network is fully off:**
   ```bash
   ip link show
   # every interface should be DOWN or state DOWN
   nmcli radio all
   # wifi / wwan / rfkill all = off
   ```
   If anything shows UP, open Settings → Network and disable everything.
   For paranoia: pull the ethernet cable and use the hardware WiFi kill
   switch. **Do not proceed until every interface is down.**

3. **Verify `/tmp` is tmpfs (RAM-backed):**
   ```bash
   mount | grep ' /tmp '
   # Must show "tmpfs on /tmp type tmpfs ..."
   ```
   If `/tmp` is mounted on disk, the wallet.dat could persist after
   shutdown. **Stop and investigate** — do not proceed until /tmp is
   confirmed tmpfs.

4. **Entropy wait — 5 minutes.** Freshly-booted live systems have thin
   `/dev/urandom` entropy. Wait 5+ minutes while moving the mouse
   randomly and typing on the keyboard (not the WIF — anything random).
   Then verify:
   ```bash
   cat /proc/sys/kernel/random/entropy_avail
   # Should be > 256 (ideally > 1024)
   ```

5. **Transfer** `ratatoskr-v1.0.0-alpha-linux-x86_64.tar.gz` from USB
   stick #1 to `/tmp`:
   ```bash
   cp /media/ubuntu/<USB1>/ratatoskr-v1.0.0-alpha-linux-x86_64.tar.gz /tmp/
   ```

6. **Verify sha256:**
   ```bash
   sha256sum /tmp/ratatoskr-v1.0.0-alpha-linux-x86_64.tar.gz
   ```
   Compare to the value on the GitHub release page (copied to USB #1 as
   a separate `.sha256` file — read from there, not from memory). **Stop
   and restart from step 0 if it doesn't match.**

---

## 2. Generate the private key

1. **Extract and chdir:**
   ```bash
   cd /tmp
   tar xzf ratatoskr-v1.0.0-alpha-linux-x86_64.tar.gz
   cd ratatoskr-v1.0.0-alpha-linux-x86_64/bin
   ```

2. **Start the daemon with full network-off flags:**
   ```bash
   mkdir -p /tmp/treasury-keygen
   ./ratatoskrd -datadir=/tmp/treasury-keygen \
                -daemon \
                -server=1 \
                -listen=0 \
                -connect=0 \
                -dnsseed=0 \
                -maxconnections=0 \
                -rpcport=18181 \
                -rpcuser=keygen \
                -rpcpassword=keygen-only-local-ignore
   ```
   (Note: `-noconnect` as in older runbook revisions is NOT a real flag.
   The canonical set is `-listen=0 -connect=0 -dnsseed=0 -maxconnections=0`.)

3. **Poll until the daemon is responsive** (don't use fixed `sleep`):
   ```bash
   CLI="./ratatoskr-cli -datadir=/tmp/treasury-keygen -rpcport=18181 -rpcuser=keygen -rpcpassword=keygen-only-local-ignore"
   for i in $(seq 1 30); do
     $CLI getblockchaininfo >/dev/null 2>&1 && break
     sleep 1
   done
   $CLI getblockchaininfo >/dev/null || { echo "daemon not responding"; exit 1; }
   ```

4. **Create LEGACY wallet** (default in modern Dash/Bitcoin is descriptor
   wallets, which do NOT support `dumpprivkey`):
   ```bash
   $CLI -named createwallet wallet_name=treasury descriptors=false
   ```
   Verify legacy mode:
   ```bash
   $CLI -rpcwallet=treasury getwalletinfo | grep -E '(descriptors|format)'
   # Should show "descriptors": false (or omit it), and a legacy format
   ```

5. **Generate the address:**
   ```bash
   WCLI="$CLI -rpcwallet=treasury"
   ADDR=$($WCLI getnewaddress "ratatoskr-treasury-v1")
   echo "Address: $ADDR"
   ```

6. **Sanity check the address prefix** — mainnet treasuries MUST start
   with `R`:
   ```bash
   [[ "$ADDR" =~ ^R ]] && echo "OK: mainnet prefix" || { echo "WRONG PREFIX — abort"; exit 1; }
   ```
   (For testnet dress rehearsal, replace `^R` with `^y`.)

7. **Extract WIF (private), pubkey, and scriptPubKey using robust JSON
   parsing** (NOT grep — JSON field order is not stable):
   ```bash
   WIF=$($WCLI dumpprivkey "$ADDR")

   INFO_JSON=$($WCLI getaddressinfo "$ADDR")
   PUBKEY=$(echo "$INFO_JSON" | python3 -c "import json,sys; print(json.load(sys.stdin)['pubkey'])")
   SCRIPT_PUB_KEY=$(echo "$INFO_JSON" | python3 -c "import json,sys; print(json.load(sys.stdin)['scriptPubKey'])")
   ```

8. **Verify scriptPubKey matches exact P2PKH pattern:**
   ```bash
   [[ "$SCRIPT_PUB_KEY" =~ ^76a914[0-9a-f]{40}88ac$ ]] && echo "OK: P2PKH" || { echo "NOT P2PKH — abort"; exit 1; }
   PUBKEY_HASH=${SCRIPT_PUB_KEY:6:40}  # strip 76a914 prefix and 88ac suffix
   echo "pubkey hash (20 bytes): $PUBKEY_HASH"
   ```

9. **Display all outputs for paper backup:**
   ```bash
   echo "============================================"
   echo "RATR MAINNET TREASURY — v1.0"
   echo "Generated: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
   echo "Address:       $ADDR"
   echo "Pubkey:        $PUBKEY"
   echo "scriptPubKey:  $SCRIPT_PUB_KEY"
   echo "Pubkey hash:   $PUBKEY_HASH"
   echo "--------------------------------------------"
   echo "WIF (PRIVATE — PAPER ONLY — NEVER DIGITAL):"
   echo "$WIF"
   echo "============================================"
   ```

10. **WRITE THE WIF ON PAPER.** By hand. Twice. On two separate sheets.
    Read both copies back to verify they match the screen character-for-
    character. Include the date and a sequential number (so you know
    which is copy 1 and copy 2). Treasury-grade handwriting — if a digit
    is ambiguous, rewrite the whole line.

11. **Stamp the metal backup.** Same WIF, same verification procedure.
    Double-check by comparing stamped chars against paper.

12. **Sign a message PROVING CONTROL** — this MUST happen air-gapped now,
    not later on an online machine:
    ```bash
    MESSAGE="RATR treasury v1.0 confirmed by project operator. Launch block 1 is 2026-06-01 00:00 UTC."
    SIGNATURE=$($WCLI signmessage "$ADDR" "$MESSAGE")
    echo "Signature: $SIGNATURE"
    ```
    Include both the message and the signature on the transport USB.

13. **Write PUBLIC outputs to USB stick #2** (never the WIF):
    ```bash
    mkdir -p /media/ubuntu/<USB2>/ratatoskr-treasury
    cat > /media/ubuntu/<USB2>/ratatoskr-treasury/output.txt <<EOF
    RATR MAINNET TREASURY — v1.0
    Generated:     $(date -u +%Y-%m-%dT%H:%M:%SZ)
    Address:       $ADDR
    Pubkey:        $PUBKEY
    scriptPubKey:  $SCRIPT_PUB_KEY
    Pubkey hash:   $PUBKEY_HASH
    Signed-message (proves operator control — anyone can verifymessage):
      message:   $MESSAGE
      signature: $SIGNATURE
    EOF
    sync
    ```

    **NEVER write the WIF to any USB stick.** Paper + metal only.

---

## 3. Independent verification (still air-gapped)

Before leaving the air-gapped machine, verify the WIF you wrote down
actually regenerates the same address. This catches illegible handwriting
— the #1 cause of cold-wallet loss.

```bash
# stop the daemon
$CLI stop
sleep 3

# wipe the wallet
rm -rf /tmp/treasury-keygen

# restart fresh
./ratatoskrd -datadir=/tmp/treasury-keygen -daemon -server=1 -listen=0 \
             -connect=0 -dnsseed=0 -maxconnections=0 \
             -rpcport=18181 -rpcuser=keygen -rpcpassword=keygen-only-local-ignore

# poll until responsive (same pattern as §2.3)
for i in $(seq 1 30); do
  $CLI getblockchaininfo >/dev/null 2>&1 && break
  sleep 1
done

$CLI -named createwallet wallet_name=verify descriptors=false
VCLI="$CLI -rpcwallet=verify"

# import the WIF YOU JUST HAND-WROTE — type it manually from paper, not
# copy-paste. This tests that your handwriting is readable.
read -p "Type WIF from paper: " WIF_FROM_PAPER
$VCLI importprivkey "$WIF_FROM_PAPER" "verify" false

# dump info for the imported key
VERIFY_ADDR=$($VCLI getaddressesbylabel verify | python3 -c "import json,sys; print(list(json.load(sys.stdin).keys())[0])")
echo "Recovered from paper: $VERIFY_ADDR"
echo "Original generated:   $ADDR"

[[ "$VERIFY_ADDR" == "$ADDR" ]] && echo "MATCH ✓" || { echo "MISMATCH — START OVER"; exit 1; }
```

**If it doesn't match, the handwritten WIF is wrong. Start over from §2.**

Stop the daemon:
```bash
$CLI stop
```

---

## 4. Leave the air-gapped session

1. **Unmount + eject USB stick #2** (transport stick). Verify it contains
   only `output.txt`:
   ```bash
   ls -la /media/ubuntu/<USB2>/ratatoskr-treasury/
   # expect only output.txt
   umount /media/ubuntu/<USB2>
   ```

2. **Physically destroy USB stick #1** (binary) — physical crush / drill
   / shred. Or reformat and reuse for non-sensitive work. Your call, but
   paranoid is safer.

3. **Power off the air-gapped laptop.** Since the Ubuntu live session is
   non-persistent and `/tmp` is tmpfs, nothing is saved to disk. The WIF
   exists only on paper + metal.

4. **Immediately store paper + metal backups.** Do not leave them on a
   desk. Do not photograph them. Paper copy 1 → home safe. Paper copy 2
   → off-site (safe deposit box or trusted family safe, in sealed
   envelope marked "SEALED — DO NOT OPEN UNLESS I ASK"). Metal → separate
   location from both paper copies.

5. **Post-ceremony USB #1 sanity check.** Plug USB #1 into a clean
   machine and verify no wallet files landed on it (some live-USB configs
   write `/tmp` elsewhere). If any `wallet.dat` or `.keygen` files exist,
   wipe + destroy immediately.

---

## 5. Bake the script into chainparams

On your online dev machine.

1. Plug in USB #2, copy `output.txt` to the RATR repo as `/tmp/treasury-output.txt`.

2. Open `src/chainparams.cpp`, find the `CMainParams` block (around
   lines 185–197), and the line:
   ```cpp
   // PLACEHOLDER: OP_RETURN-"RATR" burn script. Any value sent here is unspendable.
   // MUST BE REPLACED with a real P2PKH/P2SH treasury script in a follow-up commit
   // before mainnet launch on 2026-06-01.
   consensus.treasuryPaymentScript = CScript() << OP_RETURN << ParseHex("52415452");
   ```

3. Replace with the **ONLY valid form** (the iterator-pair / vector
   constructors of `CScript` are explicitly deleted in `src/script/script.h`
   line 427 — do NOT attempt `CScript(ParseHex(...))`, it will not compile):

   ```cpp
   // Ratatoskr mainnet treasury — single-sig, air-gapped keygen YYYY-MM-DD
   // Address: R... (see doc/treasury-keygen-runbook.md; signed message in release notes)
   consensus.treasuryPaymentScript = CScript()
       << OP_DUP
       << OP_HASH160
       << ParseHex("<PUBKEY_HASH_20_BYTES_HEX>")
       << OP_EQUALVERIFY
       << OP_CHECKSIG;
   ```

   Replace `<PUBKEY_HASH_20_BYTES_HEX>` with the value from §2.8
   (40 hex chars, the middle of the scriptPubKey between `76a914` and `88ac`).

4. **Leave the testnet / devnet / regtest blocks alone.** Those keep
   their `OP_RETURN "RATR"` placeholders — we're only changing mainnet.

5. **Pre-build check — run the placeholder guard:**
   ```bash
   # From repo root
   awk '/CMainParams\(\)/{in_main=1} /CTestNetParams\(\)/{in_main=0} in_main && /ParseHex\("52415452"\)/{print "FAIL line "NR": mainnet still has placeholder"; exit 1}' src/chainparams.cpp
   echo "OK: mainnet placeholder replaced"
   ```

6. **Run the full test suite:**
   ```bash
   make check
   ```
   All tests must pass. If anything breaks, STOP — the treasury change
   broke something. Investigate before committing.

7. **Commit with focused diff:**
   ```bash
   git checkout -b treasury-address
   git diff src/chainparams.cpp   # must be ONLY the treasuryPaymentScript line + comment
   git commit -am "chainparams: set mainnet treasury to R..."
   git push origin treasury-address
   # open PR, review one more time, merge to main
   ```

8. **Rebuild all three tarballs** (Linux + Windows + macOS-arm64) from
   the post-merge commit. Re-sha256, re-publish. Update the GitHub
   release page with new hashes.

### CI guard (add this to the release workflow BEFORE tagging v1.0.0)

In `.github/workflows/`, any release-build workflow should include:

```yaml
- name: Placeholder treasury guard
  run: |
    if awk '/CMainParams\(\)/{in_main=1} /CTestNetParams\(\)/{in_main=0} in_main && /ParseHex\("52415452"\)/{exit 1}' src/chainparams.cpp; then
      echo "OK: mainnet placeholder is replaced"
    else
      echo "FAIL: mainnet chainparams still has placeholder OP_RETURN 'RATR' script"
      echo "This would burn 10% of issuance forever. Do not ship."
      exit 1
    fi
```

This ensures a v1.0.0 tarball can NEVER be built with the placeholder still
in place.

---

## 6. Public verification

Before launch, publish:

1. **The treasury address** (`R...`) in release notes, README, whitepaper,
   launch announcement — every public surface.

2. **The signed message produced air-gapped in §2.12.** Post it alongside
   the address. Anyone can verify with:
   ```bash
   ratatoskr-cli verifymessage "R..." "<SIGNATURE>" "RATR treasury v1.0 confirmed by project operator. Launch block 1 is 2026-06-01 00:00 UTC."
   # must return true
   ```

3. **The `chainparams.cpp` commit hash** on GitHub so verifiers can
   independently check that the script in the binary matches the
   published address. Include in release notes as:
   ```
   Treasury address baked at commit <sha>.
   Reproducible build instructions: doc/build-unix.md
   ```

---

## 7. Post-launch sanity check

Immediately after block 1 is mined:

```bash
ratatoskr-cli getblock $(ratatoskr-cli getblockhash 1) 2
# Look for a coinbase (vin[0].coinbase present) output with:
# - scriptPubKey.addresses == ["R..."] (your treasury address)
# - value == 5.00000000 (exactly 10% of 50 RATR subsidy for block 1)
```

Every block from block 1 onward MUST contain a treasury output at the
correct amount. If any block is missing it or has wrong amount, **stop
the chain immediately** (coordinate with pool operators) — the consensus
code or chainparams didn't deploy correctly.

This is also why we test on testnet first (§0.5).

---

## 8. Multisig upgrade (v1.1+ roadmap)

v1.0 treasury is intentionally single-sig for launch simplicity. v1.1
hard-fork migrates to 3-of-5 P2SH multisig. Migration path:

1. Generate 5 new keys on 5 separate air-gapped machines (this runbook ×5)
2. Construct P2SH redeem script, compute new treasury address
3. Hard fork: at activation height H, treasury script switches from old
   P2PKH to new P2SH
4. Remaining balance in old address is swept by a signed tx from the v1.0
   single-sig key, into the new multisig

This runbook will be revised (and stress-tested) for v1.1. Do not attempt
multisig migration without a fresh runbook review.

---

## What NOT to do

- ❌ **Do not generate the key on your daily Windows box.**
- ❌ **Do not type the WIF into any website, password manager, note app,
  or anything that syncs to cloud.**
- ❌ **Do not photograph the paper backup.** Camera metadata + cloud sync
  = leaked WIF.
- ❌ **Do not dictate the WIF out loud** (microphones, Alexa, neighbors).
- ❌ **Do not tell anyone what coin the WIF is for when handing them the
  envelope.** "Sealed envelope, do not open unless I ask" is enough.
- ❌ **Do not skip §3 (independent verification)** — illegible handwriting
  is the #1 cause of cold-wallet loss.
- ❌ **Do not skip §0.5 (testnet dress rehearsal)** — first-try success
  on mainnet is a bad bet.
- ❌ **Do not publish the WIF**, ever, to anyone, for any reason.
- ❌ **Do not use `CScript(ParseHex(...))` in chainparams** — it will not
  compile (the constructor is deleted in `src/script/script.h`).
- ❌ **Do not skip the pre-tag placeholder guard (§5)** — launching with
  the burn placeholder destroys ~5% of supply in year 1.
- ❌ **Do not do the signmessage step on an online machine after the
  ceremony** — it requires bringing the WIF online, defeating the whole
  air-gap. Sign air-gapped (§2.12) and carry the signature out on USB #2.
- ❌ **Do not default to descriptor wallets** (`createwallet` without the
  `descriptors=false` flag). `dumpprivkey` does not work on descriptor
  wallets in modern Dash/Bitcoin.
