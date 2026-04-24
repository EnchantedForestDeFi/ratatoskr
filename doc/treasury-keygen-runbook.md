# Treasury Address Generation Runbook (v1.0.0-alpha)

**Goal:** Generate the Ratatoskr mainnet treasury address using an air-gapped
machine, in a way that is (a) reproducible by outside verifiers, (b) backed up
physically, and (c) never exposes the private key to any networked system.

**Output:**
1. A P2PKH mainnet address starting with `R...`
2. A `CScript` hex blob ready to paste into `src/chainparams.cpp`
3. Two offline backups of the WIF-encoded private key

**Time budget:** ~45 minutes end to end (unhurried).

**When to run this:** No earlier than 2 weeks before mainnet launch
(2026-06-01). The treasury script must be committed, tarballs rebuilt,
hashes published, and tested on testnet before launch day.

---

## 0. What you need

### Hardware
- [ ] A laptop you are willing to keep offline forever — or — a laptop
      you can wipe immediately after. A Raspberry Pi or old ThinkPad
      works. **Do not use your daily-driver Windows box.**
- [ ] A fresh USB stick (brand new, or freshly formatted)
- [ ] A second USB stick for transporting the final address + pubkey back
      to an online machine (different stick from the one carrying the binary)
- [ ] Paper + pen for handwritten backup
- [ ] Optional but strongly recommended: a metal backup plate (CryptoSteel,
      Blockplate, or even stamped steel bolts)
- [ ] A camera-off phone / webcam-covered laptop (no ambient recording)

