#include <stdbool.h>

#include <gmp.h>

#include "../../types/blockchain_format/sized_bytes.h"

#include "classgroup.h"

void get_discriminant(mpz_t result, const struct Bytes32 *challenge, int size_bytes);
bool verify_vdf(const mpz_t disc, const struct Bytes100 *input_el, const uint8_t *output, size_t output_size,
    uint64_t number_of_iterations, uint64_t discriminant_size, uint8_t witness_type);

struct VDFInfo {
    struct Bytes32 challenge; // Used to generate the discriminant (VDF group)
    uint64_t number_of_iterations;
    struct ClassgroupElement output;
};

struct VDFProof {
    uint8_t witness_type;
    uint8_t *witness;
    size_t witness_size;
    bool normalized_to_identity;
};

// If target_vdf_info is passed in, it is compared with info.
bool is_valid_vdf_proof(
    const struct VDFProof *proof,
    const struct ConsensusConstants *constants,
    const struct ClassgroupElement *input_el,
    const struct VDFInfo *info
    //const struct VDFInfo *target_vdf_info // optional
);

// Stores, for a given VDF, the field that uses it.
enum CompressibleVDFField {
    COMPRESSIBLE_VDF_FIELD_CC_EOS_VDF = 1,
    COMPRESSIBLE_VDF_FIELD_ICC_EOS_VDF = 2,
    COMPRESSIBLE_VDF_FIELD_CC_SP_VDF = 3,
    COMPRESSIBLE_VDF_FIELD_CC_IP_VDF = 4
};

