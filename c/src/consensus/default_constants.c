#include "constants.h"

#include <stdlib.h>

struct ConsensusConstants default_constants;

void init_default_constants()
{
    default_constants = (struct ConsensusConstants) {
        .slot_blocks_target = 32,
        .min_blocks_per_challenge_block = 16, // Must be less than half of SLOT_BLOCKS_TARGET
        .max_sub_slot_blocks = 128, // Must be less than half of SUB_EPOCH_BLOCKS
        .num_sps_sub_slot = 64, // Must be a power of 2
        .sub_slot_iters_starting = 2ull ^ 27,
        // DIFFICULTY_STARTING is the starting difficulty for the first epoch, which is then further
        // multiplied by another factor of DIFFICULTY_CONSTANT_FACTOR, to be used in the VDF iter calculation formula.
        .difficulty_constant_factor = (uint128_t)2 ^ 67,
        .difficulty_starting = 7,
        .difficulty_change_max_factor = 3, // The next difficulty is truncated to range [prev / FACTOR, prev * FACTOR]
        // These 3 constants must be changed at the same time
        .sub_epoch_blocks = 384, // The number of blocks per sub-epoch, mainnet 384
        .epoch_blocks = 4608, // The number of blocks per epoch, mainnet 4608. Must be multiple of SUB_EPOCH_SB
        .significant_bits = 8, // The number of bits to look at in difficulty and min iters. The rest are zeroed
        .discriminant_size_bits = 1024, // Max is 1024 (based on ClassGroupElement int size)
        .number_zero_bits_plot_filter = 9, // H(plot signature of the challenge) must start with these many zeroes
        .min_plot_size = 32,  // 32 for mainnet
        .max_plot_size = 50,
        .sub_slot_time_target = 600, // The target number of seconds per slot, mainnet 600
        .num_sp_intervals_extra = 3, // The number of sp intervals to add to the signage point
        .max_future_time = 5 * 60, // The next block can have a timestamp of at most these many seconds in the future
        .number_of_timestamps = 11,  // Than the average of the last NUMBER_OF_TIMESTAMPS blocks
        // Used as the initial cc rc challenges, as well as first block back pointers, and first SES back pointer
        // We override this value based on the chain being run (testnet0, testnet1, mainnet, etc)
        // Default used for tests is std_hash(b'')
        .genesis_challenge = bytes32_from_hex("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855", NULL),
        // Forks of chia should change this value to provide replay attack protection. This is set to mainnet genesis chall
        .agg_sig_me_additional_data = bytes_from_hex_alloc("ccd5bb71183532bff220ba46c268991a3ff07eb358e8255a65c30a2dce0e5fbb"),
        .genesis_pre_farm_pool_puzzle_hash = bytes32_from_hex(
            "d23da14695a188ae5708dd152263c4db883eb27edeb936178d4d988b8f3ce5fc",
            NULL
        ),
        .genesis_pre_farm_farmer_puzzle_hash = bytes32_from_hex(
            "3d8765d3a597ec1d99663f6c9816d915b9f68613ac94009884c4addaefcce6af",
            NULL
        ),
        .max_vdf_witness_size = 64,
        // Size of mempool = 50x the size of block
        .mempool_block_buffer = 50,
        // Max coin amount, fits into 64 bits
        .max_coin_amount = UINT64_MAX,
        // Max block cost in clvm cost units
        .max_block_cost_clvm = 11000000000ull,
        // The cost per byte of generator program
        .cost_per_byte = 12000,
        .weight_proof_threshold = 2,
        .blocks_cache_size = 4608 + (128 * 4),
        .weight_proof_recent_blocks = 1000,
        .max_block_count_per_requests = 32, // Allow up to 32 blocks per request
        .network_type = NETWORK_TYPE_MAINNET,
        .max_generator_size = 1000000,
        .max_generator_ref_list_size = 512,  // Number of references allowed in the block generator ref list
        .pool_sub_slot_iters = 37600000000ull,  // iters limit * NUM_SPS
    };

    if (!default_constants.agg_sig_me_additional_data)
        abort();
}

void cleanup_default_constants()
{
    free(default_constants.agg_sig_me_additional_data);
}