### Software
- [ ] A freshly-downloaded Ubuntu live USB (22.04 or 24.04, doesn't matter —
      we'll never `apt install` anything from it)
- [ ] The Ratatoskr Linux tarball (`ratatoskr-1.0.0-alpha-linux-x86_64.tar.gz`)
      plus its sha256

### Environment
- [ ] A private room, blinds drawn
- [ ] No networked devices within arm's reach (phone in another room)
- [ ] Ethernet unplugged, WiFi physically off if possible (BIOS hardware
      kill-switch, if available)

---

## 1. Prep the air-gapped machine

1. Boot the target laptop from the Ubuntu live USB. Pick **"Try Ubuntu"**
   (NOT install — we want no persistence).
2. Confirm the network is off:
   ```bash
   ip link show
   # every interface should be DOWN or state DOWN
   nmcli radio all
   # wifi / wwan / rfkill all = off
   ```
3. If anything shows UP, open Settings → Network and disable everything.
   For paranoia: pull the ethernet cable and use the hardware WiFi kill
   switch. Do not proceed until every interface is down.
4. Transfer `ratatoskr-1.0.0-alpha-linux-x86_64.tar.gz` from the fresh USB
   stick to the Ubuntu live session (`/tmp` is fine — it vanishes on
   reboot anyway).
5. Verify the sha256 against the published hash:
   ```bash
   sha256sum /tmp/ratatoskr-1.0.0-alpha-linux-x86_64.tar.gz
   # compare to https://github.com/EnchantedForestDeFi/ratatoskr/releases
   ```
   **Stop and restart from step 0 if it doesn't match.**

---

## 2. Generate the private key

1. Unpack and install the runtime deps (from the live USB — but we installed
   these earlier on build machines; on a clean live session you may need them):
   ```bash
   cd /tmp
   tar xzf ratatoskr-1.0.0-alpha-linux-x86_64.tar.gz
   cd ratatoskr-1.0.0-alpha-linux-x86_64/bin
   # If libs are missing, bring them from a staged USB — but DO NOT
   # apt install anything that might phone home. If the libs aren't
   # findable, use a machine that already has the RATR deps pre-staged.
   ```

2. Start the daemon in an isolated data dir with networking disabled via
   flags (belt-and-suspenders):
   ```bash
   mkdir -p /tmp/treasury-keygen
   ./ratatoskrd -datadir=/tmp/treasury-keygen \
                -daemon \
                -server=1 \
                -listen=0 \
                -connect=0 \
                -dnsseed=0 \
                -noconnect \
                -rpcport=18181 \
                -rpcuser=keygen -rpcpassword=keygen-only-local-ignore
   # wait ~10s for the rpc to come up
   sleep 10
   ```

3. Generate a fresh address + dump the private key:
   ```bash
   CLI="./ratatoskr-cli -datadir=/tmp/treasury-keygen -rpcport=18181 -rpcuser=keygen -rpcpassword=keygen-only-local-ignore"

   # create the wallet if the build requires it
   $CLI createwallet treasury 2>/dev/null || true

   # generate the address
   ADDR=$($CLI getnewaddress "ratatoskr-treasury-v1")
   echo "Address: $ADDR"

   # extract the private key (WIF) and pubkey
   WIF=$($CLI dumpprivkey "$ADDR")
   PUBKEY=$($CLI getaddressinfo "$ADDR" | grep -oP '"pubkey"\s*:\s*"\K[^"]+')
   SCRIPT_PUB_KEY=$($CLI getaddressinfo "$ADDR" | grep -oP '"scriptPubKey"\s*:\s*"\K[^"]+')

   echo "Address:      $ADDR"
   echo "Pubkey:       $PUBKEY"
   echo "scriptPubKey: $SCRIPT_PUB_KEY"
   echo "WIF (PRIVATE, DO NOT LEAK): $WIF"
   ```

4. Sanity check — the address MUST start with `R`:
   ```bash
   [[ "$ADDR" =~ ^R ]] && echo "OK: mainnet prefix" || echo "WRONG PREFIX — abort"
   ```

5. **Write the WIF on paper, by hand. Twice. Read it back to verify both
   copies match the screen.** One copy goes in a safe at home, the other
   goes in an off-site safe deposit box (or trusted family member's safe).
   If you have a metal backup plate, also stamp it now.

6. Write the **address** and **scriptPubKey** to a text file on the
   transport USB stick (USB stick #2 — not the one with the binary):
   ```bash
   cat > /media/.../treasury-output.txt <<EOF
   RATR treasury v1.0
   generated: $(date -u +%Y-%m-%dT%H:%M:%SZ)
   address:      $ADDR
   pubkey:       $PUBKEY
   scriptPubKey: $SCRIPT_PUB_KEY
   EOF
   ```
   The WIF does NOT go on any USB stick. Paper only.

---

## 3. Independent verification (still air-gapped)

Before leaving the air-gapped machine, verify that the WIF you wrote
down actually regenerates the same address:

```bash
# stop the daemon
$CLI stop
sleep 3

# wipe the wallet
rm -rf /tmp/treasury-keygen

# restart fresh
./ratatoskrd -datadir=/tmp/treasury-keygen -daemon -server=1 -listen=0 \
             -connect=0 -dnsseed=0 -noconnect \
             -rpcport=18181 -rpcuser=keygen -rpcpassword=keygen-only-local-ignore
sleep 10

$CLI createwallet verify 2>/dev/null || true

# import the WIF you wrote on paper (type it manually — test that your
# handwriting is readable)
$CLI importprivkey "WIF_FROM_PAPER" "verify" false

# get the address of the imported key
VERIFY_ADDR=$($CLI getaddressesbylabel verify | grep -oP '"R[^"]+' | head -1 | tr -d '"')
echo "Recovered from paper: $VERIFY_ADDR"
echo "Original generated:   $ADDR"
[[ "$VERIFY_ADDR" == "$ADDR" ]] && echo "MATCH ✓" || echo "MISMATCH — START OVER"
```

**If it doesn't match, the handwritten WIF is wrong. Start over from step 2.**

Stop the daemon:
```bash
$CLI stop
```

---

## 4. Leave the air-gapped session

1. Eject USB stick #2 (transport stick, contains only public address).
2. Physically destroy USB stick #1 (binary) — or keep it, reformat, and
   reuse for non-sensitive work. Your call.
3. Power off the air-gapped laptop. Since the Ubuntu live session is
   non-persistent, nothing is saved to disk. The WIF only exists on paper
   (and optionally metal).
4. Store paper backups immediately — do not leave them on a desk.

---

## 5. Bake the script into chainparams

On your online dev machine:

1. Plug in USB #2, copy `treasury-output.txt`.
2. Open `src/chainparams.cpp` and find the mainnet block (CMainParams):
   ```cpp
   consensus.treasuryPaymentScript = CScript() << OP_RETURN << ParseHex("52415452");
   ```
3. Replace with the real P2PKH script. For a pubkey hash address, the
   scriptPubKey from step 2 is:
   ```
   76a914<20-byte-pubkey-hash>88ac
   ```
   So the replacement line is:
   ```cpp
   // Ratatoskr mainnet treasury (single-sig, air-gapped keygen 2026-05-XX)
   // Address: R...
   consensus.treasuryPaymentScript = CScript(ParseHex("<SCRIPT_PUB_KEY_HEX>"));
   ```
   Or, safer (explicit opcodes):
   ```cpp
   consensus.treasuryPaymentScript = CScript() << OP_DUP << OP_HASH160
       << ParseHex("<20-BYTE-PUBKEY-HASH>") << OP_EQUALVERIFY << OP_CHECKSIG;
   ```
4. Leave the testnet/devnet burn placeholders alone.
5. Commit:
   ```bash
   git checkout -b treasury-address
   git commit -am "chainparams: set mainnet treasury address to R..."
   git push origin treasury-address
   # open PR, merge to rebrand → main
   ```
6. Rebuild both tarballs (Linux + Windows) from the post-merge commit.
   Re-sha256sum, re-publish, update the release page.

---

## 6. Public verification

Before launch, publish:

1. The **treasury address** (`R...`) in release notes, README, whitepaper,
   launch announcement — everywhere.
2. A **signed message** from the treasury key, proving you control it:
   ```bash
   ratatoskr-cli signmessage "R..." "RATR treasury v1.0 confirmed by project operator. Launch block 1 is 2026-06-01 00:00 UTC."
   ```
   Post the signature alongside the address. Anyone can verify with
   `verifymessage`.
3. The `chainparams.cpp` commit hash on GitHub so verifiers can
   independently check that the script in the binary matches the
   published address.

---

## 7. Post-launch sanity check

Once block 1 is mined and the daemon is running:

```bash
ratatoskr-cli getblock $(ratatoskr-cli getblockhash 1) 2
# look for the coinbase output going to the treasury address
# it should be exactly 10% of subsidy (5 RATR for block 1)
```

Every block from block 1 onwards must contain a treasury output at the
correct amount. If any block is missing it, **stop the chain immediately**
— the consensus code didn't deploy correctly. That's why we also test
on testnet first (step 5).

---

## 8. Multisig upgrade (v1.1+ roadmap)

At v1.0 the treasury is single-sig. The v1.1 hard fork will migrate to a
3-of-5 multisig (P2SH). Migration path:

1. Generate 5 new keys on separate air-gapped machines (this runbook ×5).
2. Construct P2SH redeem script, compute new treasury address.
3. Hard fork: at activation height H, the treasury script switches from
   old P2PKH to new P2SH.
4. Remaining balance in old address is swept by a signed tx from the
   v1.0 single-sig key, into the new multisig address.

This runbook gets a v1.1 revision closer to that hard fork.

---

## What NOT to do

- ❌ Do not generate the key on your daily Windows box
- ❌ Do not type the WIF into any website, password manager, or note app
  that syncs to the cloud
- ❌ Do not photograph the paper backup
- ❌ Do not dictate the WIF out loud (microphones, neighbors)
- ❌ Do not tell anyone (even trusted family) what coin the WIF is for
  while handing them the envelope — "sealed envelope, do not open
  unless I ask" is enough instruction
- ❌ Do not skip step 3 (independent verification) — the single most common
  way cold wallets go wrong is illegible handwriting
- ❌ Do not publish the WIF, ever, to anyone, for any reason
