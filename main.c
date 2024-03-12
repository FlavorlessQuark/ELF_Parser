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
    char *sh_strtb;

    int class;

    class = read_header_info(file, hdr_info);

    if (class == ELFCLASS32)
    {
        // Elf32_Ehdr header;

        // memcpy(header.e_ident, hdr_info, EI_NIDENT);
        // // read_header32(&header);
        printf("32 bit files not supported");
    }
    else if (class == ELFCLASS64)
    {
        Elf64_Ehdr header;
        Elf64_Shdr *section_table;
        Elf64_Phdr *program_table;
        long count;

        memcpy(header.e_ident, hdr_info, EI_NIDENT);
        read_header64(file, &header);


        /*
            If the number of sections is greater than or equal to SHN_LORESERVE (0xff00),
            e_shnum has the value SHN_UNDEF (0) and the actual number of section header table entries
            is contained in the sh_size field of the section header at index 0.
            Otherwise, the sh_size member of the initial entry contains 0.
         */
        if (header.e_shnum == SHN_UNDEF)
        {
            Elf64_Shdr tmp_section_table;

            fseek(file, header.e_shoff, SEEK_SET);
            fread(&section_table, sizeof(Elf64_Shdr), 1, file);
            count = section_table->sh_size;
            section_table = malloc(header.e_shentsize * section_table->sh_size);
        }
        else {
            section_table = malloc(header.e_shentsize * header.e_shnum);
            count = header.e_shnum;
        }

        if (count != 0)
        {
            fseek(file, header.e_shoff, SEEK_SET);
            fread(section_table, header.e_shentsize, count, file);
            read_sections64(file, section_table, count, header.e_shstrndx);
        }
        else
            printf("There are no section headers in this file\n");

        if (header.e_phnum != 0)
        {
            program_table = malloc( header.e_phentsize * header.e_phnum);
            fseek(file, header.e_phoff, SEEK_SET);
            fread(program_table, header.e_phentsize, header.e_phnum, file );
        }
        else
            printf("There are no program headers in this file\n");
        // read program headers
        /// Display diffsections infos
        //read dynamic sectiob
        // relocation sections
        // symtabs
        // version info
        //notes
    }
    else
        D_ERROR("Invalid ELF class")
}
