#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../types/blockchain_format/classgroup.h"
#include "../types/blockchain_format/sized_bytes.h"
#include "../types/blockchain_format/sub_epoch_summary.h"
#include "../util/types.h"

#include "pot_iterations.h"

// This class is not included or hashed into the blockchain, but it is kept in memory as a more
// efficient way to maintain data about the blockchain. This allows us to validate future blocks,
// difficulty adjustments, etc, without saving the whole header block in memory.
struct BlockRecord {
    struct Bytes32 header_hash;
    struct Bytes32 prev_hash; // Header hash of the previous block
    uint32_t height;
    uint128_t weight; // Total cumulative difficulty of all ancestor blocks since genesis
    uint128_t total_iters; // Total number of VDF iterations since genesis, including this block
    uint8_t signage_point_index;
    struct ClassgroupElement challenge_vdf_output; // This is the intermediary VDF output at ip_iters in challenge chain
    struct {
        struct ClassgroupElement value;
        bool has_value;
    } infused_challenge_vdf_output; // This is the intermediary VDF output at ip_iters in infused cc, iff deficit <= 3
    struct Bytes32 reward_infusion_new_challenge; // The reward chain infusion output, input to next VDF
    struct Bytes32 challenge_block_info_hash; // Hash of challenge chain data, used to validate end of slots in the future
    uint64_t sub_slot_iters; // Current network sub_slot_iters parameter
    struct Bytes32 pool_puzzle_hash; // Need to keep track of these because Coins are created in a future block
    struct Bytes32 farmer_puzzle_hash;
    uint64_t required_iters; // The number of iters required for this proof of space
    uint8_t deficit; // A deficit of 16 is an overflow block after an infusion. Deficit of 15 is a challenge block
    bool overflow;
    uint32_t prev_transaction_block_height;

    // Transaction block (present iff is_transaction_block)
    struct {
        uint64_t value;
        bool has_value;
    } timestamp;
    struct {
        struct Bytes32 value;
        bool has_value;
    } prev_transaction_block_hash; // Header hash of the previous transaction block
    struct {
        uint64_t value;
        bool has_value;
    } fees;
    struct CoinListElement *reward_claims_incorporated;

    // Slot (present iff this is the first SB in sub slot)
    struct Bytes32ListElement *finished_challenge_slot_hashes;
    struct Bytes32ListElement *finished_infused_challenge_slot_hashes;
    struct Bytes32ListElement *finished_reward_slot_hashes;

    // Sub-epoch (present iff this is the first SB after sub-epoch)
    struct {
        struct SubEpochSummary value;
        bool has_value;
    } sub_epoch_summary_included;
};

bool block_record_is_transaction_block(const struct BlockRecord *block_record);
bool block_record_first_in_sub_slot(const struct BlockRecord *block_record);
bool block_record_is_challenge_block(const struct BlockRecord *block_record, const struct ConsensusConstants *constants);

struct SPSubSlotTotalItersResult {
    enum {
        SPSSTI_SUCCESS,
        SPSSTI_CIPI_ERROR
    } code;
    union {
        uint128_t value;
        struct CalculateIPItersResult cipi_error;
    };
};

struct SPSubSlotTotalItersResult block_record_sp_sub_slot_total_iters(const struct BlockRecord *block_record, const struct ConsensusConstants *constants);

struct IPSubSlotTotalItersResult {
    enum {
        IPSSTI_SUCCESS,
        IPSSTI_CIPI_ERROR
    } code;
    union {
        uint128_t value;
        struct CalculateIPItersResult cipi_error;
    };
};

struct IPSubSlotTotalItersResult block_record_ip_sub_slot_total_iters(const struct BlockRecord *block_record, const struct ConsensusConstants *constants);

struct CalculateSPItersResult block_record_sp_iters(const struct BlockRecord *block_record, const struct ConsensusConstants *constants);
struct CalculateIPItersResult block_record_ip_iters(const struct BlockRecord *block_record, const struct ConsensusConstants *constants);

struct SPTotalItersResult {
    enum {
        SPTI_SUCCESS,
        SPTI_SPSSTI_ERROR,
        SPTI_CSPI_ERROR
    } code;
    union {
        uint128_t value;
        struct SPSubSlotTotalItersResult spssti_error;
        struct CalculateSPItersResult cspi_error;
    };
};

struct SPTotalItersResult block_record_sp_total_iters(const struct BlockRecord *block_record, const struct ConsensusConstants *constants);

