#include <stdio.h>
#include <stdint.h>
#include "miniz.h"

#define MEM_SIZE (32 * 1048576)

uint8_t GB_ROM[MEM_SIZE];
uint8_t GB_ROM_COMP[MEM_SIZE];

#define BANK_SIZE 16384

/**
 * Usage:
 *     ./compare uncompressed_rom compressed_rom
 *
 */
int main(int argc, char *argv[])
{
    char fout_buf[256];

    uint8_t *bank = NULL;
    bank = malloc(BANK_SIZE);
    size_t comp_bytes = 0;

    /* Read in UNcompressed rom to memory */
    {
        FILE *fp = fopen(argv[1], "r");
        comp_bytes = fread(GB_ROM, 1, sizeof GB_ROM, fp);
        fclose(fp);
    }

    /* Read in compressed rom to memory */
    {
        FILE *fp = fopen(argv[2], "r");
        comp_bytes = fread(GB_ROM_COMP, 1, sizeof GB_ROM_COMP, fp);
        fclose(fp);
    }

    tinfl_decompressor decomp;

    size_t src_offset = 0;
    uint8_t n_banks = 0;
    do{
        tinfl_init(&decomp);
        tinfl_status status;
        size_t src_buf_size = comp_bytes - src_offset;
        size_t dst_buf_size = BANK_SIZE;

        status = tinfl_decompress(&decomp,
                &GB_ROM_COMP[src_offset], &src_buf_size,
                bank, bank, &dst_buf_size,
                0
                );
        assert(status == TINFL_STATUS_DONE);
        assert(dst_buf_size == BANK_SIZE);

        assert(0 == memcmp(bank, &GB_ROM[n_banks * BANK_SIZE], BANK_SIZE));

        src_offset += src_buf_size;
        n_banks += 1;
    }while(src_offset < comp_bytes);

    //bank[16384] = 0xFF;  // testing if valgrind will detect this
    printf("Parsed %d banks\n", n_banks);
    printf("All decoded banks matched.\n");
    free(bank);

    return 0;
}
