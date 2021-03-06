/*
20140709
Jan Mojzis
Public domain.
*/

#include "misc.h"
#include "crypto_core_aes256encrypt.h"

static unsigned char key[crypto_core_aes256encrypt_KEYBYTES + 16];
static unsigned char out[crypto_core_aes256encrypt_OUTPUTBYTES + 16];
static unsigned char in[crypto_core_aes256encrypt_INPUTBYTES + 16];

static unsigned char o[32] = {
    0x18, 0xbb, 0x31, 0xfc, 0xdd, 0xe0, 0x1c, 0x12, 
    0x48, 0xd7, 0x9c, 0x51, 0xc8, 0x2e, 0x48, 0x60, 
    0x92, 0x46, 0x6e, 0x6a, 0xb4, 0xf3, 0x71, 0x24, 
    0x49, 0xe4, 0xcf, 0x79, 0xc6, 0x14, 0x1f, 0x27
};

static void test_alignment(void) {

    long long i;

    for (i = 0; i < 16; ++i) {
        crypto_core_aes256encrypt(out + i, in + i, key + i, 0);
    }
}

static void test_rand(void) {

    long long i;

    for (i = 0; i < 256; ++i) {
        pseudorandombytes(key, sizeof key);
        pseudorandombytes(in, sizeof in);
        crypto_core_aes256encrypt(out, in, key, 0);
        checksum(out, crypto_core_aes256encrypt_OUTPUTBYTES);
    }
    fail_whenbadchecksum(o);
}

/* NIST testvectors from Special Publication 800-38A */
static struct vector {
    const unsigned char key[crypto_core_aes256encrypt_KEYBYTES];
    const unsigned char in[crypto_core_aes256encrypt_INPUTBYTES];
    const unsigned char out[crypto_core_aes256encrypt_OUTPUTBYTES];
} vectors[] = {
    {
        { 
            0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
            0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
        },
        {
            0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
        },
        {
            0x0b, 0xdf, 0x7d, 0xf1, 0x59, 0x17, 0x16, 0x33, 0x5e, 0x9a, 0x8b, 0x15, 0xc8, 0x60, 0xc5, 0x02
        }
    },
    {
        {
            0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
            0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
        },
        {
            0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xff, 0x00
        },
        {
            0x5a, 0x6e, 0x69, 0x9d, 0x53, 0x61, 0x19, 0x06, 0x54, 0x33, 0x86, 0x3c, 0x8f, 0x65, 0x7b, 0x94

        }
    },
    {
        {
            0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
            0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
        },
        {
            0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xff, 0x01
        },
        {
            0x1b, 0xc1, 0x2c, 0x9c, 0x01, 0x61, 0x0d, 0x5d, 0x0d, 0x8b, 0xd6, 0xa3, 0x37, 0x8e, 0xca, 0x62
        }
    },
    {
        {
            0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
            0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
        },
        {
            0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xff, 0x02
        },
        {
            0x29, 0x56, 0xe1, 0xc8, 0x69, 0x35, 0x36, 0xb1, 0xbe, 0xe9, 0x9c, 0x73, 0xa3, 0x15, 0x76, 0xb6
        }
    },

};

static void test_vector(void) {

    long long i, j;
    unsigned char out[crypto_core_aes256encrypt_OUTPUTBYTES];

    for (i = 0; i < 4; ++i) {
        crypto_core_aes256encrypt(out, vectors[i].in, vectors[i].key, 0);
        for (j = 0; j < crypto_core_aes256encrypt_OUTPUTBYTES; ++j)
            if (out[j] != vectors[i].out[j])
                fail("crypto_core_aes256encrypt() failure");
    }
}

int main(void) {

    test_vector();
    test_alignment();
    test_rand();

    _exit(0);
}
