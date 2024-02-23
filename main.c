#include "parser.h"

int main(int argc, char **argv)
{

    if (argc != 2)
        D_ERROR("Usage : elfparse [ELF file]\n");

    FILE *file;
    char *filename = argv[1];


    if ((file = fopen(filename, "r")) == NULL)
        D_ERROR("Can't open file\n");

// --------------- Begin parse

    unsigned char hdr_info[EI_NIDENT];

    int class;

    class = read_header_info(file, hdr_info);

    if (class == ELFCLASS32)
    {
        Elf32_Ehdr header;

        memcpy(header.e_ident, hdr_info, EI_NIDENT);
        // read_header32(&header);

    }
    else if (class == ELFCLASS64)
    {
        Elf64_Ehdr header;
        Elf64_Shdr *section_table;

        memcpy(header.e_ident, hdr_info, EI_NIDENT);
        read_header64(file, &header);


        // section_table = malloc(header.e_shentsize * header.e_shnum);

    }
    else
        D_ERROR("Invalid ELF class")
}
