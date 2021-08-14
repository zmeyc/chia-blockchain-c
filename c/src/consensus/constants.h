#pragma once

#include <stdint.h>

#include "../types/blockchain_format/sized_bytes.h"
#include "../util/types.h"

#include "network_type.h"

struct ConsensusConstants {
    uint32_t slot_blocks_target; // How many blocks to target per sub-slot
    uint8_t min_blocks_per_challenge_block; // How many blocks must be created per slot (to make challenge sb)
    // Max number of blocks that can be infused into a sub-slot.
    // Note: this must be less than SUB_EPOCH_BLOCKS/2, and > SLOT_BLOCKS_TARGET
    uint32_t max_sub_slot_blocks;
    uint32_t num_sps_sub_slot; // The number of signage points per sub-slot (including the 0th sp at the sub-slot start)

    uint64_t sub_slot_iters_starting; // The sub_slot_iters for the first epoch
    uint128_t difficulty_constant_factor; // Multiplied by the difficulty to get iterations
    uint64_t difficulty_starting; // The difficulty for the first epoch
    // The maximum factor by which difficulty and sub_slot_iters can change per epoch
    uint32_t difficulty_change_max_factor;
    uint32_t sub_epoch_blocks; // The number of blocks per sub-epoch
    uint32_t epoch_blocks; // The number of blocks per sub-epoch, must be a multiple of SUB_EPOCH_BLOCKS

    int significant_bits; // The number of bits to look at in difficulty and min iters. The rest are zeroed
    int discriminant_size_bits; // Max is 1024 (based on ClassGroupElement int size)
    int number_zero_bits_plot_filter; // H(plot id + challenge hash + signage point) must start with these many zeroes
    int min_plot_size;
    int max_plot_size;
    int sub_slot_time_target; // The target number of seconds per sub-slot
    int num_sp_intervals_extra; // The difference between signage point and infusion point (plus required_iters)
    int max_future_time; // The next block can have a timestamp of at most these many seconds more
    int number_of_timestamps; // Than the average of the last NUMBER_OF_TIMESTAMPS blocks
    // Used as the initial cc rc challenges, as well as first block back pointers, and first SES back pointer
    // We override this value based on the chain being run (testnet0, testnet1, mainnet, etc)
    struct Bytes32 genesis_challenge;
    // Forks of chia should change this value to provide replay attack protection
    struct Bytes32 agg_sig_me_additional_data;
    struct Bytes32 genesis_pre_farm_pool_puzzle_hash; // The block at height must pay out to this pool puzzle hash
    struct Bytes32 genesis_pre_farm_farmer_puzzle_hash; // The block at height must pay out to this farmer puzzle hash
    int max_vdf_witness_size; // The maximum number of classgroup elements within an n-wesolowski proof
    // Size of mempool = 10x the size of block
    int mempool_block_buffer;
    // Max coin amount uint(1 << 64) - 1. This allows coin amounts to fit in 64 bits. This is around 18M chia.
    uint64_t max_coin_amount;
    // Max block cost in clvm cost units
    uint64_t max_block_cost_clvm;
    // Cost per byte of generator program
    int cost_per_byte;

    uint8_t weight_proof_threshold;
    uint32_t weight_proof_recent_blocks;
    uint32_t max_block_count_per_requests;
    uint32_t blocks_cache_size;
    enum NetworkType network_type;
    uint32_t max_generator_size;
    uint32_t max_generator_ref_list_size;
    uint64_t pool_sub_slot_iters;
};

