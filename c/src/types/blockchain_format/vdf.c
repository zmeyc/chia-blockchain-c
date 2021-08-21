#include "vdf.h"

#include <stdbool.h>
#include <string.h>

#include <gmp.h>

#include "../../consensus/constants.h"
#include "../../types/blockchain_format/sized_bytes.h"
#include "../../thirdparty/chiavdf_bindings.h"

void get_discriminant(mpz_t result, const struct Bytes32 *challenge, int size_bytes)
{
    chiavdf_create_discriminant(result, challenge->value, sizeof challenge->value, size_bytes);
}

bool verify_vdf(const mpz_t disc, const struct Bytes100 *input_el, const uint8_t *output, size_t output_size,
    uint64_t number_of_iterations, uint64_t discriminant_size, uint8_t witness_type)
{
    return chiavdf_verify_n_wesolowski(
        disc,
        input_el->value,
        output,
        output_size,
        number_of_iterations,
        discriminant_size,
        witness_type
    );
}

// If target_vdf_info is passed in, it is compared with info.
bool is_valid_vdf_proof(
    const struct VDFProof *proof,
    const struct ConsensusConstants *constants,
    const struct ClassgroupElement *input_el,
    const struct VDFInfo *info
) {
    if (proof->witness_type + 1 > constants->max_vdf_witness_size)
        return false;

    uint8_t buf[sizeof info->output.data.value + proof->witness_size];
    memcpy(buf, info->output.data.value, sizeof info->output.data.value);
    memcpy(buf + sizeof info->output.data.value, proof->witness, proof->witness_size);
    
    mpz_t disc;
    mpz_init(disc);
    get_discriminant(/* out */ disc, &info->challenge, constants->discriminant_size_bits);
    // TODO: parallelize somehow, this might included multiple mini proofs (n weso)
    bool result = verify_vdf(
        disc,
        &input_el->data,
        buf, sizeof buf,
        info->number_of_iterations,
        constants->discriminant_size_bits,
        proof->witness_type
    );
    mpz_clear(disc);
    return result;
}

